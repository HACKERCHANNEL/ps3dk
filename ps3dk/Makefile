
include $(PS3DEV)/ppu.mk

OBJECTS		:= src/gpio.o
OUTPUT		:= libps3dk.a
CFLAGS		+= -Iinclude

include $(PS3DEV)/common.mk

install: $(OUTPUT)
	@echo "  COPY      $(OUTPUT)"
	@cp $(OUTPUT) $(PS3DEV)/ppu/lib
	@mkdir -p $(PS3DEV)/ppu/include/ps3dk/
	@echo "  COPY      syscall.h"
	@cp include/ps3dk/syscall.h $(PS3DEV)/ppu/include/ps3dk/
	@echo "  COPY      gpio.h"
	@cp include/ps3dk/gpio.h $(PS3DEV)/ppu/include/ps3dk/


