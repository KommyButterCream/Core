#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h>

/**
 * @brief 윈도우 생성 및 등록을 위한 베이스 클래스
 */
namespace Core
{
	namespace Window
	{
		class WindowBase
		{
		public:
			WindowBase();
			virtual ~WindowBase();

			inline HWND GetHWND() const { return m_hWnd; }
			inline bool IsValid() const { return (m_hWnd && ::IsWindow(m_hWnd)); }

		protected:
			bool Register(LPCWSTR className, HINSTANCE hInstance, UINT classStyle = CS_HREDRAW | CS_VREDRAW | CS_OWNDC);

			bool Create(
				DWORD exStyle,
				LPCWSTR className,
				LPCWSTR windowName,
				DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				UINT classStyle = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
				const RECT& rc = { 0, 0, 800, 600 },
				HWND parent = nullptr,
				HINSTANCE hInstance = nullptr);

			virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);

			static LRESULT CALLBACK WndProc(
				HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		protected:
			HWND m_hWnd = nullptr;
		};
	}
}

