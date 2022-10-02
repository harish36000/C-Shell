#include "header.h"
int cur = 0, dirty = 0;
char predict[MAX_LENGTH], finalResult[MAX_LENGTH];

void printFilesAndDirectoriesForAC(int file, int dir, char *path)
{

    struct dirent *de;

    DIR *dr = opendir(path);

    if (dr == NULL)
    {
        return;
    }

    while ((de = readdir(dr)) != NULL)
    {
        /*
            4 - file
            8 -  directory
        */
        if (de->d_type == IS_FILE)
        {
            if (file)
            {
                char *currDir = malloc(MAX_LENGTH * sizeof(char));
                getcwd(currDir, MAX_LENGTH);

                if (currDir == NULL)
                    printf(RED "Error getting cwd\n");

                //getcwd returns dir from which discover is invoked !!
                char *printDir = pathMatchUpdate(currentDirectoryHome, path);

                if (strlen(printDir) > 0)
                    printDir[0] = '.';

                printf("%s\n", de->d_name);
            }
        }

        if (de->d_type == DT_DIR)
        {
            if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
            {
                char d_path[255];

                strcpy(d_path, path);
                strcat(d_path, "/");
                strcat(d_path, de->d_name);

                //for printing
                //when u are in abc1
                char *printDir = pathMatchUpdate(currentDirectoryHome, path); // ~/abc
                if (strlen(printDir) > 0)
                    printDir[0] = '.'; //./abc

                strcat(printDir, "/");
                strcat(printDir, de->d_name);

                if (dir)
                    printf("%s\\\n", de->d_name);

                printFilesAndDirectoriesForAC(file, dir, d_path); // recurive call with the new path
            }
        }
    }

    closedir(dr);
}

int searchFileOrDir(char temp[MAX_LENGTH], char *path)
{

    struct dirent *de; //pointer to dir entry
    DIR *dr = opendir(path);

    //Directory doesnt exist
    if (dr == NULL)
        return 0;

    while ((de = readdir(dr)) != NULL)
    {
        if (de->d_type == IS_FILE)
        {
            if (strcmp(de->d_name, temp) == 0)
            {
                return 1;
            }
        }

        if (de->d_type == IS_DIR)
        {
            if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
            {
                if (strcmp(de->d_name, temp) == 0)
                    return 2;

                char d_path[255];

                strcpy(d_path, path);
                strcat(d_path, "/");
                strcat(d_path, de->d_name);

                seachWithFileName(temp, d_path); // rec call with the new path
            }
        }
    }

    closedir(dr);
    return 0;
}

int checkAndFind(char temp[MAX_LENGTH], char dName[256], ll len)
{
    int bes = 0;

    for (ll i = 0; i < len; i++)
    {
        if (dName[bes] == temp[bes])
            bes++;
        else
            return bes;
    }

    return bes;
}

int searchFileOrDirReg(char temp[MAX_LENGTH], char *path, ll len, int flag)
{

    struct dirent *de;

    DIR *dr = opendir(path);

    if (dr == NULL)
        return 0;

    while ((de = readdir(dr)) != NULL)
    {
        if (de->d_type == IS_FILE)
        {
            if (checkAndFind(temp, de->d_name, len) >= len)
            {

                if (flag)
                {
                    strcpy(finalResult, de->d_name);
                    return 1;
                }

                char findDir[MAX_LENGTH];
                strcpy(findDir, de->d_name);

                if (!dirty)
                {
                    ll i;
                    dirty = 1;
                    for (i = len; i < strlen(findDir); + i++)
                        predict[i - len] = findDir[i];

                    cur = i - len;
                }
                else if (cur)
                {
                    ll i;

                    for (i = len; i < strlen(findDir); + i++)
                        if (predict[i - len] != findDir[i])
                            break;

                    cur = i - len;
                }

                printf("./%s\n", de->d_name);
            }
        }

        if (de->d_type == DT_DIR)
        {
            if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
            {
                if (checkAndFind(temp, de->d_name, len) >= len)
                {

                    if (flag)
                    {
                        strcpy(finalResult, de->d_name);
                        return 0;
                    }

                    char findDir[MAX_LENGTH];
                    strcpy(findDir, de->d_name);

                    if (!dirty)
                    {
                        ll i;
                        dirty = 1;
                        for (i = len; i < strlen(findDir); + i++)
                            predict[i - len] = findDir[i];

                        cur = i - len;
                    }
                    else if (cur)
                    {
                        ll i;

                        for (i = len; i < strlen(findDir); + i++)
                            if (predict[i - len] != findDir[i])
                                break;

                        cur = i - len;
                    }

                    printf("./%s\n", de->d_name);
                }

                char d_path[255];

                strcpy(d_path, path);
                strcat(d_path, "/");
                strcat(d_path, de->d_name);

                seachWithFileName(temp, d_path); // rec call with the new path
            }
        }
    }

    closedir(dr);
    return 0;
}

int getCount(char temp[MAX_LENGTH], char *path, ll len)
{

    struct dirent *de;

    DIR *dr = opendir(path);

    int ret = 0;

    if (dr == NULL)
        return 0;

    while ((de = readdir(dr)) != NULL)
    {
        if (de->d_type == IS_FILE)
        {
            if (checkAndFind(temp, de->d_name, len) >= len)
            {
                ret++;
                if (ret > 1)
                    return ret;
            }
        }

        if (de->d_type == DT_DIR)
        {
            if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
            {
                if (checkAndFind(temp, de->d_name, len) >= len)
                {
                    ret++;
                    if (ret > 1)
                        return ret;
                }

                char d_path[255];

                strcpy(d_path, path);
                strcat(d_path, "/");
                strcat(d_path, de->d_name);

                seachWithFileName(temp, d_path); // rec call with the new path
            }
        }
    }

    closedir(dr);
    return ret;
}

void autoCompleteSelection(char inputCommand[MAX_LENGTH], int inputCommandPtr)
{
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    //No File Name --- Print all files and directories
    if (!inputCommandPtr)
    {
        printFilesAndDirectoriesForAC(1, 1, cwd);

        shellInit();
        dontPrintPrompt = true;
    }
    else
    {

        char inputCopy[MAX_LENGTH];
        strcpy(inputCopy, inputCommand);
        char *token = strtok(inputCommand, " ");
        char *last = NULL;

        while (token != NULL)
        {
            last = token;
            token = strtok(NULL, " ");
        }

        /*
            last --- file/dir name to be autcompleted
        */

        char findDir[MAX_LENGTH];
        strcpy(findDir, last);

        int res = searchFileOrDir(findDir, cwd);

        if (res == 0)
        {

            int count = getCount(findDir, cwd, strlen(findDir));
            if (count > 1)
            {
                searchFileOrDirReg(findDir, cwd, strlen(findDir), 0);
                shellInit();
                char *token_3 = strtok(inputCopy, " ");
                char *last_3 = NULL;

                while (token_3 != NULL)
                {
                    last_3 = token_3;
                    token_3 = strtok(NULL, " ");
                    if (last_3 && token_3)
                        printf("%s ", last_3);
                }

                printf("%s", last_3);

                for (int i = 0; i < cur; i++)
                    printf("%c", predict[i]);
            }
            else if (count == 1)
            {
                shellInit();

                char *token_3 = strtok(inputCopy, " ");
                char *last_3 = NULL;

                while (token_3 != NULL)
                {
                    last_3 = token_3;
                    token_3 = strtok(NULL, " ");
                    if (last_3 && token_3)
                        printf("%s ", last_3);
                }

                int var = searchFileOrDirReg(findDir, cwd, strlen(findDir), 1);
                printf("%s ", finalResult);
            }
            else
            {
                shellInit();
                printf("%s", inputCopy);
            }
        }
    }
}