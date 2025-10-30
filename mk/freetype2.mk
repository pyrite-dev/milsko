# $Id$

ifeq ($(USE_FREETYPE2),1)
L_CFLAGS += -DUSE_FREETYPE2

ifneq ($(CROSS),1)
L_CFLAGS += $(shell pkg-config --cflags freetype2)
L_LIBS += $(shell pkg-config --libs freetype2)
endif
endif
