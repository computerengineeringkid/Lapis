#pragma once
#include <Windows.h>


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window
{
public:
	Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	~Window();

	bool ProcessMessages();

	/*void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);*/
	HINSTANCE& GetWindowInstance() { return m_hInstance; }
	HWND& GetWindow() { return m_hWnd; }
	int GetWindowWidth() { return m_Width; }
	int GetWindowHeight() { return m_Height; }
	

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	int m_Width = 800, m_Height = 600;
	friend class Input;
};

