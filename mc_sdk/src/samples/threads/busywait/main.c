#include <stdio.h>
#include <ps3av.h>
#include <ps3gpu.h>
#include <ps3mmu.h>
#include <ps3console.h>
#include <ps3thread.h>
#include <ps3tb.h>

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

static uint64_t tbhz;

static int printAt(int r, int c, char *fmt, ...)
{
    char buf[128];
    int i, cnt;
    va_list va;
    va_start(va, fmt);
    cnt = vsnprintf(buf, sizeof(buf), fmt, va);
    for(i=0; i<cnt; i++)
        if(!buf[i])
	    break;
	else
	    ps3ConsoleCharAt(r, c+i, buf[i], 0xffffff, 0);
    va_end(va);
    return cnt;
}

static void busyWait(int t)
{
    uint64_t target = tbhz*t + ps3TbRead();

    /* Lower the priority of this HW thread while looping */
    __asm__ __volatile__ ("or 1,1,1");

    while ((int64_t)(ps3TbRead() - target) < 0)
        ;

    /* Restore normal priority */
    __asm__ __volatile__ ("or 2,2,2");
}

static int getHwThreadId(void)
{
    uint32_t id;
    __asm__("mfspr %0,0x88; cntlzw %0,%0" : "=r" (id));
    return id;
}

static void threadFunc(void *arg)
{
    int id = (int)(ptrdiff_t)arg;
    int i, r = ((id&1)? 15 : 2), c = ((id&2)? 40 : 4);

    for (i=0; i<5; i++) {
        printAt(r++, c, "Thread %d (HW %d) busy waits",
		id, getHwThreadId());
	busyWait(2);
    }
      
    for (i=0; i<5; i++) {
        printAt(r++, c, "Thread %d (HW %d) sleeps",
		id, getHwThreadId());
	sleep(2);
    }
}

int main(int argc, char *argv[])
{
    int i;

    ps3AvVideoInit();
    ps3AvAudioInit();
    ps3AvAvInit(0);
    ps3AvAvbParamDualVideo(PS3AV_MODE_480I, PS3AV_MODE_480I);
    ps3GpuInit();

    ps3MmuInit();
    ps3MmuOn();
    ps3ConsoleInit();

    tbhz = ps3TbHz();

    /* Create four threads */
    for(i=0; i<4; i++)
        ps3ThreadCreate(threadFunc, (void *)(ptrdiff_t)i, 8192, 100);

    /* Main thread sleeps forever */
    for(;;)
        sleep(100);
}

