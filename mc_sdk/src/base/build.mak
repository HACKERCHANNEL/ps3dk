# PS3 Software Development Kit
# -----------------------------------------------------------------------
# Licensed under the BSD license, see LICENSE in PS3SDK root for details.
#
# build.mak - Base makefile for projects using PS3SDK.
#
# Copyright (c) 2008 Marcus Comstedt
#

# Note: The PS3SDK make variable must be defined before this file is included.
ifeq ($(PS3SDK),)
$(error $$(PS3SDK) is undefined.  Use "PS3SDK := $$(PS3DEV)/ps3sdk" in your Makefile)
endif

CC       = ppu-gcc
CXX      = ppu-g++
AS       = ppu-gcc
LD       = ppu-gcc
AR       = ppu-ar
RANLIB   = ppu-ranlib
STRIP    = ppu-strip

# Add in PS3SDK includes and libraries.
INCDIR   := $(INCDIR) . $(PS3SDK)/ppu/include
LIBDIR   := $(LIBDIR) . $(PS3SDK)/ppu/lib

CFLAGS   := $(addprefix -I,$(INCDIR)) $(CFLAGS)
CXXFLAGS := $(CFLAGS) $(CXXFLAGS)
ASFLAGS  := $(CFLAGS) $(ASFLAGS)
LDFLAGS  := $(addprefix -L,$(LIBDIR)) -B$(PS3SDK)/ppu/startup -T linkfile

PS3SDK_LIBS = -lps3network -lps3console -lps3av -lps3vuart \
	-lps3gpu -lps3kernel -lps3hv
LIBS     := $(LIBS) -lc -lps3posix -lps3nlglue -llwipv4 $(PS3SDK_LIBS) -lc -lps3nlglue

all: $(EXTRA_TARGETS) $(FINAL_TARGET)

$(TARGET).elf: $(OBJS)
	$(LINK.c) $^ $(LIBS) -o $@

clean: 
	-rm -f $(FINAL_TARGET) $(EXTRA_CLEAN) $(OBJS) $(EXTRA_TARGETS)
