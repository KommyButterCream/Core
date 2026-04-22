#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

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
		class Logger
		{
		public:
			Logger() = default;
			~Logger() = default;

		private:
			inline static LogLevel m_logLevel = LogLevel::LOG_INFO;
			inline static SRWLOCK m_srwLock = SRWLOCK_INIT;

		public:
			static LogLevel GetLogLevel();

			static void SetLogLevel(LogLevel level);

			static void Log(LogLevel level, const char* format, ...);
			static void Log(LogLevel level, const wchar_t* format, ...);
		};
	}
}
