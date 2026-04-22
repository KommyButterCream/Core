#include "WindowBase.h"

using Core::Window::WindowBase;

WindowBase::WindowBase()
{
}

WindowBase::~WindowBase()
{
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		::DestroyWindow(m_hWnd);
		m_hWnd = nullptr;
	}
}

bool WindowBase::Register(LPCWSTR className, HINSTANCE hInstance, UINT classStyle)
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.style = classStyle;
	wc.lpfnWndProc = WindowBase::WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = className;
	wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = nullptr;

	return ::RegisterClassEx(&wc) || GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

bool WindowBase::Create(
	DWORD exStyle,
	LPCWSTR className,
	LPCWSTR WindowBaseName,
	DWORD style,
	UINT classStyle,
	const RECT& rc,
	HWND parent,
	HINSTANCE hInstance)
{
	if (parent && !::IsWindow(parent))
		return false;

	// Register Class
	if (!hInstance)
		hInstance = ::GetModuleHandle(nullptr);

	bool bRegister = Register(className, hInstance, classStyle);

	m_hWnd = ::CreateWindowEx(
		exStyle,
		className,
		WindowBaseName,
		style,
		rc.left, rc.top,
		rc.right - rc.left,
		rc.bottom - rc.top,
		parent,
		nullptr,
		hInstance,
		this);

	if (m_hWnd == nullptr)
	{
		DWORD error = ::GetLastError();
		__debugbreak();
	}
	return m_hWnd != nullptr;
}

LRESULT WindowBase::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(m_hWnd, message, wParam, lParam);
}

LRESULT WindowBase::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WindowBase* impl = nullptr;

	if (message == WM_NCCREATE)
	{
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		impl = reinterpret_cast<WindowBase*>(cs->lpCreateParams);
		impl->m_hWnd = hWnd;
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)impl);
	}
	else
	{
		impl = reinterpret_cast<WindowBase*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	return impl ? impl->WndProc(message, wParam, lParam) : ::DefWindowProc(hWnd, message, wParam, lParam);
}
