#include "header.h"

bool regularFile(struct stat st)
{
    if (S_ISREG(st.st_mode))
        return true;

    return false;
}

ll getBLKSIZE(bool Aflag, DIR *dir)
{
    struct dirent *it;
    ll cnt = 0;
    struct stat st;
    while ((it = readdir(dir)) != NULL)
    {
        if (it->d_name[0] == '.' && !Aflag)
            continue;
        lstat(it->d_name, &st);
        cnt += st.st_blocks;
    }

    return cnt;
}

char getCharFileType(mode_t m)
{
    char ans;
    if (S_ISDIR(m))
    {
        ans = 'd';
    }
    else if (S_ISREG(m))
    {
        ans = '-';
    }
    else if (S_ISLNK(m))
    {
        ans = 'l';
    }

    return ans;
}

void printDateData(struct stat st)
{
    char d[21];
    if (time(NULL) - st.st_mtime < 15780000)
        strftime(d, 20, "%b %d %H:%M", localtime(&(st.st_mtime)));
    else
        strftime(d, 20, "%b %d  %Y", localtime(&(st.st_mtime)));
    printf(" %s ", d);
}

void printInfo(bool dirFlag, char *path, char *name)
{
    struct stat st;
    char temp[MAX_LENGTH] = "";
    strcpy(temp, path);
    if (dirFlag)
    {
        if (temp[strlen(temp) - 1] != '/')
            strcat(temp, "/");
        strcat(temp, name);
    }

    lstat(temp, &st);
    char ch = getCharFileType(st.st_mode);
    printf("%c", ch);

    //Checking perms available for curr
    int perms[] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};

    for (ll i = 0; i < 9; i++)
    {
        int res = perms[i] & st.st_mode;
        if (res)
        {
            if (i % 3 == 0)
                printf("r");
            else if (i % 3 == 1)
                printf("w");
            else
                printf("x");
        }
        else
            printf("-");
    }

    printf(" %3.ld ", st.st_nlink);
    printf(" %s ", getpwuid(st.st_uid)->pw_name);
    printf(" %s ", getgrgid(st.st_gid)->gr_name);
    printf(" %6.ld ", st.st_size);

    printDateData(st);

    // printf(" %d ", dirFlag);

    /*
        Color Code
            Yello for files
            Green for dir

    */

    if (dirFlag)
    {
        printf(RESET);
        printf(GREEN "%s", name);
        printf(RESET);
    }
    else
    {
        printf(RESET);
        printf(YELLOW "%s", name);
        printf(RESET);
    }
    printf("\n");
}

void lsExecute(bool Lflag, bool Aflag, char *dir, ll totalDir)
{
    DIR *directory = opendir(dir);

    //File or not -> !opendir(dir) -> FILE
    //regularFile (stat)

    struct stat st;
    lstat(dir, &st);

    if (regularFile(st)) //file
    {
        printf("File\n");

        if (Lflag && totalDir)
        {
            printf("%s:", dir);
            printInfo(0, dir, dir);
        }
        else //print only fileName
        {
            // printf("No LFlag1 \n");
            printf("%s\n", dir);
        }
    }
    else
    {
        ll cnt = getBLKSIZE(Aflag, directory);

        if (totalDir)
            printf("%s:", dir);
        if (Lflag)
            printf("total %lld\n", cnt / 2);

        closedir(directory);

        DIR *directory1 = opendir(dir);
        struct dirent *it, **namelist;

        int i = 0, n = scandir(".", &namelist, NULL, alphasort);

        while (i < n)
        {

            char tempArray[500];

            strncpy(tempArray, namelist[i]->d_name, 500);

            if (tempArray[0] != '.' || Aflag)
            {

                if (Lflag)
                {
                    struct stat t;
                    lstat(namelist[i]->d_name, &t);

                    if (regularFile(t))
                    {
                        printInfo(0, dir, namelist[i]->d_name);
                    }
                    else
                    {
                        printInfo(1, dir, namelist[i]->d_name);
                    }
                }

                else
                {
                    // printf("No LFlag2 \n");
                    printf("%s\n", namelist[i]->d_name);
                }
            }

            i++;
        }

        free(namelist);
    }
}

void ls(char **passerParams, ll totalArguments)
{
    //Look which all flags preseent
    bool isAFlag = false;
    bool isLFlag = false;
    char flagString[4][4] = {"-l",
                             "-a",
                             "-la",
                             "-al"};
    ll dirCount = 0;
    char dirList[MAX_LENGTH][MAX_LENGTH];

    //i = 0 --> ls
    for (ll i = 1; i < totalArguments; i++)
    {
        bool isFlag = false;
        for (ll j = 0; j < 4; j++)
        {
            if (strcmp(flagString[j], passerParams[i]) == 0)
            {
                if (j > 1)
                {
                    isAFlag = true;
                    isLFlag = true;
                }
                else if (j == 1)
                    isAFlag = true;
                else
                    isLFlag = true;

                isFlag = true;
            }
        }

        if (!isFlag)
        {
            strcpy(dirList[dirCount++], passerParams[i]);
        }
    }

    //Replace all ~ with rootDir
    for (ll i = 0; i < totalArguments; i++)
    {
        // printf("%ld\n", i);
        if (strcmp("~", passerParams[i]) == 0)
        {
            strcpy(passerParams[i], currentDirectoryHome);
        }
    }

    // printf("Direcount: %ld\n", dirCount);

    //ls , ls -l, ls -a
    //no dir -> consider cwd
    if (dirCount == 0)
    {
        // printf("pass with curr");
        char currDir[MAX_LENGTH];
        if (getcwd(currDir, MAX_LENGTH) == NULL)
            printf(RED "Error getting current direcotry\n");

        lsExecute(isLFlag, isAFlag, currDir, dirCount);
    }
    // go directory by directory and print necc info based on flag
    for (ll i = 0; i < dirCount; i++)
    {
        // printf("%lld\t %s\n", i, dirList[i]);
        lsExecute(isLFlag, isAFlag, dirList[i], dirCount);
    }
}