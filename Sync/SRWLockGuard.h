#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

namespace CoreLibrary
{
	namespace Sync
	{
		class SRWWriteLockGuard
		{
		public:
			inline explicit SRWWriteLockGuard(SRWLOCK& lock)
				: m_lock(lock)
			{
				::AcquireSRWLockExclusive(&m_lock);
			}

			inline ~SRWWriteLockGuard() noexcept
			{
				::ReleaseSRWLockExclusive(&m_lock);
			}

		private:
			SRWWriteLockGuard(const SRWWriteLockGuard& rhs) = delete;
			SRWWriteLockGuard& operator = (const SRWWriteLockGuard& rhs) = delete;

		private:
			SRWLOCK& m_lock;
		};

		class SRWReadLockGuard
		{
		public:
			inline explicit SRWReadLockGuard(SRWLOCK& lock)
				: m_lock(lock)
			{
				::AcquireSRWLockShared(&m_lock);
			}

			inline ~SRWReadLockGuard() noexcept
			{
				::ReleaseSRWLockShared(&m_lock);
			}


		private:
			SRWReadLockGuard(const SRWReadLockGuard& rhs) = delete;
			SRWReadLockGuard& operator = (const SRWReadLockGuard& rhs) = delete;

		private:
			SRWLOCK& m_lock;
		};
	}
}