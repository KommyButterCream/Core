#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <atomic> // for std::atomic, std::atomic_flag, std::memory_order_acquire, std::memory_order_release
#include <cstdint> // for uint32_t, uint64_t
#include <functional> // for std::function
#include <mutex> // for std::mutex, std::lock_guard
#include <utility> // for std::move, std::exchange

namespace Core::Concurrency
{
    // 제어 호출은 직렬화하며, 같은 타이머의 콜백은 중첩 실행하지 않는다.
    // 객체 파괴는 콜백 밖에서 수행하고 다른 제어 호출과 겹치지 않게 한다.
    class ThreadTimer final
    {
    public:
        using TimerCallback = std::function<void()>;
        ThreadTimer() = default;
        ~ThreadTimer() { Stop(); }
        ThreadTimer(const ThreadTimer&) = delete;
        ThreadTimer& operator=(const ThreadTimer&) = delete;

        // 기존 실행을 정리하고 새 콜백을 등록한다. periodMs가 0이면 단발이다.
        // 콜백 안에서 제어 함수를 호출하면 자기 자신 또는 다른 콜백의 대기를 만들 수 있어 거절한다.
        [[nodiscard]] bool Start(TimerCallback callback, uint32_t dueTimeMs, uint32_t periodMs = 0)
        {
            if (IsCallbackThread() || !callback) return false;
            std::lock_guard lock(m_controlMutex);
            StopLocked();
            m_callback = std::move(callback);
            m_timer = ::CreateThreadpoolTimer(InternalCallback, this, nullptr);
            if (!m_timer)
            {
                m_callback = {};
                return false;
            }
            m_failed.store(false);
            m_skippedCount.store(0);
            m_stopRequested.store(false);
            ULARGE_INTEGER due{};
            due.QuadPart = static_cast<ULONGLONG>(-static_cast<LONGLONG>(dueTimeMs) * 10'000);
            FILETIME time{ due.LowPart, due.HighPart };
            ::SetThreadpoolTimer(m_timer, &time, periodMs, 0);
            return true;
        }

        // 새 실행을 차단하고 예약을 취소한다. 이미 실행 중인 콜백은 Stop에서 기다린다.
        bool RequestStop()
        {
            if (IsCallbackThread()) return false;
            std::lock_guard lock(m_controlMutex);
            RequestStopLocked();
            return true;
        }

        // 대기 중인 콜백을 취소하고 실행 중인 콜백이 끝난 뒤 캡처 자원까지 해제한다.
        bool Stop()
        {
            if (IsCallbackThread()) return false;
            std::lock_guard lock(m_controlMutex);
            StopLocked();
            return true;
        }

        [[nodiscard]] bool HasFailed() const noexcept { return m_failed.load(); }
        [[nodiscard]] uint64_t GetSkippedCount() const noexcept { return m_skippedCount.load(); }
        [[nodiscard]] static bool IsCallbackThread() noexcept { return s_currentTimer != nullptr; }

    private:
        void RequestStopLocked()
        {
            m_stopRequested.store(true);
            if (m_timer) ::SetThreadpoolTimer(m_timer, nullptr, 0, 0);
        }
        void StopLocked()
        {
            RequestStopLocked();
            if (m_timer)
            {
                ::WaitForThreadpoolTimerCallbacks(m_timer, TRUE);
                ::CloseThreadpoolTimer(m_timer);
                m_timer = nullptr;
            }
            m_callback = {};
        }
        static VOID CALLBACK InternalCallback(PTP_CALLBACK_INSTANCE, PVOID context, PTP_TIMER timer) noexcept
        {
            auto& self = *static_cast<ThreadTimer*>(context);
            if (self.m_stopRequested.load()) return;
            if (self.m_executing.test_and_set(std::memory_order_acquire))
            {
                self.m_skippedCount.fetch_add(1);
                return;
            }
            struct Guard
            {
                ThreadTimer& timer;
                ThreadTimer* previous;
                ~Guard()
                {
                    s_currentTimer = previous;
                    timer.m_executing.clear(std::memory_order_release);
                }
            } guard{self, std::exchange(s_currentTimer, &self)};
            if (self.m_stopRequested.load()) return;
            try
            {
                self.m_callback();
            }
            catch (...)
            {
                // 예외를 Windows 콜백 경계 밖으로 전달하지 않는다. 실패한 타이머는 다시 시작하기 전까지 중단한다.
                self.m_failed.store(true);
                self.m_stopRequested.store(true);
                ::SetThreadpoolTimer(timer, nullptr, 0, 0);
            }
        }

        std::mutex m_controlMutex;
        PTP_TIMER m_timer = nullptr;
        TimerCallback m_callback;
        std::atomic<bool> m_stopRequested{true};
        std::atomic<bool> m_failed{false};
        std::atomic_flag m_executing = ATOMIC_FLAG_INIT;
        std::atomic<uint64_t> m_skippedCount{0};
        inline static thread_local ThreadTimer* s_currentTimer = nullptr;
    };
}
