#include "header.h"

void seachWithFileName(char temp[MAX_LENGTH], char *path)
{
    struct dirent *de; // Pointer for directory entry

    DIR *dr = opendir(path);

    if (dr == NULL)
    {
        printf(RED "Could not open current directory\n");
        return;
    }

    while ((de = readdir(dr)) != NULL)
    {
        if (de->d_type == IS_FILE)
        {
            if (!strcmp(de->d_name, temp))
            {
                // printf(" Found => %s \n", de->d_name);
                char *currDir = malloc(MAX_LENGTH * sizeof(char));
                getcwd(currDir, MAX_LENGTH);

                if (currDir == NULL)
                    printf(RED "Error getting cwd\n");

                //getcwd returns dir from which discover is invoked !!
                char *printDir = pathMatchUpdate(currentDirectoryHome, path);

                if (strlen(printDir) > 0)
                    printDir[0] = '.';

                printf("%s/%s\n", printDir, de->d_name);
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

                seachWithFileName(temp, d_path); // rec call with the new path
            }
        }
    }

    closedir(dr);
}

void printFilesAndDirectories(int file, int dir, char *path)
{

    struct dirent *de;

    //pointer to the dir stream
    DIR *dr = opendir(path);

    if (dr == NULL)
    {
        printf(RED "Could not open current directory\n");
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

                printf("%s/%s\n", printDir, de->d_name);
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
                    printf("%s\n", printDir);

                printFilesAndDirectories(file, dir, d_path); // recurive call with the new path
            }
        }
    }

    closedir(dr);
}

void discoverCommand(char **passerParams, int totalArguments)
{
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    //1 argument = discover -> so print all dir and file of current working dir
    if (totalArguments == 1)
    {
        printf(".\n");
        printFilesAndDirectories(1, 1, cwd);
        return;
    }
    bool toSearch = false;
    /*
    Cases 

    1. printAll
        -d
        -f
        -d and -f
        (no flag)
    2. search
        - "fileName" must be present
        - if dir present -> "." or ".." or "~" or "./abc/abc1"
        - else -> "."
    */

    for (ll i = 0; i < totalArguments; i++)
    {
        if (strlen(passerParams[i]))
        {
            if (passerParams[i][0] == 34) //ASCII for "
            {
                toSearch = true;
                // printf("to search\n");
                break;
            }
        }
    }

    if (toSearch)
    {
        bool pathFlag = false;

        char *fileName = malloc(sizeof(char) * MAX_LENGTH);
        char *pathName = malloc(sizeof(char) * MAX_LENGTH);

        for (ll i = 0; i < totalArguments; i++)
        {
            if (strlen(passerParams[i]))
            {
                if (passerParams[i][0] == 34) //ASCII for "
                {
                    ll j = 1;
                    ll fileNamePtr = 0;
                    while (j < strlen(passerParams[i]) && passerParams[i][j] != 34) //till next "
                    {
                        fileName[fileNamePtr++] = passerParams[i][j];
                        j++;
                    }

                    break;
                }
            }
        }

        // printf("Filename: %s\n", fileName);

        for (ll i = 0; i < totalArguments; i++)
        {
            if (strlen(passerParams[i]))
            {
                if (passerParams[i][0] == '.') //ASCII for "
                {
                    strcpy(pathName, passerParams[i]);
                    pathFlag = true;
                }
            }
        }

        if (pathFlag)
        {
            if (strcmp(pathName, "~") == 0)
            {
                seachWithFileName(fileName, currentDirectoryHome);
            }
            else if (strcmp(pathName, "..") == 0)
            {
                seachWithFileName(fileName, previousDirectoryHome);
            }
            else
                seachWithFileName(fileName, pathName);
        }
        else
        {
            seachWithFileName(fileName, currentDirectoryHome);
        }
    }
    else
    {
        bool dFlag = false;
        bool fFlag = false;
        bool pathFlag = false;
        char *pathName = malloc(sizeof(char) * MAX_LENGTH);

        //index = 0 --> "discover"
        for (ll i = 1; i < totalArguments; i++)
        {
            if (strcmp(passerParams[i], "-d") == 0)
            {
                dFlag = true;
            }
            else if (strcmp(passerParams[i], "-f") == 0)
            {
                fFlag = true;
            }
            else
            {
                strcpy(pathName, passerParams[i]);
                pathFlag = true;
            }
        }

        // printf("path name: %s\n", pathName);
        if (pathFlag)
        {
            if (strcmp(pathName, "~") == 0)
            {
                printFilesAndDirectories(fFlag, dFlag, currentDirectoryHome);
            }
            else if (strcmp(pathName, "..") == 0)
            {
                printFilesAndDirectories(fFlag, dFlag, previousDirectoryHome);
            }
            else
                printFilesAndDirectories(fFlag, dFlag, pathName);
        }
        else
            printFilesAndDirectories(fFlag, dFlag, ".");
    }
}
