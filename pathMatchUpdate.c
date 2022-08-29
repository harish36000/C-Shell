#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 100

char *pathMatchDisplay(char *rootDir, char *currDir)
{
    printf("%s\n%s\n", rootDir, currDir);

    char *newPath = malloc(N);

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

    //printf("insdie fn: %s\n", newPath);

    return (char *)newPath;
}

int main()
{
    char rootDir[N] = "";
    char currDir[N] = "";

    strcpy(rootDir, "/home/harish/Desktop/on/Assignments/A2");
    strcpy(currDir, "/home/harish/Desktop/on/Assignments/A2/abc");

    char *printDir = malloc(N);
    strcpy(printDir, pathMatchDisplay(rootDir, currDir));
    printf("%s\n", printDir);
}