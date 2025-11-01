# $Id$

# Standard gcc accepts this but we have to override this for Apple's gcc.
SHARED = -shared

ifeq ($(UNIX),1)
L_CFLAGS += -DUSE_X11 -DUNIX
L_OBJS += src/backend/x11.o
L_LIBS += -lX11 -lXrender -lXcursor

GL = -lGL -lGLU

E_LIBS += -lm

LIB = lib
SO = .so
EXEC =
endif

ifeq ($(WINDOWS),1)
L_CFLAGS += -DUSE_GDI
L_LDFLAGS += -Wl,--out-implib,src/libMw.a -static-libgcc
L_OBJS += src/backend/gdi.o
L_LIBS += -lgdi32

GL = -lopengl32 -lglu32

LIB =
SO = .dll
EXEC = .exe
endif

ifeq ($(DARWIN),1)
L_CFLAGS += -DSTBI_NO_THREAD_LOCALS -DUSE_DARWIN
L_OBJS += src/backend/mac/mac.o src/backend/mac/carbon.o

LIB = lib
SO = .dylib
EXEC =

SHARED = -dynamiclib
endif
