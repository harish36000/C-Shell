#include "header.h"

int isInDropped(int i)  {

    for( int j = 0 ; j < droppedProcessesCounter ; j++ )
        if(droppedProcesses[j] == i)
            return 0;
        
    return 1;

}

void sortBgNodes() {

    if(totalBGProcessHistory == 1)
        return;


    for (int i = 1; i <= totalBGProcessHistory; i++)    {
        for (int j = 1; j < totalBGProcessHistory - i - 1; j++)    {

            if (isInDropped(j+1) && isInDropped(j) && strcmp(bgProcess[j].name, bgProcess[j + 1].name) > 0)    {

                Process temp = bgProcess[j];

                bgProcess[j] = bgProcess[j+1];
                bgProcess[j+1] = temp;

            }
        }
    }

}

void backgroundNodeAddition(pid_t pid, char *process_name)  {

    ll emptyIndex = totalBGProcessHistory;

    totalBGProcess++;
    totalBGProcessHistory++;

    bgProcess[emptyIndex].name = malloc(sizeof(char) * strlen(process_name));
    strcpy(bgProcess[emptyIndex].name, process_name);
    bgProcess[emptyIndex].pid = pid;
    bgProcess[emptyIndex].startTime = time(0);
    bgProcess[emptyIndex].jobNo = emptyIndex;

    sortBgNodes();

}

void backgroundProcessExecution(char **passerParams, ll totalArguments)
{
    pid_t pid;

    pid = fork();

    if (pid == 0)
    {
        setpgid(0, 0);
        if (execvp(passerParams[0], passerParams) == -1)
        {
            printf(RED "Shell Error\n");
        }
        exit(0);
    }
    else if (pid < 0)
    {
        printf(RED "Fork Error\n");
    }
    else
    {

        printf("[%lld] %d\n", totalBGProcessHistory, pid);

        char *pname = combination(passerParams, totalArguments);
        backgroundNodeAddition(pid, pname);

        free(pname);
    }
}

void foregroundProcessExecution(char **passerParams, ll totalArguments) {
    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0)
    {
        if (execvp(passerParams[0], passerParams) == -1)
        {
            printf(RED "Shell Error\n");
        }
        exit(1);
    }
    else if (pid < 0)
    {
        printf(RED "Fork Error \n");
    }
    else
    {
        foregroundProcess = pid;
        waitpid(pid, &status, WUNTRACED);

        char *pname = combination(passerParams, totalArguments);
        foregroundNodeAddition(pid, pname);

        free(pname);
    }
}

bool isProcessBG(pid_t pid)
{
    for (ll i = 0; i < MAX_PROCESS_COUNT; i++)
    {
        //bg proc exists
        if (bgProcess[i].name != NULL)
        {
            if (bgProcess[i].pid == pid)
                return true;
        }
    }

    return false;
}

bool isProcessFG(pid_t pid, pid_t pgpid)
{
    if (pid == pgpid)
        return true;

    return false;
}

Process findBGProcessName(int pid)
{
    //deallocate string before returing

    Process retProcess;
    for (int i = 0; i < MAX_PROCESS_COUNT; i++)
    {
        if (bgProcess[i].pid == pid)
        {
            //assign to retprocess
            retProcess.name = malloc(strlen(bgProcess[i].name) * sizeof(char));
            strcpy(retProcess.name, bgProcess[i].name);
            retProcess.pid = bgProcess[i].pid;

            //free memory
            // free(bgProcess[i].name);
            // bgProcess[i].name = NULL;
            // bgProcess[i].pid = 0;
            
            totalBGProcess--;

            return retProcess;
        }
    }
}

void foregroundNodeAddition(pid_t pid, char *process_name)
{
    ll emptyIndex = -1;
    for (ll i = 0; i < MAX_PROCESS_COUNT; i++)
    {
        if (fgProcess[i].name == NULL)
        {
            //printf("empty found at %ld\n", i);

            emptyIndex = i;
            break;
        }
    }

    if (emptyIndex == -1)
    {
        printf(RED "Max process amount reached\n");
    }

    //Add entry to bgPorcess

    fgProcess[emptyIndex].name = malloc(sizeof(char) * strlen(process_name));
    strcpy(fgProcess[emptyIndex].name, process_name);
    fgProcess[emptyIndex].pid = pid;

    // printf("new bg proc: %ld %s\n", bgProcess[emptyIndex].pid, bgProcess[emptyIndex].name);
}

Process findFGProcessName(int pid)
{
    //deallocate string before returing

    Process retProcess;
    for (int i = 0; i < MAX_PROCESS_COUNT; i++)
    {
        if (fgProcess[i].pid == pid)
        {
            //assign to retprocess
            retProcess.name = malloc(strlen(fgProcess[i].name) * sizeof(char));
            strcpy(retProcess.name, fgProcess[i].name);
            retProcess.pid = fgProcess[i].pid;

            //free memory
            free(fgProcess[i].name);
            fgProcess[i].name = NULL;

            return retProcess;
        }
    }
}
