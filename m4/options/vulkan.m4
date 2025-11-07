dnl $Id$
ifdef([build_vulkan],[new_object([src/widget/vulkan.c])dnl
new_example([examples/vkdemos/vulkan])dnl
ifdef([use_vulkan_string_helper],[add_comdef([-DHAS_VK_ENUM_STRING_HELPER])dnl
])dnl
])dnl
