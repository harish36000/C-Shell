#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/utsname.h>
#include <pwd.h>

//change order
#define MAX_LENGTH 100

//Displaying User Details ---- Spec 1
char userName[MAX_LENGTH];
char systemName[MAX_LENGTH];

//Directory Flow ---- Spec 2 (cd command)
char rootDir[MAX_LENGTH];
char prevDir[MAX_LENGTH];

void newLine()
{
    printf("\n\n");
}
char *pathMatchUpdate(char *rootDir, char *currDir)
{
    //printf("%s\n%s\n", rootDir, currDir);

    char *newPath = malloc(MAX_LENGTH);

    int equal = 1;
    long long int ptr = 0;
    long long int newPtr = 1;

    strcpy(newPath, "~");

    while (ptr < strlen(currDir))
    {
        if (equal)
        {
            if (rootDir[ptr] == currDir[ptr])
                ptr++;
            else
            {
                newPath[newPtr++] = currDir[ptr++];
                equal = 0;
            }
        }
        else
        {
            newPath[newPtr++] = currDir[ptr++];
        }
    }

    newPath[newPtr] = '\0';
    //printf("insdie fn: %s\n", newPath);

    return (char *)newPath;
}
void shellInit()
{
    // printf("Inside shellInit() \n\n");

    /*
        varInit()
        initialize all global variables to "";

        ** IMPORTANT **
    */

    /***** GET USER AND SYSTEM INFO **********/

    char systemNameInit[MAX_LENGTH] = "";
    if (gethostname(systemNameInit, MAX_LENGTH) == 0)
    {
        strcpy(systemName, systemNameInit);

        // printf("SystemName: %s\n", systemName);
    }
    else
    {
        perror("Error obtaining system name!");
    }

    uid_t UserID = getuid();
    char userNameInit[MAX_LENGTH] = "";
    strcpy(userNameInit, strdup(getpwuid(UserID)->pw_name));
    strcpy(userName, userNameInit);
    //printf("UserNameInit: %s\n", userNameInit);

    char currDirectoryInit[MAX_LENGTH];
    if (getcwd(currDirectoryInit, MAX_LENGTH) != NULL)
    {
        char *currDirectory = malloc(MAX_LENGTH);
        currDirectory = pathMatchUpdate(rootDir, currDirectoryInit);

        printf("%s@%s:%s ", userName, systemName, currDirectory);
        fflush(stdout);
        //<Name@UBUNTU:~>
    }
    else
    {
        perror("Error while obtaining directory");
    }

    // newLine();

    // printf("RESULT:\n"); //----------- TAKES TIME, ASYNC -> GOES TO READCOMMAND() ---- fflush helped
    // printf("%s", userName);
    // printf("@");
    // printf("%s", systemName);
    // printf(":~");
    // printf("%s", currDirectory);

    /****************************************************/
}
int parse(char command[MAX_LENGTH], char *splitChar, char tokensList[MAX_LENGTH][MAX_LENGTH])
{
    int cnt = 0;

    //    printf("Inside fn: \n");

    char *token = strtok(command, (splitChar));
    //  printf("%s\n", token);
    long long int tokenCnt = 0;
    while (token != NULL)
    {
        strcpy(tokensList[cnt], token);
        //printf("%s\n", token);
        token = strtok(NULL, (splitChar));
        cnt++;
    }

    return cnt;
}

int isMultipleCmd(long long int total, char tokensList[MAX_LENGTH][MAX_LENGTH])
{
    char *multiList[] = {"&",
                         ";"};

    for (long long int i = 0; i < total; i++)
    {
        for (long long int j = 0; j < 2; j++)
        {
            if (strcmp(tokensList[i], multiList[j]) == 0)
                return 1;
        }
    }

    return 0;
}

void cd(long long int tokenCnt, char tokensList[MAX_LENGTH][MAX_LENGTH])
{

    /***************** CHANGE DIRECTORY ********************/
    //Cases: "string", ‘.’, ‘..’, ‘-’, ‘~’

    if (tokenCnt > 2)
    {
        printf("Error: cd cannot take more than 2 arguments!\n");
        return;
    }
    else
    {
        //Case 1: cd        ................    Go to Root
        if (tokenCnt == 1)
        {
            //newLine();
            //printf("cd cmd\n");

            //Assigning current Directory as new Prev Directory
            if (getcwd(prevDir, MAX_LENGTH) == NULL)
            {
                perror("Error updating prevDir");
            }

            if (chdir(rootDir) == -1)
            {
                perror("Error while moving to rootDir");
            }
        }

        if (tokenCnt > 1)
        {
            //Case 2: cd .  ----- ??

            //Case 3: cd .. //Case 4: cd - ---------> Prev Dir
            //Problem: How to update prev dir
            //prev dir -> prev of prev dir ?
            if (strcmp(tokensList[1], "..") == 0 || strcmp(tokensList[1], "-") == 0)
            {
                // newLine();
                //printf("cd .. cmd\n");

                //Check if current directory is the directory in which shell was invoked
                char currDir[MAX_LENGTH] = "";
                if (getcwd(currDir, MAX_LENGTH) == NULL)
                {
                    perror("Error getting current directory");
                }

                if (strcmp(currDir, rootDir) == 0)
                {
                    //NEED TO FIX THIS
                    ;
                }

                char tempPrevDir[MAX_LENGTH];
                strcpy(tempPrevDir, prevDir);

                //Assigning current Directory as new Prev Directory
                if (getcwd(prevDir, MAX_LENGTH) == NULL)
                {
                    perror("Error updating prevDir");
                }

                if (chdir("..") == -1)
                {
                    perror("Error while moving to parent Dir");
                }
            }
            //Case 5: cd ~ ---------> Change to root
            else if (strcmp(tokensList[1], "~") == 0)
            {
                //newLine();
                //printf("cd ~ cmd\n");

                //Assigning current Directory as new Prev Directory
                if (getcwd(prevDir, MAX_LENGTH) == NULL)
                {
                    perror("Error updating prevDir");
                }

                if (chdir(rootDir) == -1)
                {
                    perror("Error while moving to rootDir");
                }
            }
            //Case 6: cd path
            else
            {
                //printf("%s %s\n", tokensList[0], tokensList[1]);

                char filePath[MAX_LENGTH] = "";
                strcpy(filePath, tokensList[1]);
                if (getcwd(prevDir, MAX_LENGTH) == NULL)
                {
                    perror("Error updating prevDir");
                }

                if (chdir(filePath) == -1)
                {
                    perror("Error while moving to rootDir");
                }
            }
        }
    }
}
void echo(long long int tokenCnt, char tokensList[MAX_LENGTH][MAX_LENGTH])
{
    //print all tokens
    for (long long int it = 1; it < tokenCnt; it++)
        printf("%s ", tokensList[it]);
    printf("\n");
}
void pwd(long long int tokenCnt, char tokensList[MAX_LENGTH][MAX_LENGTH])
{
    char currWorkDir[MAX_LENGTH];
    if (getcwd(currWorkDir, MAX_LENGTH) == NULL)
    {
        perror("Error getting pwd");
    }
    printf("%s\n", currWorkDir);
}
void readCommand()
{

    // printf("READ COMMAND: \n");

    char currRead[2];

    char inputCommand[MAX_LENGTH] = "";
    int inputCommandPtr = 0;

    //printf("Command Read Before Reading: %s\n", inputCommand);

    //read till you get new line char
    while (read(0, currRead, 1) != 0 && currRead[0] != '\n')
    {
        inputCommand[inputCommandPtr++] = currRead[0];
        //printf("CurrRead: %s\n", currRead);
    }

    inputCommand[inputCommandPtr] = '\0';
    //printf("Command Read Before Removing Spaces: %s\n", inputCommand);
    //printf("%ld\n", strlen(inputCommand));

    //remove continuous/unecesary spaces and tabspaces from read Command
    char inputCommandUpdated[MAX_LENGTH];
    long long int j = 0;
    for (long long int i = 0; i < inputCommandPtr; i++)
    {
        if (inputCommand[i] == ' ')
        {
            if (j == 0)
                continue;
            else
            {
                if (inputCommandUpdated[j - 1] == ' ') //last and curr is space
                    ;
                else
                    inputCommandUpdated[j++] = ' ';
            }
        }
        else if (inputCommand[i] == '\t')
        {
            if (j == 0)
                ;
            else if (inputCommandUpdated[j - 1] == ' ') //if previous char is space, no need to allocate another space
            {
                ;
            }
            else //if previous char is not space
            {
                inputCommandUpdated[j++] = ' ';
            }
        }
        else
            inputCommandUpdated[j++] = inputCommand[i];
    }

    inputCommandUpdated[j] = '\0';
    // printf("Command After removing additional spaces and tabs: %s\n", inputCommandUpdated);

    //Get all tokens from inputCommandUpdated

    char tokensList[MAX_LENGTH][MAX_LENGTH];
    long long int tokenCnt = parse(inputCommandUpdated, ";", tokensList);

    //Tokenizing commands first by ;
    //Later for each token ---- divide by " " to extract command and arguments

    for (long long int i = 0; i < tokenCnt; i++)
    {
        executeCommand(tokensList[i]);
    }

    // char *token = strtok(inputCommandUpdated, " ");
    // long long int tokenCnt = 0;
    // while (token != NULL)
    // {
    //     strcpy(tokensList[tokenCnt], token);
    //     token = strtok(NULL, " ");
    //     tokenCnt++;
    // }

    // printf("Tokens Obtained: \n");
    // for (int j = 0; j < tokenCnt; j++)
    //     printf("%s\n", tokensList[j]);

    //Check if multiple commmands need to be runned together?

    if (strcmp(tokensList[0], "cd") == 0)
    {
        cd(tokenCnt, tokensList);
    }
    else if (strcmp(tokensList[0], "echo") == 0)
    {
        echo(tokenCnt, tokensList);
    }
    else if (strcmp(tokensList[0], "pwd") == 0)
    {
        pwd(tokenCnt, tokensList);
    }
    else
    {
        printf("FUNCTION NOT DEFINED!\n");
    }
}

void getRootDirectory()
{
    /**********ASSIGN ROOT DIRECTORY ***********/
    if (getcwd(rootDir, MAX_LENGTH) == NULL)
    {
        perror("Error getting root directory");
    }

    //in the start - we assign root = prev dir
    if (getcwd(prevDir, MAX_LENGTH) == NULL)
    {
        perror("Error getting previous directory");
    }

    // printf("Got root dir as: %s\n", rootDir);

    /******************************************************/
}

void startShell()
{
    getRootDirectory();

    while (1)
    {
        shellInit();
        readCommand();
    }
}

int main()
{
    startShell();
}