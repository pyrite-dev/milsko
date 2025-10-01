/* $Id$ */
/*!
 * %file Mw/Vulkan.h
 * %brief Vulkan widget
 * %warning This header is not documented yet
 */

/**
 * ioixd maintains this file. nishi doesn't know vulkan at all
 */

#ifndef __MW_VULKAN_H__
#define __MW_VULKAN_H__

#if !defined(_WIN32) && !defined(__linux__) && !defined(__FreeBSD__)
#error Vulkan is unsupported on the requested platform.
#endif

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <Mw/MachDep.h>
#include <Mw/TypeDefs.h>
#include <Mw/Error.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief Vulkan widget class
 */
MWDECL MwClass MwVulkanClass;

/*!
 * %brief Add an extension to the list of extensions to enable prior to initialization.
 * %warning This must be called before MwCreateWidget.
 */
MWDECL void MwVulkanEnableExtension(const char* ext_name);

/*!
 * %brief Add an layer to the list of layers to enable prior to initialization.
 * %warning This must be called before MwCreateWidget.
 */
MWDECL void MwVulkanEnableLayer(const char* ext_name);

/*!
 * %brief Configuration options that can be passed to setup Vulkan before a widget is created.
 */
typedef struct MwVulkanConfig_T {
	/*!
	 * %brief Vulkan API version (default: VK_API_VERSION_1_0)
	 */
	uint32_t api_version;
	/*!
	 * %brief Vulkan version (default: VK_VERSION_1_0)
	 */
	uint32_t vk_version;
	/*!
	 * %brief Whether or not to enable validation layers (default: false)
	 */
	VkBool32 validation_layers;
} MwVulkanConfig;

/*!
 * %brief Configure Vulkan prior to initializing the widget.
 * %warning This must be called before MwCreateWidget.
 * %warning The configuration provided will be used for future initializations of the Vulkan widget (unless it's changed)
 */
MWDECL void MwVulkanConfigure(MwVulkanConfig cfg);

/*!
 * %brief Field that can be gotten from Vulkan.
 */
typedef enum MwVulkanField_T {
	/*!
	 * %brief The address of the vulkan widget's vkGetInstanceProcAddr function (PFN_vkGetInstanceProcAddr)
	 */
	MwVulkanField_GetInstanceProcAddr = 0,
	/*!
	 * %brief The address of the vulkan widget's instance (VkInstance)
	 */
	MwVulkanField_Instance,
	/*!
	 * %brief The address of the vulkan widget's surface (VkSurfaceKHR)
	 */
	MwVulkanField_Surface,
	/*!
	 * %brief The address of the vulkan widget's physical device (VkPhysicalDevice)
	 */
	MwVulkanField_PhysicalDevice,
	/*!
	 * %brief The address of the vulkan widget's logical device (VkDevice)
	 */
	MwVulkanField_LogicalDevice,
	/*!
	 * %brief The address of the index that the vulkan widget uses for the graphics queue (uint32_t *)
	 */
	MwVulkanField_GraphicsQueueIndex,
	/*!
	 * %brief The address of the index that the vulkan widget uses for the present queue (uint32_t *)
	 */
	MwVulkanField_PresentQueueIndex,
	MwVulkanField_GraphicsQueue,
	/*!
	 * %brief The address of the vulkan widget's graphics queue (VkQueue)
	 */
	MwVulkanField_PresentQueue,
	/*!
	 * %brief The address of the vulkan widget's present queue (VkQueue)
	 */
} MwVulkanField;

/*!
 * %brief Function for getting a field from within Vulkan.
 * %warning Consult the documentation for MwVulkanField to know what type is expected for out.
 */
MWDECL void* MwVulkanGetField(MwWidget handle, MwVulkanField field, MwErrorEnum* out);

#ifdef __cplusplus
}
#endif

#endif
