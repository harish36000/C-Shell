#include "header.h"

void executeCDCommand(char **passerParams, ll totalArguments)
{
    /***************** CHANGE DIRECTORY ********************/
    //Cases: "string", ‘.’, ‘..’, ‘-’, ‘~’

    if (totalArguments > 2)
    {
        printf(RED "Error: cd cannot take more than 2 arguments!\n");
        return;
    }
    else
    {
        //Case 1: cd        ................    Go to Root
        if (totalArguments == 1)
        {
            //newLine();
            //printf("cd cmd\n");

            //Assigning current Directory as new Prev Directory
            if (getcwd(previousDirectoryHome, MAX_LENGTH) == NULL)
            {
                printf(RESET);
                printf(RED "Error updating prevDir");
                printf(RESET);
            }

            if (chdir(currentDirectoryHome) == -1)
            {
                printf(RESET);
                printf(RED "Error while moving to rootDir");
                printf(RESET);
            }
        }

        if (totalArguments > 1)
        {
            //Case 2: cd .  ----- ??

            //Case 3: cd ..
            if (strcmp(passerParams[1], "..") == 0)
            {
                // newLine();
                //printf("cd .. cmd\n");

                //Check if current directory is the directory in which shell was invoked
                char currDir[MAX_LENGTH] = "";
                if (getcwd(currDir, MAX_LENGTH) == NULL)
                {
                    printf(RED "Error getting current directory");
                }

                char tempPrevDir[MAX_LENGTH];
                strcpy(tempPrevDir, previousDirectoryHome);

                //Assigning current Directory as new Prev Directory
                if (getcwd(previousDirectoryHome, MAX_LENGTH) == NULL)
                {
                    printf(RED "Error updating prevDir");
                }

                if (chdir("..") == -1)
                {
                    printf(RED "Error while moving to parent Dir");
                }
            }

            //Case 4: cd - ---------> Prev Dir
            else if (strcmp(passerParams[1], "-") == 0)
            {
                char moveToDir[MAX_LENGTH] = "";
                strcpy(moveToDir, previousDirectoryHome);
                if (getcwd(previousDirectoryHome, MAX_LENGTH) == NULL)
                {
                    printf(RED "Error updating prevDir");
                }

                if (chdir(moveToDir) == -1)
                {
                    printf(RED "Error while moving to previous Directory");
                }
            }

            //Case 5: cd ~ ---------> Change to root
            else if (strcmp(passerParams[1], "~") == 0)
            {
                //newLine();
                //printf("cd ~ cmd\n");

                //Assigning current Directory as new Prev Directory
                if (getcwd(previousDirectoryHome, MAX_LENGTH) == NULL)
                {
                    printf(RED "Error updating prevDir\n");
                }

                if (chdir(currentDirectoryHome) == -1)
                {
                    printf(RED "Error while moving to rootDir\n");
                }
            }
            //Case 6: cd path
            else
            {
                // printf("%s %s\n", tokensList[0], tokensList[1]);

                char filePath[MAX_LENGTH] = "";
                strcpy(filePath, passerParams[1]);
                if (getcwd(previousDirectoryHome, MAX_LENGTH) == NULL)
                {
                    printf(RED "getcwd Error\n");
                }

                if (chdir(filePath) == -1)
                {
                    printf(RED "chdir Error\n");
                }
            }
        }
    }
}

void executeEchoCommand(char **passerParams, ll totalArguments)
{
    int i;
    for (i = 1; i < totalArguments; i++)
    {
        printf("%s ", passerParams[i]);
    }
    printf("\n");
}

void executePwdCommand()
{
    char temp[MAX_LENGTH];
    getcwd(temp, MAX_LENGTH);
    printf("%s\n", temp);
}

void processCommandsInQueue(char command[])
{

    ll totalArguments;
    char **passerParams = parseAndCountInputs(command, " ", &totalArguments);

    int standardInputCopy = dup(0);
    int standardOutputCopy = dup(1);

    processIOCommands(passerParams, &totalArguments);

    if (passerParams != NULL)
    {
        if (!strcmp(passerParams[0], "cd"))
        {
            if (totalArguments > 2)
            {
                printf(RESET);
                printf(RED "Invalid number of parameters");
                printf(RESET);
            }
            else
                executeCDCommand(passerParams, totalArguments);
        }
        else if (!strcmp(passerParams[0], "history"))
        {
            addHisNodeEnd(passerParams[0]);
            executeHistoryCommand();
        }
        else if (!strcmp(passerParams[0], "echo"))
        {
            executeEchoCommand(passerParams, totalArguments);
        }
        else if (!strcmp(passerParams[0], "pinfo"))
        {
            if (totalArguments == 1)
            {
                pid_t pid = getpid();
                pinfoCommand(pid);
            }
            else if (totalArguments == 2)
            {
                if (!isINT(strlen(passerParams[1]), passerParams[1]))
                {
                    printf(RED "PID not integer\n");
                }
                else
                {
                    pinfoCommand(atoi(passerParams[1]));
                }
            }
            else
                printf(RED "Excess arguments entered!\n");
        }
        else if (!strcmp(passerParams[0], "pwd"))
        {
            executePwdCommand();
        }
        else if (!strcmp(passerParams[0], "discover"))
        {
            discoverCommand(passerParams, totalArguments);
        }
        else if (!strcmp(passerParams[0], "ls"))
        {
            ls(passerParams, totalArguments);
        }
        else if (!strcmp(passerParams[0], "jobs"))
        {
            executeJobsCommand(passerParams, totalArguments);
        }
        else if (!strcmp(passerParams[0], "sig"))
        {
            if (totalArguments < 3)
            {
                printf("Incorrent signal input, please enter a signal after processId\n");
            }
            else
                signalRunningProcess(atoi(passerParams[1]), atoi(passerParams[2]));
        }
        else if (!strcmp(passerParams[0], "fg"))
        {
            if (totalArguments < 2)
            {
                printf("Incorrent fd command, please input processId\n");
            }
            else
                executeFgCommand(atoi(passerParams[1]));
        }
        else if (!strcmp(passerParams[0], "bg"))
        {
            if (totalArguments < 2)
            {
                printf("Incorrent bg command, please input processId\n");
            }
            else
                executeBgCommand(atoi(passerParams[1]));
        }
        else
        {
            if (!strcmp(passerParams[totalArguments - 1], "&"))
            {
                /* For execvp 
                    - arg[0] = "sleep"
                    - arg[1] = "2"
                */

                //Copying without last char
                totalArguments -= 1;
                char **newPasserParams = malloc(totalArguments * sizeof(char *));
                for (ll i = 0; i < totalArguments; i++)
                {
                    newPasserParams[i] = passerParams[i];
                    // printf("%d %s\n", i, newPasserParams[i]);
                }

                newPasserParams[totalArguments] = NULL;
                backgroundProcessExecution(newPasserParams, totalArguments);
            }
            else
            {
                time_t begin = time(NULL);
                foregroundProcessExecution(passerParams, totalArguments);
                time_t end = time(NULL);

                if (end - begin >= 1.0)
                {

                    char currDirectoryInit[MAX_LENGTH];
                    if (getcwd(currDirectoryInit, MAX_LENGTH) != NULL)
                    {
                        char *currDirectory = malloc(MAX_LENGTH);
                        currDirectory = pathMatchUpdate(currentDirectoryHome, currDirectoryInit);

                        printf(BLUE "<%s@%s:" CYAN "%s" WHITE "took %lds>", currentUsername, currentSystemName, currDirectory, end - begin);
                        printf(RESET);
                        // printf("\n");

                        fflush(stdout);
                        //<Name@UBUNTU:~>

                        dontPrintPrompt = true;
                    }
                    else
                    {
                        printf(RED "Error while obtaining directory");
                    }
                }
            }
        }
        if (dup2(standardInputCopy, 0) < 0)
        {
            printf(RESET);
            printf(RED "dup2 Error");
            printf(RESET);
            return;
        }
        close(standardInputCopy);
        if (dup2(standardOutputCopy, 1) < 0)
        {
            printf(RESET);
            printf(RED "dup2 Error");
            printf(RESET);
            return;
        }
        close(standardOutputCopy);

        releaseMemoryAndFree(&passerParams, totalArguments);
    }
}