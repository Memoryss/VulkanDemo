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
#ifdef _DEBUG
	m_vkEnableValidationLayer = true;
#endif

	createVulkanInstance();
	createVulkanPhysicalDevice();
	createVulkanExtProperties();
	
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
	// check validation layer
	if (m_vkEnableValidationLayer && !checkValidationLayerSupport()) {
		std::cout << "Failed to Support Validation Layer" << std::endl;
		exit(1);
	}

	VkResult result;
	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = m_name.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = m_name.c_str();
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_1;
	appInfo.pNext = nullptr;

	VkInstanceCreateInfo instanceInfo;
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.pNext = NULL;
	if (m_vkEnableValidationLayer)
	{
		instanceInfo.enabledLayerCount = (uint32_t)m_vkValidationLayers.size();
		instanceInfo.ppEnabledLayerNames = m_vkValidationLayers.data();
	}
	else
	{
		instanceInfo.enabledLayerCount = 0;
		instanceInfo.ppEnabledLayerNames = nullptr;
	}
	instanceInfo.enabledExtensionCount = (uint32_t)m_vkEnabledExtProperties.size();
	instanceInfo.ppEnabledExtensionNames = m_vkEnabledExtProperties.data();

	result = vkCreateInstance(&instanceInfo, nullptr, &m_vkInstance);
	if (result != VK_SUCCESS)
	{
		std::cout << "Failed to create instance! res:" << result << std::endl;
		exit(1);
	}
}

void ApplicationBase::createVulkanPhysicalDevice()
{
	uint32_t gpuCnt = 1;
	// get gpu count
	VkResult result = vkEnumeratePhysicalDevices(m_vkInstance, &gpuCnt, NULL);
	if (gpuCnt <= 0 || result != VK_SUCCESS)
	{
		std::cout << "Failed to find Gpu Device! res:" << result << std::endl;
		exit(1);
	}

	m_vkPhyDevices.resize(gpuCnt);
	result = vkEnumeratePhysicalDevices(m_vkInstance, &gpuCnt, m_vkPhyDevices.data());
	if (gpuCnt <= m_vkPhyDeviceIndex || result != VK_SUCCESS)
	{
		std::cout << "Failed to find Gpu Device! res:" << result << std::endl;
		exit(1);
	}
}

void ApplicationBase::createVulkanExtProperties()
{
	uint32_t extCnt = 0;
	// take physical device;
	vkEnumerateDeviceExtensionProperties(m_vkPhyDevices[m_vkPhyDeviceIndex], nullptr, &extCnt, nullptr);

	m_vkExtProperties.resize(extCnt);
	vkEnumerateDeviceExtensionProperties(m_vkPhyDevices[m_vkPhyDeviceIndex], nullptr, &extCnt, m_vkExtProperties.data());
}

void ApplicationBase::destoryVulkanInstance()
{
	vkDestroyInstance(m_vkInstance, nullptr);
}

bool ApplicationBase::checkValidationLayerSupport()
{
	uint32_t layerCnt{ 0 };
	vkEnumerateInstanceLayerProperties(&layerCnt, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCnt);
	vkEnumerateInstanceLayerProperties(&layerCnt, availableLayers.data());

	for (const char *layerName : m_vkValidationLayers)
	{
		bool layerFound = false;
		for (const auto &layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) != 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}
	
	return true;
}

void ApplicationBase::EnableValidationLayer(bool enable)
{
	m_vkEnableValidationLayer = enable;
}

void ApplicationBase::addExtProperties(const char *propertyName)
{
	m_vkEnabledExtProperties.push_back(propertyName);
}

void ApplicationBase::setPhysicalDevice(uint8_t index)
{
	m_vkPhyDeviceIndex = index;
}
