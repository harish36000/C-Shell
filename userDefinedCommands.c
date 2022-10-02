#include "header.h"

void executeJobsCommand(char **passerParams, ll totalArguments)   {

    if (totalArguments == 1) {

            for (ll i = 1; i <= totalBGProcessHistory; i++)
            {
                if (bgProcess[i].name != NULL)
                {

                    char status = getProcessStatus(bgProcess[i].pid);

                    if (status == 'R' || status == 'S')
                        printf("[%d] Running %s [%d]\n",bgProcess[i].jobNo, bgProcess[i].name, bgProcess[i].pid);
                    else
                        printf("[%d] Stopped %s [%d]\n",bgProcess[i].jobNo, bgProcess[i].name, bgProcess[i].pid);
                }
            }
    }
    else    {
        bool rFlag = false;
        bool sFlag = false;

        if (strcmp(passerParams[1], "-r") == 0)
            rFlag = true;
        if (strcmp(passerParams[1], "-s") == 0)
            sFlag = true;
        if (!strcmp(passerParams[1], "-sr") || !strcmp(passerParams[1], "-rs"))    
            rFlag = sFlag = true;

        for (ll i = 1; i <= totalBGProcessHistory; i++)
        {
            if (bgProcess[i].name != NULL)    {

                char status = getProcessStatus(bgProcess[i].pid);

                if ((status == 'R' || status == 'S') && rFlag == true)
                    printf("[%d] Running %s [%d]\n",bgProcess[i].jobNo,  bgProcess[i].name, bgProcess[i].pid);
                else if(status != 'R' && status != 'S' && sFlag == true)
                    printf("[%d] Stopped %s [%d]\n",bgProcess[i].jobNo, bgProcess[i].name, bgProcess[i].pid);
            }
        }
        
    }
}

void signalRunningProcess(int processNo, int signal)    {
    
    if(processNo > totalBGProcessHistory)   {
        printf("No such process exit : %d\n", processNo);
        return;
    }

    if (signal > 0 && signal < 65)    {

        char status; 
        int accessNo = -1;

        for( int i = 1 ; i <= totalBGProcessHistory ; i++ )
            if(bgProcess[i].jobNo == processNo) {
                accessNo = i;
                status = getProcessStatus(bgProcess[accessNo].pid);
                break;
            }
        if(accessNo == -1)  {
            printf("No such process exit : %d\n", processNo);
            return;
        }
        
        if (status == 'R' || status == 'S') 
            kill((pid_t)(bgProcess[accessNo].pid), signal);
        else    
            printf("Process %s with Pid %d already killed\n", bgProcess[accessNo].name, bgProcess[accessNo].pid);
    }
    else
        printf("No such signal exists : %d\n", signal);
    
}

void executeFgCommand(int processNo) {

    if(processNo >= totalBGProcessHistory)   {
        printf("No such process exit : %d\n", processNo);
        return;
    }

    char status; 
    int accessNo = -1;

    for( int i = 1 ; i <= totalBGProcessHistory ; i++ )
        if(bgProcess[i].jobNo == processNo) {
            accessNo = i;
            status = getProcessStatus(bgProcess[accessNo].pid);
            break;
        }        
    if(accessNo == -1)  {
        printf("No such process exit : %d\n", processNo);
        return;
    }

    if (status == 'R' || status == 'S') 
        kill((pid_t)(bgProcess[accessNo].pid), 18);
    
    printf("Moved process %d with pid %d to foreground\n", processNo, bgProcess[accessNo].pid);

    Process temp = bgProcess[accessNo];

    bgProcess[accessNo] = bgProcess[totalBGProcessHistory];
    droppedProcesses[droppedProcessesCounter++] = accessNo;
    totalBGProcessHistory--;

    sortBgNodes();

    //Execute command 

    pid_t pid;
    int status_w;
    pid = fork();
    ll numCmds;
    char **commands = parseAndCountInputs(temp.name, " ", &numCmds);

    if (pid == 0)
    {
        if (execvp(commands[0], commands) == -1)
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
        waitpid(pid, &status_w, WUNTRACED);

        char *pname = temp.name;
        foregroundNodeAddition(pid, pname);

        free(pname);
    }

}

void executeBgCommand(int processNo) {

    if(processNo >= totalBGProcessHistory)   {
        printf("No such process exit : %d\n", processNo);
        return;
    }

    char status; 
    int accessNo = -1;

    for( int i = 1 ; i <= totalBGProcessHistory ; i++ )
        if(bgProcess[i].jobNo == processNo) {
            accessNo = i;
            status = getProcessStatus(bgProcess[accessNo].pid);
            break;
        }   
    if(accessNo == -1)  {
        printf("No such process exit : %d\n", processNo);
        return;
    }

    if (status == 'R' || status == 'S') {
        printf("Process [%d] with Pid %d already in execution\n", processNo, bgProcess[accessNo].pid);
        return;
    }

    Process temp = bgProcess[accessNo];

    bgProcess[accessNo] = bgProcess[totalBGProcessHistory];
    droppedProcesses[droppedProcessesCounter++] = accessNo;
    totalBGProcessHistory--;

    pid_t pid = fork();
    ll numCmds;
    char **commands = parseAndCountInputs(temp.name, " ", &numCmds);

    if (pid == 0)
    {
        setpgid(0, 0);
        if (execvp(commands[0], commands) == -1)
        {
            printf(RED "Shell Error\n");
        }
        return;
    }
    else if (pid < 0)
    {
        printf(RED "Fork Error\n");
    }
    else
    {

        //printf("[%lld] %d\n", totalBGProcess, pid);

        char *pname = temp.name;
        backgroundNodeAddition(pid, pname);

        free(pname);
    }
}