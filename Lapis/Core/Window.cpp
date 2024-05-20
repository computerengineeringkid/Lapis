#include "Window.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

Window::Window()
	:m_hInstance(GetModuleHandle(nullptr))
{
	const char* CLASS_NAME = "Renderer";

	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = WindowProc;

	RegisterClass(&wndClass);
	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	
	RECT rect;
	rect.left = 250;
	rect.top = 250;
	rect.right = rect.left + m_Width;
	rect.bottom = rect.top + m_Height;

	AdjustWindowRect(&rect, style, false);
	m_hWnd = CreateWindowExA(
		0,
		CLASS_NAME,
		CLASS_NAME,
		style,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		m_hInstance,
		NULL


	);
	ShowWindow(m_hWnd, SW_SHOW);
}

Window::~Window()
{
	const char* CLASS_NAME = "Renderer";
	UnregisterClass(CLASS_NAME, m_hInstance);;
}

bool Window::ProcessMessages()
{
	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}


