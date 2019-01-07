#ifndef __APPLICATION_BASE_H__
#define __APPLICATION_BASE_H__

// !windows only
#include <windows.h>

#include "CommonDefs.h"

class ApplicationBase
{
public:
	ApplicationBase();

	virtual ~ApplicationBase();

	bool InitWindow(HINSTANCE hInstance, WNDPROC wndProc);

	// resize windows size
	void ResizeWindow(uint32_t width, uint32_t height);

private:
	// window instance only in Windows! 
	HINSTANCE m_instance{};
	HWND m_hwnd{};

	// window param
	uint32_t m_width{ DEFAULE_WINDOW_WIDTH };
	uint32_t m_height{ DEFAULE_WINDOW_HEIGHT };
	std::string m_name{ DEFAULE_WINDOW_NAME };
}



#endif