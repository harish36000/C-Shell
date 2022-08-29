#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100

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

int main()
{
    char cmd[MAX_LENGTH] = "cd\t..";
    printf("%s\n", cmd);

    char cmd1[MAX_LENGTH], cmd2[MAX_LENGTH];
    strcpy(cmd1, cmd);
    strcpy(cmd2, cmd);

    char tokensList1[MAX_LENGTH][MAX_LENGTH];
    char tokensList2[MAX_LENGTH][MAX_LENGTH];

    int cnt1 = parse(cmd1, " ", tokensList1);
    int cnt2 = parse(cmd2, ";", tokensList2);

    printf("Parsed By \n");
    for (int i = 0; i < cnt1; i++)
        printf("%s %ld\n", tokensList1[i], strlen(tokensList1[i]));

    printf("Parsed By ; \n");
    for (int i = 0; i < cnt2; i++)
        printf("%s %ld\n", tokensList2[i], strlen(tokensList2[i]));
}
