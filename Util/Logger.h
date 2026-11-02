#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>

enum class LogLevel
{
	LOG_DEBUG = 0,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_FORCE,
	LOG_HIGH,
	LOG_NO_USE
};

namespace Core
{
	namespace Util
	{
		// 로그 출력 대상. 비트 조합 가능.
		enum LogSink : unsigned int
		{
			LOG_SINK_NONE = 0,
			LOG_SINK_CONSOLE = 1 << 0,
			LOG_SINK_FILE = 1 << 1,
		};

		class Logger
		{
		public:
			Logger() = default;
			~Logger() = default;

			// 한 줄의 최대 길이. 초과분은 잘리고 끝에 "~" 가 붙는다.
			static constexpr int MAX_LINE_LENGTH = 1024;

		private:
			// volatile 로 두는 이유:
			// Enabled() 가 헤더 인라인이라 핫 루프에서 레지스터에 캐시되면
			// 런타임 레벨 변경이 반영되지 않을 수 있다.
			inline static volatile LogLevel m_logLevel = LogLevel::LOG_INFO;
			inline static SRWLOCK m_srwLock = SRWLOCK_INIT;

			inline static unsigned int m_sinks = LOG_SINK_CONSOLE;
			inline static FILE* m_file = nullptr;

			static void WriteV(LogLevel level, const char* function, const char* format, va_list args);

		public:
			static LogLevel GetLogLevel();
			static void SetLogLevel(LogLevel level);

			// 레벨 게이트. 매크로에서 인자 평가 이전에 호출된다.
			// 반드시 인라인이어야 하며 여기서 걸리면 비용이 비교 한 번이다.
			static bool Enabled(LogLevel level) noexcept
			{
				return !(level < m_logLevel);
			}

			// 출력 대상 제어
			static void SetSinks(unsigned int sinks);
			static unsigned int GetSinks();

			// 파일 싱크. 성공하면 LOG_SINK_FILE 이 자동으로 켜진다.
			// 내부적으로 큰 버퍼를 사용하며 LOG_ERROR 이상에서만 flush 한다.
			static bool OpenFile(const char* filePath);
			static void CloseFile();
			static void Flush();

			// --- 신규 API ---
			// function 은 매크로가 __FUNCTION__ 을 자동 주입한다.
			// 출력 한 줄에 경과시간 / 스레드ID / 레벨 / 함수명이 자동으로 붙는다.
			static void Write(LogLevel level, const char* function, const char* format, ...);

			// --- 기존 API (호출부 736곳 호환 유지) ---
			// 내부적으로 신규 경로를 타므로 기존 호출부도 타임스탬프와 스레드ID 를 얻는다.
			static void Log(LogLevel level, const char* format, ...);
			static void Log(LogLevel level, const wchar_t* format, ...);
		};
	}
}

// ---------------------------------------------------------------------------
// 로그 매크로
//
// 함수 호출 대신 매크로를 쓰는 이유:
//   Logger::Log(LOG_INFO, "...", GetSessionID()) 형태는 레벨 체크가 함수 안에서
//   일어나므로, 로그가 꺼져 있어도 GetSessionID() 같은 인자가 먼저 평가된다.
//   (GetSessionID 는 가상 함수라 인라인도 제거도 되지 않는다)
//   매크로로 감싸면 꺼진 로그의 비용이 비교 한 번으로 줄어든다.
// ---------------------------------------------------------------------------

// 가변인자 전체를 __VA_ARGS__ 로 넘긴다.
// fmt 를 따로 받으면 인자 없는 호출 LOGI("hello") 에서 "fmt," 뒤에 빈
// __VA_ARGS__ 가 붙어 컴파일러 확장에 의존하게 된다.
#define CORE_LOG(level, ...)                                              \
	do {                                                                  \
		if (::Core::Util::Logger::Enabled(level))                         \
			::Core::Util::Logger::Write((level), __FUNCTION__, __VA_ARGS__); \
	} while (0)

// LOGT 는 패킷/할당 단위처럼 초당 수만~수백만 번 찍히는 흐름 추적용이다.
// CORE_ENABLE_TRACE_LOG 가 정의되지 않으면 코드 자체가 사라진다.
#ifdef CORE_ENABLE_TRACE_LOG
#define LOGT(...) CORE_LOG(LogLevel::LOG_DEBUG, __VA_ARGS__)
#else
#define LOGT(...) ((void)0)
#endif

#define LOGD(...) CORE_LOG(LogLevel::LOG_DEBUG,   __VA_ARGS__)
#define LOGI(...) CORE_LOG(LogLevel::LOG_INFO,    __VA_ARGS__)
#define LOGW(...) CORE_LOG(LogLevel::LOG_WARNING, __VA_ARGS__)
#define LOGE(...) CORE_LOG(LogLevel::LOG_ERROR,   __VA_ARGS__)
#define LOGC(...) CORE_LOG(LogLevel::LOG_FORCE,   __VA_ARGS__)
