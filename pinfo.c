#include "header.h"

void getExePath(pid_t pid)
{
    //Read exe file
    /*
        - exe path
    */

    char fileName[25];
    char buf[20];
    sprintf(fileName, "/proc/%d/exe", pid);

    ll len = readlink(fileName, buf, MAX_LENGTH);
    buf[len] = '\0';

    char *printPath = pathMatchUpdate(currentDirectoryHome, buf);

    printf("executable Path : %s\n", printPath);
}

void pinfoCommand(pid_t pid)
{
    /* Steps

        4 subparts 
        1. print pid
        2. procStatus
        3. Memory
        4. exe path


        1,2,3 -> stat file

 
        Exe Path
        read exe file -> readlink (look for alt) 

    */

    //Read stat file "/proc/%d/stat"
    /*
        - [1] - pid
        - [2] - state
        - [23] - vsize
    */

    // printf("fn: pid : %d\n", pid);

    char fileName[25];
    char buf[10];

    //Assuming no PID other than running PID is given

    sprintf(fileName, "/proc/%d/stat", pid);

    FILE *ptr = fopen(fileName, "r");

    // If process does not exist
    if (ptr == NULL)
    {
        printf(RED "Process not found");
        return;
    }

    char execPath[MAX_LENGTH + 1], state;
    unsigned long int vSize;
    pid_t pgpid;
    ll virtualMemSize;

    for (ll i = 1; i <= 23; i++)
    {
        fscanf(ptr, "%s", buf);
        // printf("%lld %s\n", i, buf);

        if (i == 3)
            state = buf[0];
        else if (i == 5)
            pgpid = atoi(buf);
        else if (i == 23)
            virtualMemSize = atoi(buf);
    }

    printf("pid : %d\n", pid);
    printf("process Status : %c", state);
    if (isProcessBG(pid))
        printf("\n");
    else
        printf("+\n");

    printf("memory : %lld\n", virtualMemSize);

    getExePath(pid);
}