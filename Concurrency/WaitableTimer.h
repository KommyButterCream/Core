#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <stdint.h>

// Win10 1803 이전 SDK 헤더에는 없다. 값은 winbase.h 정의와 동일.
#ifndef CREATE_WAITABLE_TIMER_HIGH_RESOLUTION
#define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002
#endif

namespace Core
{
	namespace Concurrency
	{
		// 커널 대기 타이머(waitable timer) RAII 래퍼.
		//
		// [왜 Sleep 이 아닌가]
		// Sleep 계열은 시스템 타이머 틱(기본 약 15.6ms)에 묶인다. 30fps(33.3ms)
		// 를 노려도 31.2ms 또는 46.8ms 로 튄다. 남는 시간을 스핀으로 메우면
		// 정밀도는 얻지만 코어를 태운다.
		// 대기 타이머를 CREATE_WAITABLE_TIMER_HIGH_RESOLUTION (Win10 1803+) 으로
		// 만들면 틱과 무관하게 0.5ms 수준 정밀도를 주면서도 CPU 를 쓰지 않는다.
		//
		// [자동 리셋(동기화 타이머)]
		// 생성 플래그에서 CREATE_WAITABLE_TIMER_MANUAL_RESET 을 빼면 자동 리셋이
		// 된다. 대기 한 번이 신호 하나를 소비하므로 주기 타이머의 페이싱이 그대로
		// 유지된다. 수동 리셋으로 만들면 한 번 신호된 뒤 계속 신호 상태로 남아
		// 이후 대기가 전부 즉시 통과해 버린다.
		//
		// [ThreadTimer 와의 차이]
		// ThreadTimer 는 스레드풀이 콜백을 불러주는 방식이다. 이 클래스는 호출한
		// 스레드가 직접 기다린다. 프레임 페이싱처럼 내 루프를 붙잡아 두는 용도.
		//
		// [스레드 안전성]
		// 핸들 자체는 커널 오브젝트라 여러 스레드가 같이 기다려도 되지만,
		// SetXxx / Cancel / Close 를 여러 스레드에서 섞어 부르는 것은 막지 않는다.
		// 한 스레드가 소유하고 그 스레드에서만 조작하는 사용을 전제로 한다.
		class WaitableTimer
		{
		public:
			enum class WaitResult
			{
				Signaled, // 타이머 만료
				Stopped,  // 같이 기다린 stop 이벤트가 신호됨
				Failed,   // 타이머가 없거나 대기 실패
			};

			WaitableTimer()
				: m_handle(::CreateWaitableTimerExW(
					nullptr, nullptr, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS))
				, m_isHighResolution(m_handle != nullptr)
			{
				if (m_handle == nullptr)
				{
					// 고해상도 플래그를 지원하지 않는 환경(Win10 1803 미만).
					// 일반 타이머는 시스템 타이머 틱에 묶이므로, 호출부가
					// IsHighResolution() 을 보고 timeBeginPeriod 같은 보정을
					// 걸지 판단한다.
					m_handle = ::CreateWaitableTimerExW(nullptr, nullptr, 0, TIMER_ALL_ACCESS);
				}
			}

			~WaitableTimer()
			{
				Close();
			}

			WaitableTimer(const WaitableTimer&) = delete;
			WaitableTimer& operator=(const WaitableTimer&) = delete;

			WaitableTimer(WaitableTimer&& other) noexcept
				: m_handle(other.m_handle)
				, m_isHighResolution(other.m_isHighResolution)
			{
				other.m_handle = nullptr;
				other.m_isHighResolution = false;
			}

			WaitableTimer& operator=(WaitableTimer&& other) noexcept
			{
				if (this != &other)
				{
					Close();

					m_handle = other.m_handle;
					m_isHighResolution = other.m_isHighResolution;

					other.m_handle = nullptr;
					other.m_isHighResolution = false;
				}

				return *this;
			}

			bool IsValid() const
			{
				return m_handle != nullptr;
			}

			// false 면 폴백(일반 타이머)으로 내려간 것이다. 이 경우 대기 정밀도가
			// 시스템 타이머 틱에 묶이므로, 필요하면 호출부가 timeBeginPeriod 로
			// 보정한다. 그 설정은 프로세스 전역이라 이 클래스가 대신 걸지 않는다.
			bool IsHighResolution() const
			{
				return m_isHighResolution;
			}

			// WaitForMultipleObjects 에 다른 핸들과 같이 넣고 싶을 때 쓴다.
			// 단순히 stop 이벤트와 함께 기다리는 것이라면 Wait() 로 충분하다.
			HANDLE GetHandle() const
			{
				return m_handle;
			}

			// 지금부터 delay_100ns (100ns 단위) 뒤에 한 번 신호한다.
			// 0 이하는 가능한 한 빨리로 보고 최소값으로 내린다.
			bool SignalAfter100ns(int64_t delay_100ns)
			{
				if (m_handle == nullptr)
					return false;

				if (delay_100ns <= 0)
					delay_100ns = 1;

				// 음수 = 상대 시각(100ns 단위). 양수면 절대 시각(FILETIME)이 된다.
				LARGE_INTEGER dueTime = {};
				dueTime.QuadPart = -delay_100ns;

				return ::SetWaitableTimer(m_handle, &dueTime, 0, nullptr, nullptr, FALSE) != FALSE;
			}

			// 지금부터 delay_ms 뒤에 한 번만 신호한다.
			// 계속 반복하려면 SignalEvery 를 쓴다.
			bool SignalAfter(double delay_ms)
			{
				return SignalAfter100ns(static_cast<int64_t>(delay_ms * 10000.0));
			}

			// firstDelay_ms 뒤 첫 신호, 그 뒤 period_ms 마다 반복한다.
			// firstDelay_ms 를 0 으로 주면 사실상 즉시 첫 신호가 난다.
			//
			// 신호 시각은 첫 신호를 원점으로 한 격자(원점 + k * period)에 고정된다.
			// 처리가 주기를 넘겨 늦어도 다음 신호는 격자 위에 그대로 있으므로
			// 드리프트가 누적되지 않는다. 매번 Sleep 으로 다시 재는 방식은
			// 늦은 만큼이 그대로 뒤로 밀린다.
			//
			// 다만 놓친 주기가 개수만큼 쌓이지는 않는다. 자동 리셋 타이머는
			// 신호 상태가 하나뿐이라, 주기를 두 번 놓쳐도 대기 한 번이 즉시
			// 통과할 뿐 두 번 통과하지 않는다. 밀린 만큼 일감이 몰리지 않는다.
			bool SignalEvery(double firstDelay_ms, uint32_t period_ms)
			{
				if (m_handle == nullptr)
					return false;

				// SetWaitableTimer 는 lPeriod 0 을 one-shot 으로 해석한다.
				// 주기 요청인데 0 이 들어오면 최소 주기 1ms 로 올린다.
				if (period_ms == 0U)
					period_ms = 1U;

				int64_t firstDelay_100ns = static_cast<int64_t>(firstDelay_ms * 10000.0);
				if (firstDelay_100ns <= 0)
					firstDelay_100ns = 1;

				LARGE_INTEGER dueTime = {};
				dueTime.QuadPart = -firstDelay_100ns;

				return ::SetWaitableTimer(
					m_handle, &dueTime, static_cast<LONG>(period_ms),
					nullptr, nullptr, FALSE) != FALSE;
			}

			// 걸어둔 타이머만 해제한다. 핸들은 살아 있으므로 다시 걸 수 있다.
			void Cancel()
			{
				if (m_handle)
				{
					::CancelWaitableTimer(m_handle);
				}
			}

			// 타이머를 완전히 포기한다. 이후 IsValid() 는 false.
			// 타이머 없이도 계속 돌아야 하는 폴백 경로에서 쓴다.
			void Close()
			{
				if (m_handle)
				{
					::CancelWaitableTimer(m_handle);
					::CloseHandle(m_handle);
					m_handle = nullptr;
				}

				m_isHighResolution = false;
			}

			// stopEvent 를 같이 기다린다. 정지 요청이 오면 남은 대기 시간을
			// 버리고 즉시 깨어난다. 둘 다 신호된 상태라면 stopEvent 가 이긴다.
			// WaitForMultipleObjects 가 가장 낮은 인덱스를 돌려주기 때문이다.
			WaitResult Wait(HANDLE stopEvent = nullptr) const
			{
				if (m_handle == nullptr)
					return WaitResult::Failed;

				if (stopEvent == nullptr)
				{
					return (::WaitForSingleObject(m_handle, INFINITE) == WAIT_OBJECT_0)
						? WaitResult::Signaled
						: WaitResult::Failed;
				}

				const HANDLE waitHandles[2] = { stopEvent, m_handle };
				const DWORD waitResult = ::WaitForMultipleObjects(2, waitHandles, FALSE, INFINITE);

				if (waitResult == WAIT_OBJECT_0)
					return WaitResult::Stopped;

				if (waitResult == WAIT_OBJECT_0 + 1)
					return WaitResult::Signaled;

				return WaitResult::Failed;
			}

		private:
			HANDLE m_handle = nullptr;
			bool m_isHighResolution = false;
		};
	}
}
