#include "header.h"
#define ClearTerminos clearTerminos("tcgetattr");

struct termios bufferlessTerminalOrig;

void clearTerminos(const char *s)
{
    printf(RED "%s failed\n", s);
    printf(RESET);
    exit(1);
}

void exposeBufferlessTerminal()
{
    //get Params associated with terminal and store in buffferlessTerminalOrig
    if (tcgetattr(0, &bufferlessTerminalOrig) == -1)
        ClearTerminos;

    //Copying original state to restore back at the end
    struct termios nativeBuffer = bufferlessTerminalOrig;

    atexit(disableBufferlessTerminal);

    //c_lflag - input mode
    /*
        Turn off echo - stops printing what ur typing
        Turn off icanon - 

    */
    nativeBuffer.c_lflag &= ~(ICANON | ECHO);

    if (tcsetattr(0, 2, &nativeBuffer) == -1)
        ClearTerminos;
}

void disableBufferlessTerminal()
{
    //set params associated with termainl to .
    if (tcsetattr(0, 2, &bufferlessTerminalOrig) == -1)
        ClearTerminos;
}