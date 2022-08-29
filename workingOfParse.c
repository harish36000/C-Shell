#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/utsname.h>
#include <pwd.h>

char **parse(char string[], char delim[], int *no_of_parts)
{
    char *part, *saveptr;
    char temp[1024];
    int i = 0;
    strcpy(temp, string);
    *no_of_parts = 0;
    for (part = strtok_r(temp, delim, &saveptr); part != NULL; part = strtok_r(NULL, delim, &saveptr))
    {
        (*no_of_parts)++;
    }

    strcpy(temp, string);

    char **parts = (char **)calloc((*no_of_parts + 1), sizeof(char *));

    // walk through the tokenized buffer again
    part = string;

    for (part = strtok_r(temp, delim, &saveptr); part != NULL; part = strtok_r(NULL, delim, &saveptr))
    {
        parts[i] = (char *)calloc(strlen(part) + 2, sizeof(char));
        strcpy(parts[i], part);
        i++;
    }
    parts[*no_of_parts] = NULL;
    return parts;
}

int main()
{
    char str[] = "Hi hello bye";

    // Returns first token
    char *token = strtok(str, " ");

    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (token != NULL)
    {
        printf(" %s\n", token);
        token = strtok(NULL, " ");
    }
}