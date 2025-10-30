# $Id$

ifeq ($(USE_STB_IMAGE),1)
L_CFLAGS += -DUSE_STB_IMAGE
else
include external/deps.mk
endif

ifeq ($(USE_STB_TRUETYPE),1)
L_CFLAGS += -DUSE_STB_TRUETYPE
endif
