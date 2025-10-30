# $Id$

ifeq ($(OPENGL),1)
L_OBJS += src/widget/opengl.o

EXAMPLES += examples/gldemos/clock$(EXEC) examples/gldemos/triangle$(EXEC) examples/gldemos/gears$(EXEC) examples/gldemos/boing$(EXEC) examples/gldemos/cube$(EXEC) examples/gldemos/tripaint$(EXEC)
endif
