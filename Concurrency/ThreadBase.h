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
				, m_threadId(0)
			{
				if (m_stopEventHandle == nullptr)
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
			}

			ThreadBase(const ThreadBase&) = delete;
			ThreadBase& operator=(const ThreadBase&) = delete;

			bool Start()
			{
				Stop();

				if (m_stopEventHandle == nullptr)
				{
					return false;
				}

				::ResetEvent(m_stopEventHandle);
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

				self->Run();
				return 0;
			}

		private:
			HANDLE m_threadHandle;
			HANDLE m_stopEventHandle;
			uint32_t m_threadId;
			wchar_t m_threadName[64] = {};
		};
	}
}