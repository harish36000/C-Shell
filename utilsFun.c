#include "header.h"

bool isINT(ll length, char *s)
{
    ll i = 0;
    while (i < length)
    {
        if (!isdigit(s[i]))
            return false;
        i++;
    }

    return true;
}

void processInputAndRemoveSpaces(char string[])
{
    ll n = strlen(string) + 1;
    char newString[n];

    ll newPtr = 0;
    ll oldPtr = 0;

    for (ll i = 0; string[i] != '\0'; i++)
    {
        //replace tabs with spaces
        if (string[i] == '\t')
            string[i] = ' ';
    }

    for (ll oldPtr = 0; string[oldPtr] != '\0';)
    {
        //ignore consecutive sapces
        while (string[oldPtr] == ' ' && string[oldPtr + 1] == ' ')
            oldPtr++;
        newString[newPtr++] = string[oldPtr++];
    }

    newString[newPtr] = '\0';
    strcpy(string, newString);
}
void removeCharacter(char *str, char charToRemmove)
{
    int i, j;
    int len = strlen(str);
    for (i = 0; i < len; i++)
    {
        if (str[i] == charToRemmove)
        {
            for (j = i; j < len; j++)
            {
                str[j] = str[j + 1];
            }
            len--;
            i--;
        }
    }
}
char *combination(char **passerParams, ll n)
{
    char temp[MAX_LENGTH] = "";
    for (int i = 0; i < n; i++)
    {
        strcat(temp, passerParams[i]);
        strcat(temp, " ");
    }
    temp[strlen(temp) - 1] = '\0';
    return strdup(temp);
}

char *pathMatchUpdate(char *rootDir, char *currDir)
{
    //printf("%s\n%s\n", rootDir, currDir);
    char *newPath = malloc(MAX_LENGTH);
    if (strlen(currDir) < strlen(rootDir))
    {
        strcpy(newPath, currDir);
        return newPath;
    }

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
    // printf("insdie fn: %s\n", newPath);

    return (char *)newPath;
}
char **parseAndCountInputs(char string[], char delim[], ll *totalArguments)
{
    char *extractor, *currentPointer, temp[MAX_LENGTH], *helper;
    int i = 0, flag = 0;
    strcpy(temp, string);
    *totalArguments = 0;
    for (extractor = strtok_r(temp, delim, &currentPointer); extractor != NULL; extractor = strtok_r(NULL, delim, &currentPointer))
    {
        for (int i = 0; i < strlen(extractor); i++)
            if (extractor[i] == '&')
            {
                flag = 1;
            }
        (*totalArguments)++;
    }

    strcpy(temp, string);

    char **extractors = (char **)calloc((*totalArguments + 1), sizeof(char *));

    extractor = string;

    for (extractor = strtok_r(temp, delim, &currentPointer); extractor != NULL; extractor = strtok_r(NULL, delim, &currentPointer))
    {
        extractors[i] = (char *)calloc(strlen(extractor) + 2, sizeof(char));

        for (int i = 0; i < strlen(extractor); i++)
            if (extractor[i] == '&')
            {
                flag = 1;
            }

        strcpy(extractors[i], extractor);
        i++;
    }

    extractors[*totalArguments] = NULL;

    return extractors;
}

void releaseMemoryAndFree(char ***mem, ll index)
{
    for (int i = index; i >= 0; i--)
        free((*mem)[i]);

    free(*mem);
}