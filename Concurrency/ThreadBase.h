#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <stdint.h>
#include <process.h> // for _beginthread, _endthread
#include <wchar.h> // for wcsncpy_s

namespace Core
{
	namespace Concurrency
	{
		class ThreadBase
		{
		public:
			explicit ThreadBase(const wchar_t* name = L"")
				: m_threadHandle(nullptr)
				, m_stopEventHandle(::CreateEventW(nullptr, TRUE, FALSE, nullptr))
				, m_startedEventHandle(::CreateEventW(nullptr, TRUE, FALSE, nullptr))
				, m_threadId(0)
			{
				if (m_stopEventHandle == nullptr || m_startedEventHandle == nullptr)
				{
					__debugbreak();
				}

				if (name != nullptr)
				{
					::wcsncpy_s(m_threadName, name, _TRUNCATE);
				}
				else
				{
					m_threadName[0] = L'\0';
				}
			}

			virtual ~ThreadBase()
			{
				Stop();

				if (m_stopEventHandle)
				{
					::CloseHandle(m_stopEventHandle);
					m_stopEventHandle = nullptr;
				}

				if (m_startedEventHandle)
				{
					::CloseHandle(m_startedEventHandle);
					m_startedEventHandle = nullptr;
				}
			}

			ThreadBase(const ThreadBase&) = delete;
			ThreadBase& operator=(const ThreadBase&) = delete;

			// 스레드를 띄우고, 그 스레드가 진입점에 도달할 때까지 기다린 뒤 돌아온다.
			//
			// 기다리는 이유 (순수 가상 호출 방지)
			//   _beginthreadex 는 스레드를 만들자마자 돌아온다. 새 스레드가
			//   ThreadProc 에서 self->Run() 에 닿기까지는 시간이 걸리는데,
			//   그 사이에 객체가 소멸을 시작하면 ~ThreadBase 가 vptr 을 기반
			//   클래스로 내려놓는다. 그 뒤에 스레드가 Run() 을 부르면 Run 은
			//   순수 가상이므로 purecall -> 프로세스가 즉사한다(0xC0000409).
			//
			//   기다려 두면 "Start 가 돌아왔다 = 스레드가 이미 진입점을 지났다"
			//   가 성립하므로, 소멸이 스레드 기동을 앞지를 수 없다.
			//
			//   실측: StartServer 가 포트 충돌로 실패하면 객체가 수 밀리초 만에
			//   소멸한다. 워커 32개를 띄우자마자 지우는 셈이라 이 창이 그대로
			//   열렸고, 200회 반복에서 5회 안에 죽었다. 이 대기를 넣은 뒤
			//   1,000회 반복 x 5회 실행에서 0건.
			bool Start()
			{
				Stop();

				if (m_stopEventHandle == nullptr || m_startedEventHandle == nullptr)
				{
					return false;
				}

				::ResetEvent(m_stopEventHandle);
				::ResetEvent(m_startedEventHandle);
				m_threadId = 0;

				HANDLE threadHandle = reinterpret_cast<HANDLE>(_beginthreadex(
					nullptr,
					0,
					&ThreadBase::ThreadProc,
					this,
					0,
					&m_threadId));

				if (threadHandle == nullptr)
				{
					return false;
				}

				m_threadHandle = threadHandle;

				// 진입 신호를 기다린다. ThreadProc 이 무조건 세우므로 걸리지 않는다.
				::WaitForSingleObject(m_startedEventHandle, INFINITE);

				return true;
			}

			void Stop()
			{
				RequestStop();
				Join();
			}

			void RequestStop()
			{
				if (m_stopEventHandle)
				{
					::SetEvent(m_stopEventHandle);
				}
			}

			void Join()
			{
				if (!m_threadHandle)
				{
					return;
				}

				const DWORD waitResult = ::WaitForSingleObject(m_threadHandle, INFINITE);
				if (waitResult != WAIT_OBJECT_0)
				{
					__debugbreak();
				}

				::CloseHandle(m_threadHandle);
				m_threadHandle = nullptr;
				m_threadId = 0;
			}

			bool IsRunning() const
			{
				if (!m_threadHandle)
				{
					return false;
				}

				const DWORD waitResult = ::WaitForSingleObject(m_threadHandle, 0);
				return (waitResult == WAIT_TIMEOUT);
			}

			bool IsStopRequested() const
			{
				if (!m_stopEventHandle)
				{
					return true;
				}

				return (::WaitForSingleObject(m_stopEventHandle, 0) == WAIT_OBJECT_0);
			}

			HANDLE GetStopEvent() const
			{
				return m_stopEventHandle;
			}

			uint32_t GetThreadId() const
			{
				return m_threadId;
			}

		protected:
			virtual void Run() = 0;

		private:
			static unsigned int __stdcall ThreadProc(void* arg)
			{
				ThreadBase* self = reinterpret_cast<ThreadBase*>(arg);
				if (!self)
				{
					return 0;
				}

				if (self->m_threadName[0] != L'\0')
				{
					::SetThreadDescription(::GetCurrentThread(), self->m_threadName);
				}

				// 진입을 알린다. Start() 가 이 신호를 받고 나서야 돌아간다.
				if (self->m_startedEventHandle)
				{
					::SetEvent(self->m_startedEventHandle);
				}

				// 이미 정지 요청이 서 있으면 Run 에 들어가지 않는다.
				//
				// 위 대기만으로 거의 다 막히지만 완전하지는 않다. 소멸자는
				// vptr 을 내린 다음 RequestStop -> Join 순으로 움직이므로,
				// "vptr 은 내려갔는데 아직 RequestStop 전" 인 찰나가 이론상
				// 남는다. 이 검사가 그 폭을 한 번 더 줄인다.
				if (self->IsStopRequested())
				{
					return 0;
				}

				self->Run();
				return 0;
			}

		private:
			HANDLE m_threadHandle;
			HANDLE m_stopEventHandle;

			// 스레드가 ThreadProc 진입점에 도달했음을 알리는 신호.
			// Start() 가 이걸 기다린 뒤에 돌아간다. (이유는 Start 주석 참고)
			HANDLE m_startedEventHandle;

			uint32_t m_threadId;
			wchar_t m_threadName[64] = {};
		};
	}
}