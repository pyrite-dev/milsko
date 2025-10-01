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

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * %brief Vulkan widget class
 */
MWDECL MwClass MwVulkanClass;

// Add an extension to the list of extensions to enable prior to initialization.
// This must be called before MwCreateWidget.
MWDECL void MwVulkanEnableExtension(const char* ext_name);

MWDECL PFN_vkGetInstanceProcAddr MwVulkanGetInstanceProcAddr(MwWidget handle);
MWDECL VkInstance		 MwVulkanGetInstance(MwWidget handle);
MWDECL VkSurfaceKHR		 MwVulkanGetSurface(MwWidget handle);
MWDECL VkPhysicalDevice		 MwVulkanGetPhysicalDevice(MwWidget handle);
MWDECL VkDevice			 MwVulkanGetLogicalDevice(MwWidget handle);
MWDECL int			 MwVulkanGetGraphicsQueueIndex(MwWidget handle);
MWDECL int			 MwVulkanGetPresentQueueIndex(MwWidget handle);
MWDECL VkQueue			 MwVulkanGetGraphicsQueue(MwWidget handle);
MWDECL VkQueue			 MwVulkanGetPresentQueue(MwWidget handle);

#ifdef __cplusplus
}
#endif

#endif
