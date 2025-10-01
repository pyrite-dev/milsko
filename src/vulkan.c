/* $Id$ */
#include <Mw/Milsko.h>
#include <Mw/Vulkan.h>

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR 1
#endif
#ifdef __linux__
#define VK_USE_PLATFORM_XLIB_KHR 1
#endif
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>
#ifdef __linux__
#include <vulkan/vulkan_xlib.h>
#endif

#include <dlfcn.h>
#include <assert.h>
#include <stdbool.h>

#include "stb_ds.h"

// convienence macro for handling vulkan errors
#define VK_CMD(func) \
	vk_res = func; \
	if(vk_res != VK_SUCCESS) { \
		printf("VULKAN ERROR AT %s:%d: %s\n", __FILE__, __LINE__, string_VkResult(vk_res)); \
		exit(0); \
	}

// convienence macro for loading a vulkan function pointer into memory
#define LOAD_VK_FUNCTION(name) \
	PFN_##name _##name = (PFN_##name)o->vkGetInstanceProcAddr(o->vkInstance, #name); \
	assert(_##name);

bool enableValidationLayers = true;

const char** enabledExtensions;
unsigned int enabledExtensionCount = 0;

typedef struct vulkan {
	void*			  vulkanLibrary;
	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
	VkInstance		  vkInstance;
	VkSurfaceKHR		  vkSurface;

	VkPhysicalDevice vkPhysicalDevice;
	VkDevice	 vkLogicalDevice;
	VkQueue		 vkGraphicsQueue;
	VkQueue		 vkPresentQueue;
	uint32_t	 vkGraphicsFamilyIDX;
	uint32_t	 vkPresentFamilyIDX;

	const char** vkInstanceExtensions;
	const char** vkDeviceExtensions;
	const char** vkLayers;
	int	     vkInstanceExtensionCount;
	int	     vkDeviceExtensionCount;
	int	     vkLayerCount;
} vulkan_t;

static void vulkan_instance_setup(MwWidget handle, vulkan_t* o);
static void vulkan_surface_setup(MwWidget handle, vulkan_t* o);
static void vulkan_devices_setup(MwWidget handle, vulkan_t* o);

static void create(MwWidget handle) {
	vulkan_t* o = malloc(sizeof(*o));

	// !! important to call it in this order
	vulkan_instance_setup(handle, o);
	vulkan_surface_setup(handle, o);
	vulkan_devices_setup(handle, o);

	handle->internal = o;
	MwSetDefault(handle);
}

static void destroy(MwWidget handle) {
	vulkan_t* o = (vulkan_t*)handle->internal;
	free(o);
}

static void vulkan_instance_setup(MwWidget handle, vulkan_t* o) {
	// todo: Some sort of function for being able to set the vulkan version?
	uint32_t      vulkan_version  = VK_VERSION_1_0;
	uint32_t      api_version     = VK_API_VERSION_1_0;
	uint32_t      extension_count = 0;
	uint32_t      layer_count     = 0;
	unsigned long i, n = 0;

	PFN_vkEnumerateInstanceExtensionProperties
					       _vkEnumerateInstanceExtensionProperties;
	PFN_vkEnumerateInstanceLayerProperties _vkEnumerateInstanceLayerProperties;
	PFN_vkCreateInstance		       _vkCreateInstance;

	VkApplicationInfo    app_info;
	VkInstanceCreateInfo instance_create_info;

	VkExtensionProperties* ext_props;
	VkLayerProperties*     layer_props;

	VkResult vk_res;

	// TODO: support for whatever win32's equivalants to dlopen/dlsym are
	o->vulkanLibrary	 = dlopen("libvulkan.so", RTLD_LAZY | RTLD_GLOBAL);
	o->vkGetInstanceProcAddr = dlsym(o->vulkanLibrary, "vkGetInstanceProcAddr");
	assert(o->vkGetInstanceProcAddr);

	// Load in any other function pointers we need.
	_vkEnumerateInstanceExtensionProperties = dlsym(o->vulkanLibrary, "vkEnumerateInstanceExtensionProperties");
	assert(_vkEnumerateInstanceExtensionProperties);
	_vkEnumerateInstanceLayerProperties = dlsym(o->vulkanLibrary, "vkEnumerateInstanceLayerProperties");
	assert(_vkEnumerateInstanceLayerProperties);
	_vkCreateInstance = dlsym(o->vulkanLibrary, "vkCreateInstance");
	assert(_vkCreateInstance);

	// setup enabled extensions
	arrput(enabledExtensions, VK_KHR_SURFACE_EXTENSION_NAME);
#if defined(_WIN32)
	arrput(enabledExtensions, VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(__linux__) || defined(__FreeBSD__)
	arrput(enabledExtensions, VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#endif

	// passing null gives us all the extensions provided by the current vulkan implementation
	VK_CMD(_vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL));
	ext_props = malloc(sizeof(VkExtensionProperties) * extension_count);
	VK_CMD(_vkEnumerateInstanceExtensionProperties(NULL, &extension_count, ext_props));
	o->vkInstanceExtensions = malloc(sizeof(const char*) * (arrlen(enabledExtensions) + 1));

	for(i = 0; i < extension_count; i++) {
		for(n = 0; n < arrlen(enabledExtensions); n++) {
			if(strcmp(ext_props[i].extensionName, enabledExtensions[n]) == 0) {
				o->vkInstanceExtensions[o->vkInstanceExtensionCount] = ext_props[i].extensionName;
				o->vkInstanceExtensionCount++;
				break;
			}
		}
	}
	printf("enabled %d instance extensions\n", o->vkInstanceExtensionCount);

	app_info = (VkApplicationInfo){
	    .sType		= VK_STRUCTURE_TYPE_APPLICATION_INFO,
	    .pNext		= NULL,
	    .pApplicationName	= "",
	    .applicationVersion = vulkan_version,
	    .pEngineName	= "",
	    .engineVersion	= vulkan_version,
	    .apiVersion		= api_version,
	};

	VK_CMD(_vkEnumerateInstanceLayerProperties(&layer_count, NULL));
	layer_props = malloc(sizeof(VkLayerProperties) * layer_count);
	VK_CMD(_vkEnumerateInstanceLayerProperties(&layer_count, layer_props));
	o->vkLayers = malloc(256 * (layer_count + 2));
	for(i = 0; i < layer_count; i++) {
		if(enableValidationLayers) {
			if(strcmp(layer_props[i].layerName, "VK_LAYER_KHRONOS_validation") == 0) {
				printf("layer: %s\n", layer_props[i].layerName);
				memset(&o->vkLayers[i], 0, 255);
				memcpy(&o->vkLayers[i], layer_props[i].layerName, 254);
				o->vkLayerCount++;
				break;
			} else {
				continue;
			}
		}
	}
	o->vkLayers[i++] = NULL;

	instance_create_info = (VkInstanceCreateInfo){
	    .sType		     = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	    .flags		     = 0,
	    .pApplicationInfo	     = &app_info,
	    .enabledExtensionCount   = o->vkInstanceExtensionCount,
	    .enabledLayerCount	     = 0,
	    .ppEnabledExtensionNames = o->vkInstanceExtensions,
	    .ppEnabledLayerNames     = o->vkLayers,
	};
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

	VK_CMD(_vkCreateInstance(&instance_create_info, NULL, &o->vkInstance));
}

static void vulkan_surface_setup(MwWidget handle, vulkan_t* o) {
	int vk_res;
#ifdef _WIN32
	LOAD_VK_FUNCTION(vkCreateWin32SurfaceKHR);

	VkWin32SurfaceCreateInfoKHR createInfo = {
	    .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
	    .pNext     = NULL,
	    .flags     = 0,
	    .hinstance = handle->lowlevel->hInstance,
	    .hwnd      = handle->lowlevel->hWnd,
	};

	VK_CMD(_vkCreateWin32SurfaceKHR(o->vkInstance, &createInfo, NULL,
					&o->vkSurface));
#endif
#ifdef __linux__
	LOAD_VK_FUNCTION(vkCreateXlibSurfaceKHR);

	VkXlibSurfaceCreateInfoKHR createInfo = {
	    .sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
	    .pNext  = NULL,
	    .flags  = 0,
	    .dpy    = handle->lowlevel->display,
	    .window = handle->lowlevel->window,
	};
	VK_CMD(_vkCreateXlibSurfaceKHR(o->vkInstance, &createInfo, NULL, &o->vkSurface));
#endif
}

static void vulkan_devices_setup(MwWidget handle, vulkan_t* o) {
	int			 vk_res;
	unsigned long		 i, n;
	uint32_t		 deviceCount;
	VkPhysicalDevice*	 devices;
	uint32_t		 queueFamilyCount;
	VkQueueFamilyProperties* family_props;
	float			 queuePriority = 1.0f;
	VkDeviceQueueCreateInfo	 queueCreateInfos[2];
	VkDeviceCreateInfo	 createInfo;
	VkExtensionProperties*	 ext_props;
	uint32_t		 extension_count;
	VkBool32		 has_graphics	  = false;
	VkBool32		 has_present	  = false;
	int			 queueCreateCount = 0;

	LOAD_VK_FUNCTION(vkEnumeratePhysicalDevices);
	LOAD_VK_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties);
	LOAD_VK_FUNCTION(vkCreateDevice);
	LOAD_VK_FUNCTION(vkGetDeviceQueue);
	LOAD_VK_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR);

	PFN_vkEnumerateDeviceExtensionProperties _vkEnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties)
											     o->vkGetInstanceProcAddr(o->vkInstance, "vkEnumerateDeviceExtensionProperties");
	assert(_vkEnumerateDeviceExtensionProperties);

	// create the physical device
	VK_CMD(_vkEnumeratePhysicalDevices(o->vkInstance, &deviceCount, NULL));
	devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);
	VK_CMD(_vkEnumeratePhysicalDevices(o->vkInstance, &deviceCount, devices));

	for(i = 0; i < deviceCount; i++) {
		_vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, NULL);
		family_props = malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
		_vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, family_props);
		o->vkGraphicsFamilyIDX = 0;
		o->vkPresentFamilyIDX  = 0;
		for(n = 0; n < queueFamilyCount; n++) {
			if(family_props[n].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				has_graphics	       = true;
				o->vkGraphicsFamilyIDX = n;
			};

			_vkGetPhysicalDeviceSurfaceSupportKHR(devices[i], n, o->vkSurface,
							      &has_present);
			if(has_present) {
				o->vkPresentFamilyIDX = n;
			}
			if(has_graphics && has_present) {
				o->vkPhysicalDevice = devices[i];
				break;
			}
		}
		free(family_props);
	}
	if(!has_graphics && !has_present) {
		// rare, yes, but idk maybe some shitty drivers will present this dillema idk.
		printf("There were no devices with either a graphics or presentation queue. Exiting!\n");
		exit(1);
	}

	// create the logical device
	queueCreateInfos[queueCreateCount] = (VkDeviceQueueCreateInfo){
	    .sType	      = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
	    .pNext	      = NULL,
	    .flags	      = 0,
	    .queueFamilyIndex = o->vkGraphicsFamilyIDX,
	    .queueCount	      = 1,
	    .pQueuePriorities = &queuePriority,
	};
	if(o->vkGraphicsFamilyIDX == o->vkPresentFamilyIDX) {
		queueCreateInfos[queueCreateCount++] = (VkDeviceQueueCreateInfo){
		    .sType	      = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		    .pNext	      = NULL,
		    .flags	      = 0,
		    .queueFamilyIndex = o->vkPresentFamilyIDX,
		    .queueCount	      = 1,
		    .pQueuePriorities = &queuePriority,
		};
	}

	VK_CMD(_vkEnumerateDeviceExtensionProperties(o->vkPhysicalDevice, NULL, &extension_count, NULL));
	ext_props = malloc(sizeof(VkExtensionProperties) * extension_count);
	VK_CMD(_vkEnumerateDeviceExtensionProperties(o->vkPhysicalDevice, NULL, &extension_count, ext_props));
	o->vkDeviceExtensions = malloc(sizeof(const char*) * (arrlen(enabledExtensions) + 1));

	for(i = 0; i < extension_count; i++) {
		for(n = 0; n < arrlen(enabledExtensions); n++) {
			if(strcmp(ext_props[i].extensionName, enabledExtensions[n]) == 0) {
				o->vkDeviceExtensions[o->vkDeviceExtensionCount] = ext_props[i].extensionName;
				o->vkDeviceExtensionCount++;
				break;
			}
		}
	}
	printf("enabled %d device extensions\n", o->vkDeviceExtensionCount);

	createInfo = (VkDeviceCreateInfo){
	    .sType		     = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
	    .pNext		     = NULL,
	    .flags		     = 0,
	    .queueCreateInfoCount    = queueCreateCount,
	    .pQueueCreateInfos	     = queueCreateInfos,
	    .pEnabledFeatures	     = NULL,
	    .enabledExtensionCount   = o->vkDeviceExtensionCount,
	    .ppEnabledExtensionNames = o->vkDeviceExtensions,
	    .enabledLayerCount	     = 0,
	    .ppEnabledLayerNames     = NULL,
	};

	VK_CMD(_vkCreateDevice(o->vkPhysicalDevice, &createInfo, NULL, &o->vkLogicalDevice) != VK_SUCCESS);

	_vkGetDeviceQueue(o->vkLogicalDevice, o->vkGraphicsFamilyIDX, 0, &o->vkGraphicsQueue);
	if(o->vkGraphicsFamilyIDX == o->vkPresentFamilyIDX) {
		o->vkPresentQueue = o->vkGraphicsQueue;
	} else {
		_vkGetDeviceQueue(o->vkLogicalDevice, o->vkPresentFamilyIDX, 0, &o->vkPresentQueue);
	}
	// free(devices);
}

void MwVulkanEnableExtension(const char* name) {
	arrput(enabledExtensions, name);
}

PFN_vkGetInstanceProcAddr MwVulkanGetInstanceProcAddr(MwWidget handle) {
	return ((vulkan_t*)handle->internal)->vkGetInstanceProcAddr;
};
VkInstance MwVulkanGetInstance(MwWidget handle) {
	return ((vulkan_t*)handle->internal)->vkInstance;
};
VkSurfaceKHR MwVulkanGetSurface(MwWidget handle) {
	return ((vulkan_t*)handle->internal)->vkSurface;
};
VkPhysicalDevice MwVulkanGetPhysicalDevice(MwWidget handle) {
	return ((vulkan_t*)handle->internal)->vkPhysicalDevice;
};
VkDevice MwVulkanGetLogicalDevice(MwWidget handle) {
	return ((vulkan_t*)handle->internal)->vkLogicalDevice;
};
VkQueue MwVulkanGetGraphicsQueue(MwWidget handle) {
	return ((vulkan_t*)handle->internal)->vkGraphicsQueue;
};
VkQueue MwVulkanGetPresentQueue(MwWidget handle) {
	return ((vulkan_t*)handle->internal)->vkPresentQueue;
};

int MwVulkanGetGraphicsQueueIndex(MwWidget handle) {
	return ((vulkan_t*)handle->internal)->vkGraphicsFamilyIDX;
}

int MwVulkanGetPresentQueueIndex(MwWidget handle) {
	return ((vulkan_t*)handle->internal)->vkPresentFamilyIDX;
}

MwClassRec MwVulkanClassRec = {
    create,  /* create */
    destroy, /* destroy */
    NULL,    /* draw */
    NULL     /* click */
};
MwClass MwVulkanClass = &MwVulkanClassRec;
