#include "Logger.h"

#include <stdio.h>

#include "../Sync/SRWLockGuard.h"

using Core::Util::Logger;
using CoreLibrary::Sync::SRWWriteLockGuard;

LogLevel Logger::GetLogLevel()
{
	return m_logLevel;
}

void Logger::SetLogLevel(LogLevel level)
{
	m_logLevel = level;
}

void Logger::Log(LogLevel level, const char* format, ...)
{
	if (level < m_logLevel)
	{
		return;
	}

	SRWWriteLockGuard lock(m_srwLock);

	const char* levelString = nullptr;
	switch (level)
	{
	case LogLevel::LOG_DEBUG:
		levelString = "[DEBUG]";
		break;
	case LogLevel::LOG_INFO:
		levelString = "[INFO]";
		break;
	case LogLevel::LOG_WARNING:
		levelString = "[WARNING]";
		break;
	case LogLevel::LOG_ERROR:
		levelString = "[ERROR]";
		break;
	default:
		levelString = "[UNKNOWN]";
		break;
	}

	printf_s("%s ", levelString);

	va_list args;
	va_start(args, format);
	vprintf_s(format, args);
	va_end(args);

	printf_s("\n");
}

void Logger::Log(LogLevel level, const wchar_t* format, ...)
{
	if (level < m_logLevel)
	{
		return;
	}

	SRWWriteLockGuard lock(m_srwLock);

	const wchar_t* levelString = nullptr;
	switch (level)
	{
	case LogLevel::LOG_DEBUG:
		levelString = L"[DEBUG]";
		break;
	case LogLevel::LOG_INFO:
		levelString = L"[INFO]";
		break;
	case LogLevel::LOG_WARNING:
		levelString = L"[WARNING]";
		break;
	case LogLevel::LOG_ERROR:
		levelString = L"[ERROR]";
		break;
	case LogLevel::LOG_FORCE:
		levelString = L"[ADMIN]";
		break;
	}

	wprintf_s(L"%s ", levelString);

	va_list args;
	va_start(args, format);
	vfwprintf_s(stdout, format, args);
	va_end(args);

	printf_s("\n");
}
