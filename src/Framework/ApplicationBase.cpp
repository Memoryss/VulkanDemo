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
	m_hInstance = hInstance;

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

bool ApplicationBase::InitVulkan()
{
	createVulkanInstance();
	
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

void ApplicationBase::createVulkanInstance()
{
	uint32_t extCnt = 0;
	VkResult result;
	// take first physical device;
	VkPhysicalDevice temp;
	vkEnumerateDeviceExtensionProperties(temp, nullptr, &extCnt, nullptr);

// 	uint32_t gpuCnt = 1;
// 	result = vkEnumeratePhysicalDevices(m_vkInstance, &gpuCnt, NULL);
// 	if (gpuCnt <= 0 || result != VK_SUCCESS)
// 	{
// 		std::cout << "Failed to find Gpu Device! res:" << result << std::endl;
// 		exit(1);
// 	}
// 
// 	m_vkPhyDevices.resize(gpuCnt);
// 	result = vkEnumeratePhysicalDevices(m_vkInstance, &gpuCnt, m_vkPhyDevices.data());
// 	if (gpuCnt <= 0 || result != VK_SUCCESS)
// 	{
// 		std::cout << "Failed to find Gpu Device! res:" << result << std::endl;
// 		exit(1);
// 	}

	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = m_name.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = m_name.c_str();
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = 0;
	appInfo.pNext = nullptr;

	VkInstanceCreateInfo instanceInfo;
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledExtensionCount = 0;

	result = vkCreateInstance(&instanceInfo, nullptr, &m_vkInstance);
	if (result != VK_SUCCESS)
	{
		std::cout << "Failed to create instance! res:" << result << std::endl;
		exit(1);
	}

	

	 extCnt = 0;
	// take first physical device;
	vkEnumerateDeviceExtensionProperties(m_vkPhyDevices[0], nullptr, &extCnt, nullptr);

	std::vector<VkExtensionProperties> extProperties(extCnt);
	vkEnumerateDeviceExtensionProperties(m_vkPhyDevices[0], nullptr, &extCnt, extProperties.data());

	std::cout << "Support Properties:";
	for (const auto &extension : extProperties)
	{
		std::cout << extension.extensionName << std::endl;
	}
}

void ApplicationBase::destoryVulkanInstance()
{
	vkDestroyInstance(m_vkInstance, nullptr);
}

void ApplicationBase::createLogicDevice()
{
	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = m_vkQueueFamilyIndex;
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.f;
	queueCreateInfo.pQueuePriorities = &queuePriority;
	
	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceCreateInfo.pEnabledFeatures = VK_FALSE;
	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	deviceCreateInfo.enabledExtensionCount = 0;
	deviceCreateInfo.enabledLayerCount = 0;

	VkResult result = vkCreateDevice(m_vkPhyDevices[0], &deviceCreateInfo, nullptr, &m_vkDevice);
	if (result != VK_SUCCESS)
	{
		std::cout << "create device faild, result:" << result << std::endl;
		exit(1);
	}

	vkGetDeviceQueue(m_vkDevice, m_vkQueueFamilyIndex, 0, &m_vkQueue);
}

void ApplicationBase::findQueueFamilies()
{
	uint32_t queueFamilyCnt = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhyDevices[0], &queueFamilyCnt, nullptr);
	std::vector<VkQueueFamilyProperties> vkQueueFamilies(queueFamilyCnt);
	vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhyDevices[0], &queueFamilyCnt, vkQueueFamilies.data());

	uint32_t i = 0;
	for (auto &queueFamily : vkQueueFamilies)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags &VK_QUEUE_GRAPHICS_BIT)
		{
			m_vkQueueFamilyIndex = i;
			if (m_vkQueueFamilyIndex >= 0)
			{
				break;
			}
		}
		++i;
	}
}

void ApplicationBase::createSurface()
{
	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.hinstance = m_hInstance;
	createInfo.hwnd = m_hwnd;
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;

	VkResult result = vkCreateWin32SurfaceKHR(m_vkInstance, &createInfo, NULL, &m_vkSurface);
	if (result != VK_SUCCESS)
	{
		std::cout << "createSuface fail, result:" << result << std::endl;
		exit(1);
	}
}
