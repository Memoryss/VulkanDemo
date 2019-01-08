#ifndef __APPLICATION_BASE_H__
#define __APPLICATION_BASE_H__

// !windows only
#include <windows.h>

#include <vulkan/vulkan.hpp>

#include "CommonDefs.h"

class ApplicationBase
{
public:
	ApplicationBase();

	virtual ~ApplicationBase();

	bool InitWindow(HINSTANCE hInstance, WNDPROC wndProc);

	bool InitVulkan();

	void HandleMsgs(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Update();

	// resize windows size
	void ResizeWindow(uint32_t width, uint32_t height);

private:
	void createVulkanInstance();

	void destoryVulkanInstance();

private:
	// window instance only in Windows! 
	HINSTANCE m_hInstance{};
	HWND m_hwnd{};

	// window param
	uint32_t m_width{ DEFAULE_WINDOW_WIDTH };
	uint32_t m_height{ DEFAULE_WINDOW_HEIGHT };
	std::string m_name{ DEFAULE_WINDOW_NAME };

	// vulkan
	VkInstance m_vkInstance;
};

#define VULKAN_DEMO_MAIN()															\
VulkanApplication *vulkanDemo = nullptr;											\
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)		\
{																					\
	if (vulkanDemo != nullptr)														\
	{																				\
		vulkanDemo->HandleMsgs(hWnd, uMsg, wParam, lParam);							\
	}																				\
																					\
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));								\
}																					\
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)					\
{																					\
	vulkanDemo = new VulkanApplication();											\
	vulkanDemo->InitWindow(hInstance, WndProc);										\
	return 0;																		\
}																					
#endif