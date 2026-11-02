#include "Logger.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for wcstombs_s

#include "../Sync/SRWLockGuard.h"

using Core::Util::Logger;
using Core::Sync::SRWWriteLockGuard;

namespace
{
	// 파일 싱크 버퍼. 크게 잡아서 write 횟수를 줄인다.
	constexpr size_t LOG_FILE_BUFFER_SIZE = 1u << 20; // 1MB
	char g_fileBuffer[LOG_FILE_BUFFER_SIZE];

	const char* ToLevelName(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::LOG_DEBUG:   return "DEBUG";
		case LogLevel::LOG_INFO:    return "INFO";
		case LogLevel::LOG_WARNING: return "WARN";
		case LogLevel::LOG_ERROR:   return "ERROR";
		case LogLevel::LOG_FORCE:   return "FORCE";
		case LogLevel::LOG_HIGH:    return "HIGH";
		default:                    return "?";
		}
	}

	// 로거 초기화 시점부터의 경과 시간(초).
	// 월클럭이 아니라 단조 증가 값을 쓰는 이유는, 경합 분석에서는
	// "무슨 일이 몇 마이크로초 차이로 벌어졌는가" 가 중요하기 때문이다.
	double ElapsedSeconds()
	{
		struct Clock
		{
			LARGE_INTEGER frequency{};
			LARGE_INTEGER start{};

			Clock()
			{
				::QueryPerformanceFrequency(&frequency);
				::QueryPerformanceCounter(&start);
				if (frequency.QuadPart == 0) frequency.QuadPart = 1;
			}
		};

		static const Clock clock; // C++11 이후 초기화가 스레드 안전하다

		LARGE_INTEGER now{};
		::QueryPerformanceCounter(&now);

		return static_cast<double>(now.QuadPart - clock.start.QuadPart)
			/ static_cast<double>(clock.frequency.QuadPart);
	}
}

LogLevel Logger::GetLogLevel()
{
	return m_logLevel;
}

void Logger::SetLogLevel(LogLevel level)
{
	m_logLevel = level;
}

void Logger::SetSinks(unsigned int sinks)
{
	SRWWriteLockGuard lock(m_srwLock);
	m_sinks = sinks;
}

unsigned int Logger::GetSinks()
{
	return m_sinks;
}

bool Logger::OpenFile(const char* filePath)
{
	if (!filePath) return false;

	SRWWriteLockGuard lock(m_srwLock);

	if (m_file)
	{
		::fclose(m_file);
		m_file = nullptr;
	}

	FILE* file = nullptr;
	if (::fopen_s(&file, filePath, "wb") != 0 || !file)
	{
		return false;
	}

	// 1MB 버퍼. 이걸 안 걸면 stdio 기본 버퍼(수 KB)라 write 호출이 급증한다.
	::setvbuf(file, g_fileBuffer, _IOFBF, LOG_FILE_BUFFER_SIZE);

	m_file = file;
	m_sinks |= Core::Util::LOG_SINK_FILE;

	return true;
}

void Logger::CloseFile()
{
	SRWWriteLockGuard lock(m_srwLock);

	if (m_file)
	{
		::fflush(m_file);
		::fclose(m_file);
		m_file = nullptr;
	}

	m_sinks &= ~static_cast<unsigned int>(Core::Util::LOG_SINK_FILE);
}

void Logger::Flush()
{
	SRWWriteLockGuard lock(m_srwLock);

	if (m_sinks & Core::Util::LOG_SINK_CONSOLE) ::fflush(stdout);
	if (m_file) ::fflush(m_file);
}

void Logger::WriteV(LogLevel level, const char* function, const char* format, va_list args)
{
	if (level < m_logLevel) return;

	// 포맷팅은 락 밖에서, 스택 버퍼에 한 번에 만든다.
	// 기존 구현은 vprintf_s 를 락 안에서 호출해서 락 보유 시간이 포맷 비용만큼
	// 길어졌고, printf 를 세 번 나눠 호출해서 스레드끼리 줄이 섞일 수 있었다.
	char line[MAX_LINE_LENGTH];
	int used = 0;

	const int prefix = ::_snprintf_s(line, sizeof(line), _TRUNCATE,
		"+%11.6f T#%-5lu %-5s %-34s | ",
		ElapsedSeconds(),
		::GetCurrentThreadId(),
		ToLevelName(level),
		function ? function : "-");

	used = (prefix > 0) ? prefix : 0;

	if (format && used < static_cast<int>(sizeof(line)) - 2)
	{
		const int body = ::_vsnprintf_s(line + used, sizeof(line) - used, _TRUNCATE, format, args);
		if (body > 0)
		{
			used += body;
		}
		else
		{
			// 잘렸다. 실제 길이를 다시 구하고 표시를 남긴다.
			used = static_cast<int>(::strnlen(line, sizeof(line)));
			if (used < static_cast<int>(sizeof(line)) - 2) line[used++] = '~';
		}
	}

	if (used > static_cast<int>(sizeof(line)) - 2) used = static_cast<int>(sizeof(line)) - 2;
	line[used++] = '\n';
	line[used] = '\0';

	// 락은 실제 쓰기만 감싼다. 한 번의 fwrite 이므로 줄이 섞이지 않는다.
	{
		SRWWriteLockGuard lock(m_srwLock);

		if (m_sinks & Core::Util::LOG_SINK_CONSOLE)
		{
			::fwrite(line, 1, static_cast<size_t>(used), stdout);
		}

		if (m_file)
		{
			::fwrite(line, 1, static_cast<size_t>(used), m_file);

			// 치명 로그는 크래시로 유실되면 안 되므로 즉시 내보낸다.
			// 그 외에는 1MB 버퍼가 찰 때까지 모은다.
			if (level >= LogLevel::LOG_ERROR)
			{
				::fflush(m_file);
			}
		}
	}
}

void Logger::Write(LogLevel level, const char* function, const char* format, ...)
{
	if (level < m_logLevel) return;

	va_list args;
	va_start(args, format);
	WriteV(level, function, format, args);
	va_end(args);
}

void Logger::Log(LogLevel level, const char* format, ...)
{
	if (level < m_logLevel) return;

	// 기존 호출부는 포맷 문자열 안에 __FUNCTION__ 을 직접 넣어 쓰므로
	// 함수명 칼럼은 비워 둔다.
	va_list args;
	va_start(args, format);
	WriteV(level, nullptr, format, args);
	va_end(args);
}

void Logger::Log(LogLevel level, const wchar_t* format, ...)
{
	if (level < m_logLevel) return;

	wchar_t wide[MAX_LINE_LENGTH];

	va_list args;
	va_start(args, format);
	const int written = ::_vsnwprintf_s(wide, MAX_LINE_LENGTH, _TRUNCATE, format, args);
	va_end(args);

	if (written == 0) return;

	// 좁은 문자열로 변환해서 동일한 출력 경로를 타게 한다.
	char narrow[MAX_LINE_LENGTH];
	size_t converted = 0;
	if (::wcstombs_s(&converted, narrow, sizeof(narrow), wide, _TRUNCATE) != 0)
	{
		return;
	}

	Logger::Write(level, nullptr, "%s", narrow);
}
