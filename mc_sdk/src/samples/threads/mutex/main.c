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

static ps3Mutex_t mutex = PS3_MUTEX_INITIALIZER;

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

static void threadFunc(void *arg)
{
    int id = (int)(ptrdiff_t)arg;
    int r = ((id&1)? 15 : 2), c = ((id&2)? 40 : 4);

    printAt(r++, c, "Thread %d takes the mutex", id);
    if(ps3MutexLockBefore(&mutex, ps3TbRead()+15*ps3TbHz())) {
      printAt(r++, c, "Thread %d gives up after 15s", id);
      return;
    }
    printAt(r++, c, "Thread %d has the mutex", id);
    printAt(r++, c, "Thread %d takes the mutex again", id);
    ps3MutexLock(&mutex);
    printAt(r++, c, "Thread %d sleeps for a bit...", id);
    sleep(3);
    printAt(r++, c, "Thread %d releases the mutex", id);
    ps3MutexUnlock(&mutex);
    printAt(r++, c, "Thread %d sleeps some more...", id);
    sleep(3);
    printAt(r++, c, "Thread %d releases the mutex again", id);
    ps3MutexUnlock(&mutex);
    printAt(r++, c, "Thread %d takes the mutex", id);
    ps3MutexLock(&mutex);
    printAt(r++, c, "Thread %d has the mutex", id);
    printAt(r++, c, "Thread %d sleeps for a bit...", id);
    sleep(3);
    printAt(r++, c, "Thread %d releases the mutex", id);
    ps3MutexUnlock(&mutex);
    printAt(r++, c, "Thread %d is done.", id);
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

    /* Create four threads */
    for(i=0; i<4; i++)
        ps3ThreadCreate(threadFunc, (void *)(ptrdiff_t)i, 8192, 100);

    /* Main thread sleeps forever */
    for(;;)
        sleep(100);
}

