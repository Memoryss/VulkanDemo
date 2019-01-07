#include "ApplicationBase.h"

#include <iostream>

ApplicationBase::ApplicationBase()
{

}

ApplicationBase::~ApplicationBase()
{

}

bool ApplicationBase::InitWindow(HINSTANCE hInstance, WNDPROC wndProc)
{
	m_instance = hInstance;

	// register window class
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = wndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = m_name.c_str();
	wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if (!RegisterClassEx(&wndClass))
	{
		std::cout << "Register Window Error!" << std::endl;
		exit(1);
	}

	RECT windowRect;
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = m_width;
	windowRect.bottom = m_height;

	DWORD windowExStyle;
	windowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	
	DWORD windowStyle;
	windowStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	// set window rect
	AdjustWindowRectEx(&windowRect, windowStyle, FALSE, windowExStyle);

	// create window hwnd
	m_hwnd = CreateWindowEx(0
		, DEFAULE_WINDOW_NAME.c_str()
		, m_name.c_str()
		, windowStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
		, 0
		, 0
		, windowRect.right - windowRect.left
		, windowRect.bottom - windowRect.top
		, NULL
		, NULL
		, hInstance
		, NULL
		);

	// screen center
	uint32_t x = (GetSystemMetrics(SM_CXSCREEN) - windowRect.right) / 2;
	uint32_t y = (GetSystemMetrics(SM_CXSCREEN) - windowRect.bottom) / 2;

	// set window pos
	SetWindowPos(m_hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	if (!m_hwnd)
	{
		std::cout << "Create Window Error" << std::endl;
		exit(1);
	}

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	return true;
}

void ApplicationBase::HandleMsgs(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		ValidateRect(hWnd, NULL);
		break;
	case WM_KEYDOWN:
		break;
	case WM_KEYUP:
		break;
	default:
		break;
	}
}

void ApplicationBase::Update()
{

}

void ApplicationBase::ResizeWindow(uint32_t width, uint32_t height)
{

}
