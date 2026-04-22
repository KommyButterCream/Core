#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <functional>
#include <memory>

namespace Core
{
	namespace Concurrency
	{
		class ThreadTimer
		{
		public:
			using TimerCallback = std::function<void()>;

			ThreadTimer() = default;
			~ThreadTimer()
			{
				Stop();
			}

			bool Start(TimerCallback callback, uint32_t dueTime_ms, uint32_t period_ms = 0)
			{
				Stop();

				m_callback = std::make_unique<TimerCallback>(std::move(callback));

				m_timer = ::CreateThreadpoolTimer(InternalCallback, m_callback.get(), nullptr);
				if (!m_timer)
				{
					return false;
				}

				ULARGE_INTEGER dueTime = {};
				dueTime.QuadPart = (ULONGLONG)-(static_cast<long long>(dueTime_ms) * 10000LL);

				FILETIME dueFileTime = {};
				dueFileTime.dwHighDateTime = dueTime.HighPart;
				dueFileTime.dwLowDateTime = dueTime.LowPart;

				::SetThreadpoolTimer(m_timer, &dueFileTime, period_ms, 0);
				return true;
			}

			void Stop()
			{
				if (m_timer)
				{
					::SetThreadpoolTimer(m_timer, nullptr, 0, 0);
					::WaitForThreadpoolTimerCallbacks(m_timer, TRUE);
					::CloseThreadpoolTimer(m_timer);
					m_timer = nullptr;
				}
			}

		private:
			static VOID CALLBACK InternalCallback(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_TIMER timer)
			{
				UNREFERENCED_PARAMETER(instance);
				UNREFERENCED_PARAMETER(timer);

				auto* callback = static_cast<TimerCallback*>(context);
				if (callback && *callback)
				{
					(*callback)();
				}
			}

			PTP_TIMER m_timer = nullptr;
			std::unique_ptr<TimerCallback> m_callback = nullptr;
		};
	}
}
