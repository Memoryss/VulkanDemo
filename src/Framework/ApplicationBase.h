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

	// enbale/disable Validation Layer
	void EnableValidationLayer(bool enable);

private:
	void createVulkanInstance();

	void createVulkanPhysicalDevice();

	void createVulkanExtProperties();

	void destoryVulkanInstance();

	bool checkValidationLayerSupport();

	virtual void addExtProperties(const char *propertyName);

	virtual void setPhysicalDevice(uint8_t index);

private:
	// window instance only in Windows! 
	HINSTANCE m_hInstance{};
	HWND m_hwnd{};

	// window param
	uint32_t m_width{ DEFAULE_WINDOW_WIDTH };
	uint32_t m_height{ DEFAULE_WINDOW_HEIGHT };
	std::string m_name{ DEFAULE_WINDOW_NAME };

	// vulkan
	VkInstance m_vkInstance{ VK_NULL_HANDLE };  // every process only have one
	std::vector<VkPhysicalDevice> m_vkPhyDevices; // gpu devices
	uint8_t m_vkPhyDeviceIndex{ 0 };	// gpu devices index;
	const std::vector<const char *> m_vkValidationLayers{
		"VK_LAYER_LUNARG_standard_validation"
	};
	bool m_vkEnableValidationLayer{ false };

	std::vector<const char *> m_vkEnabledExtProperties;
	std::vector<VkExtensionProperties> m_vkExtProperties;
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
	vulkanDemo->InitVulkan();														\
	vulkanDemo->InitWindow(hInstance, WndProc);										\
	return 0;																		\
}																					
#endif