
startupdir = @PS3SDK@/ppu/startup

startup_DATA = crt1.o crti.o crtn.o linkfile

noinst_HEADERS = spr.h

EXTRA_DIST = crt1.S crti.S crtn.S linkfile

%.o : %.S
	@PPU_CC@ -mregnames -c -o $@ $^


