#include <stdio.h>
#include <ps3av.h>
#include <ps3gpu.h>
#include <ps3mmu.h>
#include <ps3console.h>

#include <stdio.h>

int main(int argc, char *argv[])
{
    ps3AvVideoInit();
    ps3AvAudioInit();
    ps3AvAvInit(0);
    ps3AvAvbParamDualVideo(PS3AV_MODE_480I, PS3AV_MODE_480I);
    ps3GpuInit();

    ps3MmuInit();
    ps3MmuOn();
    ps3ConsoleInit();

    printf("Hello World!\n");

    return 0;
}

