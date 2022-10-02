#include "header.h"

int processInputFile(int curIndex, char **passerParams, ll *totalArguments)
{
    int currentInputFile = open(passerParams[curIndex + 1], O_RDONLY);

    if (dup2(currentInputFile, 0) < 0)
    {

        printf(RESET);
        printf(RED "Error occured during processing Input File \n");
        printf(RESET);

        return 0;
    }
    else
    {
        //Since we are executing 2 params - "<" "fileName" at once, we move the remaining comamnds forward
        while (curIndex < *totalArguments - 1)
        {
            passerParams[curIndex] = passerParams[curIndex + 2];
            curIndex++;
        }

        close(currentInputFile);
        *totalArguments -= 2;

        //successfull execution
        return 1;
    }

    return 0;
}

int processOutputFile(int curIndex, char **passerParams, ll *totalArguments, int append)
{
    int currentOutputFile = append ? open(passerParams[curIndex + 1], O_WRONLY | O_APPEND | O_CREAT, 0644) : open(passerParams[curIndex + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);

    if (dup2(currentOutputFile, 1) > 0)
    {

        while (curIndex < *totalArguments - 1)
        {
            passerParams[curIndex] = passerParams[curIndex + 2];
            curIndex++;
        }

        close(currentOutputFile);
        *totalArguments -= 2;

        return 1;
    }

    return 0;
}

void processIOCommands(char **passerParams, ll *totalArguments)
{

    for (int i = 0; i < *totalArguments; i++)
    {
        int res = 1;

        if (!strcmp(passerParams[i], "<"))
        {
            //if there is no argument following < --- ERROR
            if (i >= *totalArguments - 1)
            {
                printf(RESET);
                printf(RED "File error, no file entered \n");
                printf(RESET);
                return;
            }
            else
            {
                res = processInputFile(i, passerParams, totalArguments);
                i--;
            }
        }

        else if (!strcmp(passerParams[i], ">") || !strcmp(passerParams[i], ">>"))
        {
            int append = strcmp(passerParams[i], ">");
            res = processOutputFile(i, passerParams, totalArguments, append);
            i--;
        }

        if (!res)
        {
            printf(RESET);
            printf(RED "File error, no such file found \n");
            printf(RESET);
            return;
        }
    }
}