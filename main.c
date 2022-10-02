#include "header.h"

char currentUsername[MAX_LENGTH] = "";
char currentSystemName[MAX_LENGTH] = "";
char currentDirectoryHome[MAX_LENGTH] = "";
char previousDirectoryHome[MAX_LENGTH] = "";

int parent = 0;
int foregroundProcess = 0;

Process bgProcess[MAX_PROCESS_COUNT];
Process fgProcess[MAX_PROCESS_COUNT];

ll totalBGProcess = 0;
bool dontPrintPrompt = false;

char *historyTracker[HISTORY_MAX_SIZE + 1];

time_t getStartTime(pid_t pid)
{
    for (int i = 0; i < MAX_PROCESS_COUNT; i++)
    {
        if (bgProcess[i].name != NULL && bgProcess[i].pid == pid)
        {
            return bgProcess[i].startTime;
        }
    }
}

void childTermination(int signal)
{
    int status;
    pid_t wpid = waitpid(-1, &status, WNOHANG);

    if (wpid > 0 && wpid != foregroundProcess)
    {
        Process a = findBGProcessName(wpid);

        if (WIFEXITED(status))
        {
            printf("%s with pid %d exited normally\n", a.name, a.pid);
        }
        else
        {
            printf("%s with pid %d exited abnormally\n", a.name, a.pid);
            return;
        }

        shellInit();
    }
}

void getRootDirectory()
{
    if (getcwd(currentDirectoryHome, MAX_LENGTH) == NULL)
    {
        printf(RED "Error getting root directory");
    }

    //in the start - we assign root = prev dir
    if (getcwd(previousDirectoryHome, MAX_LENGTH) == NULL)
    {
        printf(RED "Error getting previous directory");
    }
}
void shellInit()
{

    char systemNameInit[MAX_LENGTH] = "";
    if (gethostname(systemNameInit, MAX_LENGTH) == 0)
    {
        strcpy(currentSystemName, systemNameInit);
    }
    else
    {
        printf(RED "Error obtaining system name!");
    }

    uid_t UserID = getuid();
    char userNameInit[MAX_LENGTH] = "";
    strcpy(userNameInit, strdup(getpwuid(UserID)->pw_name));
    strcpy(currentUsername, userNameInit);
    //printf("UserNameInit: %s\n", userNameInit);

    char currDirectoryInit[MAX_LENGTH];
    if (getcwd(currDirectoryInit, MAX_LENGTH) != NULL)
    {
        char *currDirectory = malloc(MAX_LENGTH);
        currDirectory = pathMatchUpdate(currentDirectoryHome, currDirectoryInit);

        printf(BLUE "<%s@%s:" CYAN "%s>", currentUsername, currentSystemName, currDirectory);
        printf(RESET);

        fflush(stdout);
        //<Name@UBUNTU:~>
    }
    else
    {
        printf(RED "Error while obtaining directory");
    }
}

int main()
{

    char individual_command[MAX_LENGTH], *input, *commands;
    ll numCmds = 0;
    size_t input_size = 0;

    getRootDirectory();
    parent = getpid();
    fetchHistory();

    signal(SIGCHLD, childTermination);

    //Initllay maintain prev and current directory as same direcrtorr
    strcpy(previousDirectoryHome, currentDirectoryHome);
    while (1)
    {
        int isThreaded = 1;

        //prints userName@systName
        if (!dontPrintPrompt)
            shellInit();
        else
            dontPrintPrompt = false;

        char temp_string[MAX_LENGTH] = "", c;

        setbuf(stdout, NULL);
        char currRead[2];

        char inputCommand[MAX_LENGTH] = "";
        int inputCommandPtr = 0;
        while (read(0, currRead, 1) != 0 && currRead[0] != '\n')
        {
            inputCommand[inputCommandPtr++] = currRead[0];
            //printf("CurrRead: %s\n", currRead);
        }
        inputCommand[inputCommandPtr] = '\0';

        processInputAndRemoveSpaces(inputCommand);

        char copyInputCommand[MAX_LENGTH];
        strcpy(copyInputCommand, inputCommand);

        addHisNodeEnd(inputCommand);

        ll ANDCmdsCount = 0;

        for (int i = 0; i < strlen(copyInputCommand); i++)
        {
            if (copyInputCommand[i] == '&')
            {
                ANDCmdsCount += 1;
            }
        }

        if (ANDCmdsCount)
        {
            //"sleep 2& sleep 3&"
            char *token = strtok(inputCommand, "&");

            strncpy(copyInputCommand, token, sizeof(token));
            copyInputCommand[strlen(token)] = ' ';
            copyInputCommand[strlen(token) + 1] = '&';
            copyInputCommand[strlen(token) + 2] = '\0';

            // printf("After strok: %s\n", copyInputCommand);
            //"sleep 2 &"

            //Got sleep 2 & -- easier to pass arg later when using execvp
            //While giving to execvp
            //arg[0] = sleep
            //arg[1] = 2

            processInputAndRemoveSpaces(copyInputCommand);

            if (copyInputCommand[strlen(copyInputCommand) - 1] == '\n')
                copyInputCommand[strlen(copyInputCommand) - 1] = '\0';

            char **commands = parseAndCountInputs(copyInputCommand, ";", &numCmds);
            // printf("Commands after parsing \n");
            for (ll i = 0; i < numCmds; i++)
            {
                // printf("%s\n", commands[i]);
                processCommandsInQueue(commands[i]);
            }
            releaseMemoryAndFree(&commands, numCmds);

            ANDCmdsCount--;

            //Repeat same //Got sleep 2 & for all commands followed by &
            //Steps
            // 1. Get one token that uses &
            // 2. Separate all of them by ;
            // 3. Run them by calling processInput function

            while (token != NULL)
            {
                token = strtok(NULL, "&");

                if (token != NULL)
                {
                    memset(copyInputCommand, 0, sizeof(copyInputCommand));
                    /*

                    When we do size of, it doesn't calculate the entire length, and treats space as limiter 
                    Thats why kept it 500 or any const value
                    */

                    strncpy(copyInputCommand, token, 500);

                    if (ANDCmdsCount > 0)
                    {
                        copyInputCommand[strlen(token)] = ' ';
                        copyInputCommand[strlen(token) + 1] = '&';
                        copyInputCommand[strlen(token) + 2] = '\0';
                        ANDCmdsCount--;
                    }

                    processInputAndRemoveSpaces(copyInputCommand);

                    if (copyInputCommand[strlen(copyInputCommand) - 1] == '\n')
                        copyInputCommand[strlen(copyInputCommand) - 1] = '\0';

                    char **commands = parseAndCountInputs(copyInputCommand, ";", &numCmds);
                    for (ll i = 0; i < numCmds; i++)
                    {
                        processCommandsInQueue(commands[i]);
                    }
                    releaseMemoryAndFree(&commands, numCmds);
                }
            }
        }
        else
        {
            processInputAndRemoveSpaces(inputCommand);

            if (inputCommand[strlen(input) - 1] == '\n')
                inputCommand[strlen(input) - 1] = '\0';

            char **commands = parseAndCountInputs(inputCommand, ";", &numCmds);
            for (ll i = 0; i < numCmds; i++)
            {
                processCommandsInQueue(commands[i]);
            }
            releaseMemoryAndFree(&commands, numCmds);
        }
        memset(copyInputCommand, 0, sizeof(copyInputCommand));
    }
}