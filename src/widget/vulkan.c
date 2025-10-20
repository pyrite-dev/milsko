/* $Id$ */
#include <Mw/Milsko.h>
#include <Mw/Widget/Vulkan.h>

#include "../error_internal.h"

/**
 * ioixd maintains this file. nishi doesn't know vulkan at all
 */

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR 1
#endif
#ifdef __linux__
#define VK_USE_PLATFORM_XLIB_KHR 1
#endif
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#ifdef __linux__
#include <vulkan/vulkan_xlib.h>
#endif

#ifdef _WIN32
#include <vulkan/vulkan_win32.h>
#endif

#ifdef HAS_VK_ENUM_STRING_HELPER
#include <vulkan/vk_enum_string_helper.h>
#endif

#include <stdbool.h>

#include "../../external/stb_ds.h"

MwVulkanConfig vulkan_config = {
    .api_version       = VK_API_VERSION_1_0,
    .vk_version	       = VK_VERSION_1_0,
    .validation_layers = VK_FALSE,
};

#ifndef _WIN32
#define LIB_TYPE void*
#define LIB_OPEN(a, b) dlopen(a, b)
#define LIB_SYM(a, b) dlsym(a, b)
#define LIB_CLOSE(a) dlclose(a)
#else
#define LIB_TYPE HINSTANCE
#define LIB_OPEN(a, b) LoadLibrary(a)
#define LIB_SYM(a, b) (void*)GetProcAddress(a, b)
#define LIB_CLOSE(a) FreeLibrary(a)
#endif

// convienence macro for handling vulkan errors
#ifdef HAS_VK_ENUM_STRING_HELPER
#define VK_CMD(func) \
	vk_res = func; \
	if(vk_res != VK_SUCCESS) { \
		setLastError("Vulkan error (%s:%d): %s\n", __FILE__, __LINE__, string_VkResult(vk_res)); \
		return MwEerror; \
	}
#else
#define VK_CMD(func) \
	vk_res = func; \
	if(vk_res != VK_SUCCESS) { \
		setLastError("Vulkan error (%s:%d): (error %d)\n", __FILE__, __LINE__, vk_res); \
		return MwEerror; \
	}
#endif

// convienence macro for loading a vulkan function pointer into memory
#define LOAD_VK_FUNCTION(name) \
	PFN_##name _##name = (PFN_##name)o->vkGetInstanceProcAddr(o->vkInstance, #name); \
	VK_ASSERT(_##name);

// convienence macro to return an error if an assert goes wrong
#define VK_ASSERT(val) \
	if(!val) { \
		setLastError("Vulkan error (%s:%d): Assertion Failed (%s != NULL)\n", __FILE__, __LINE__, #val); \
		return MwEerror; \
	}

const char** enabledExtensions;
const char** enabledLayers;

typedef enum vulkan_supported_state_t {
	VULKAN_SUPPORTED_UNKNOWN,
	VULKAN_SUPPORTED_YES,
	VULKAN_SUPPORTED_NO,
} vulkan_supported_state;

vulkan_supported_state vulkan_supported = VULKAN_SUPPORTED_UNKNOWN;

typedef struct vulkan {
	LIB_TYPE		  vulkanLibrary;
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

static MwErrorEnum vulkan_instance_setup(MwWidget handle, vulkan_t* o);
static MwErrorEnum vulkan_surface_setup(MwWidget handle, vulkan_t* o);
static MwErrorEnum vulkan_devices_setup(MwWidget handle, vulkan_t* o);

static int create(MwWidget handle) {
	vulkan_t*   o = malloc(sizeof(*o));
	MwErrorEnum err;

	err = vulkan_instance_setup(handle, o);
	if(err != MwEsuccess) {
		printf("%s", MwGetLastError());
		return 1;
	}
	err = vulkan_surface_setup(handle, o);
	if(err != MwEsuccess) {
		printf("%s", MwGetLastError());
		return 1;
	}
	err = vulkan_devices_setup(handle, o);
	if(err != MwEsuccess) {
		printf("%s", MwGetLastError());
		return 1;
	}

	handle->lowlevel->copy_buffer = 0;

	handle->internal = o;
	MwSetDefault(handle);

	return 0;
}

static MwErrorEnum _destroy(MwWidget handle) {
	vulkan_t* o = (vulkan_t*)handle->internal;

	LOAD_VK_FUNCTION(vkDestroyInstance);
	LOAD_VK_FUNCTION(vkDestroySurfaceKHR);
	LOAD_VK_FUNCTION(vkDestroyDevice);

	free(o->vkLayers);
	free(o->vkDeviceExtensions);
	free(o->vkInstanceExtensions);
	_vkDestroyDevice(o->vkLogicalDevice, NULL);
	_vkDestroySurfaceKHR(o->vkInstance, o->vkSurface, NULL);
	_vkDestroyInstance(o->vkInstance, NULL);
	LIB_CLOSE(o->vulkanLibrary);

	free(o);

	return MwEsuccess;
}

static void destroy(MwWidget handle) {
	MwErrorEnum err = _destroy(handle);
	if(err == MwEerror) {
		printf("[Vulkan Widget] %s", MwGetLastError());
	}
}

static LIB_TYPE vulkan_lib_load() {
#ifdef _WIN32
	return LIB_OPEN("vulkan-1.dll", RTLD_LAZY | RTLD_GLOBAL);
#else
	return LIB_OPEN("libvulkan.so", RTLD_LAZY | RTLD_GLOBAL);
#endif
}

static MwErrorEnum vulkan_instance_setup(MwWidget handle, vulkan_t* o) {
	(void)(handle);

	uint32_t      vulkan_version  = vulkan_config.vk_version;
	uint32_t      api_version     = vulkan_config.api_version;
	uint32_t      extension_count = 0;
	uint32_t      layer_count     = 0;
	unsigned long i		      = 0;
	unsigned long n		      = 0;

	PFN_vkEnumerateInstanceExtensionProperties
					       _vkEnumerateInstanceExtensionProperties;
	PFN_vkEnumerateInstanceLayerProperties _vkEnumerateInstanceLayerProperties;
	PFN_vkCreateInstance		       _vkCreateInstance;

	VkApplicationInfo    app_info;
	VkInstanceCreateInfo instance_create_info;

	VkExtensionProperties* ext_props;
	VkLayerProperties*     layer_props;

	VkResult vk_res;

	// Load Vulkan and any other function pointers we need.
	o->vulkanLibrary = vulkan_lib_load();
	if(!o->vulkanLibrary) {
		vulkan_supported = VULKAN_SUPPORTED_NO;
		setLastError("Vulkan not found.");
	} else {
		vulkan_supported = VULKAN_SUPPORTED_YES;
	}

	o->vkGetInstanceProcAddr = LIB_SYM(o->vulkanLibrary, "vkGetInstanceProcAddr");
	VK_ASSERT(o->vkGetInstanceProcAddr);
	_vkEnumerateInstanceExtensionProperties = LIB_SYM(o->vulkanLibrary, "vkEnumerateInstanceExtensionProperties");
	VK_ASSERT(_vkEnumerateInstanceExtensionProperties);
	_vkEnumerateInstanceLayerProperties = LIB_SYM(o->vulkanLibrary, "vkEnumerateInstanceLayerProperties");
	VK_ASSERT(_vkEnumerateInstanceLayerProperties);
	_vkCreateInstance = LIB_SYM(o->vulkanLibrary, "vkCreateInstance");
	VK_ASSERT(_vkCreateInstance);

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
		for(n = 0; n < (unsigned long)arrlen(enabledExtensions); n++) {
			if(strcmp(ext_props[i].extensionName, enabledExtensions[n]) == 0) {
				printf("[Vulkan Widget] Enabling extension %s\n", ext_props[i].extensionName);
				o->vkInstanceExtensions[o->vkInstanceExtensionCount] = ext_props[i].extensionName;
				o->vkInstanceExtensionCount++;
				break;
			}
		}
	}

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
	o->vkLayers = malloc(256 * (arrlen(enabledLayers) + 1));

	if(vulkan_config.validation_layers) {
		arrput(enabledLayers, "VK_LAYER_KHRONOS_validation");
	}
	for(i = 0; i < layer_count; i++) {
		for(n = 0; n < (unsigned long)arrlen(enabledLayers); n++) {
			if(strcmp(layer_props[i].layerName, enabledLayers[n]) == 0) {
				printf("[Vulkan Widget] Enabling layer %s\n", layer_props[i].layerName);
				o->vkLayers[o->vkLayerCount] = layer_props[i].layerName;
				o->vkLayerCount++;
				break;
			}
		}
	}
	o->vkLayers[i++] = NULL;

	instance_create_info = (VkInstanceCreateInfo){
	    .sType		     = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	    .flags		     = 0,
	    .pApplicationInfo	     = &app_info,
	    .enabledExtensionCount   = o->vkInstanceExtensionCount,
	    .enabledLayerCount	     = o->vkLayerCount,
	    .ppEnabledExtensionNames = o->vkInstanceExtensions,
	    .ppEnabledLayerNames     = o->vkLayers,
	};
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

	VK_CMD(_vkCreateInstance(&instance_create_info, NULL, &o->vkInstance));
	return MwEsuccess;
}

static MwErrorEnum vulkan_surface_setup(MwWidget handle, vulkan_t* o) {
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
	return MwEsuccess;
}

static MwErrorEnum vulkan_devices_setup(MwWidget handle, vulkan_t* o) {
	(void)(handle);

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
	LOAD_VK_FUNCTION(vkEnumerateDeviceExtensionProperties);

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
		setLastError("There were no devices with either a graphics or presentation queue.\n");
		return MwEerror;
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
		for(n = 0; n < (unsigned long)arrlen(enabledExtensions); n++) {
			if(strcmp(ext_props[i].extensionName, enabledExtensions[n]) == 0) {
				o->vkDeviceExtensions[o->vkDeviceExtensionCount] = ext_props[i].extensionName;
				o->vkDeviceExtensionCount++;
				break;
			}
		}
	}

	createInfo = (VkDeviceCreateInfo){
	    .sType		     = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
	    .pNext		     = NULL,
	    .flags		     = 0,
	    .queueCreateInfoCount    = queueCreateCount,
	    .pQueueCreateInfos	     = queueCreateInfos,
	    .pEnabledFeatures	     = NULL,
	    .enabledExtensionCount   = o->vkDeviceExtensionCount,
	    .ppEnabledExtensionNames = o->vkDeviceExtensions,
	    .enabledLayerCount	     = o->vkLayerCount,
	    .ppEnabledLayerNames     = o->vkLayers,
	};

	VK_CMD(_vkCreateDevice(o->vkPhysicalDevice, &createInfo, NULL, &o->vkLogicalDevice) != VK_SUCCESS);

	_vkGetDeviceQueue(o->vkLogicalDevice, o->vkGraphicsFamilyIDX, 0, &o->vkGraphicsQueue);
	if(o->vkGraphicsFamilyIDX == o->vkPresentFamilyIDX) {
		o->vkPresentQueue = o->vkGraphicsQueue;
	} else {
		_vkGetDeviceQueue(o->vkLogicalDevice, o->vkPresentFamilyIDX, 0, &o->vkPresentQueue);
	}
	free(devices);
	return MwEsuccess;
}

void MwVulkanConfigure(MwVulkanConfig cfg) {
	vulkan_config = cfg;
}

void MwVulkanEnableExtension(const char* name) {
	arrput(enabledExtensions, name);
}

void MwVulkanEnableLayer(const char* name) {
	arrput(enabledLayers, name);
}

VkBool32 MwVulkanSupported(void) {
	if(vulkan_supported == VULKAN_SUPPORTED_UNKNOWN) {
		LIB_TYPE lib = vulkan_lib_load();
		if(lib == NULL) {
			vulkan_supported = VULKAN_SUPPORTED_NO;
		} else {
			vulkan_supported = VULKAN_SUPPORTED_YES;
			LIB_CLOSE(lib);
		}
	}
	if(vulkan_supported == VULKAN_SUPPORTED_YES) {
		return VK_TRUE;
	} else {
		return VK_FALSE;
	}
};

static void* mwVulkanGetFieldImpl(MwWidget handle, MwVulkanField field, MwErrorEnum* out) {
	vulkan_t* o = handle->internal;

	switch(field) {
	case MwVulkanField_GetInstanceProcAddr:
		return o->vkGetInstanceProcAddr;
	case MwVulkanField_Instance:
		return o->vkInstance;
	case MwVulkanField_Surface:
		return o->vkSurface;
	case MwVulkanField_PhysicalDevice:
		return o->vkPhysicalDevice;
	case MwVulkanField_LogicalDevice:
		return o->vkLogicalDevice;
	case MwVulkanField_GraphicsQueueIndex:
		return &o->vkGraphicsFamilyIDX;
	case MwVulkanField_PresentQueueIndex:
		return &o->vkPresentFamilyIDX;
	case MwVulkanField_GraphicsQueue:
		return o->vkGraphicsQueue;
	case MwVulkanField_PresentQueue:
		return o->vkPresentQueue;
	default:
		setLastError("Unknown vulkan field request (%d given)", field);
		if(out != NULL) {
			*out = MwEerror;
		}
		return NULL;
	}
	if(out != NULL) {
		*out = MwEsuccess;
	}
};

static void func_handler(MwWidget handle, const char* name, void* out, va_list va) {
	if(strcmp(name, "mwVulkanGetField") == 0) {
		MwVulkanField field = va_arg(va, MwVulkanField);
		MwErrorEnum*  err   = va_arg(va, MwErrorEnum*);
		*(void**)out	    = mwVulkanGetFieldImpl(handle, field, err);
	}
}

MwClassRec MwVulkanClassRec = {
    create,	  /* create */
    destroy,	  /* destroy */
    NULL,	  /* draw */
    NULL,	  /* click */
    NULL,	  /* parent_resize */
    NULL,	  /* prop_change */
    NULL,	  /* mouse_move */
    NULL,	  /* mouse_up */
    NULL,	  /* mouse_down */
    NULL,	  /* key */
    func_handler, /* custom */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL};
MwClass MwVulkanClass = &MwVulkanClassRec;
