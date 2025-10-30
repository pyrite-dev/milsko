# $Id$

ifeq ($(TARGET),NetBSD)
VULKAN_NO_STRING_HELPER = 1
endif

ifeq ($(VULKAN_NO_STRING_HELPER),1)
VK_STRING_HELPER_DEFINE =
else
VK_STRING_HELPER_DEFINE = -DHAS_VK_ENUM_STRING_HELPER
endif
CFLAGS += $(VK_STRING_HELPER_DEFINE)

ifeq ($(VULKAN),1)
L_OBJS += src/widget/vulkan.o
EXAMPLES += examples/vkdemos/vulkan$(EXEC)
endif
