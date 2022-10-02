#include "header.h"

bool isCmdNonRepetitive(char *cmd, char *ref)
{
    return strcmp(cmd, ref);
}

void dropOldestCmd()
{
    for (ll i = 0; i < HISTORY_MAX_SIZE - 1; i++)
    {
        //delete ith
        free(historyTracker[i]);

        //take (i+1)th
        historyTracker[i] = malloc(strlen(historyTracker[i + 1]) * sizeof(char));
        strcpy(historyTracker[i], historyTracker[i + 1]);
    }

    free(historyTracker[HISTORY_MAX_SIZE - 1]);
}

void addHisNodeEnd(char *command)
{
    ll lastCommand = -1;
    for (ll i = 0; i < MAX_LENGTH; i++)
    {
        if (historyTracker[i] != NULL)
        {
            lastCommand = i;
        }
        else
            break;
    }

    if (lastCommand == -1)
    {
        // printf("first cmd to file: \n", command);
        addHisNode(0, command);

        //WRITE TO FILE !!!
        FILE *fptr;
        fptr = fopen("/tmp/shellHistoryCommand4", "a");

        fprintf(fptr, "%s", command);
        fprintf(fptr, "%s", "\n");
        fclose(fptr);

        return;
    }

    //valid cmd
    // printf("last cmd: %lld\n", lastCommand);
    if (isCmdNonRepetitive(command, historyTracker[lastCommand]))
    {
        //queue is full
        if (lastCommand >= HISTORY_MAX_SIZE - 1)
        {
            dropOldestCmd();
            addHisNode(19, command);
        }
        else
        {
            // printf("to add at %lld\n", lastCommand + 1);
            addHisNode(lastCommand + 1, command);
        }
        //WRITE TO FILE !!!
        FILE *fptr;
        fptr = fopen("/tmp/shellHistoryCommand4", "a");

        if (!fptr)
        {
            printf(RED "File open fails\n");
            return;
        }

        fprintf(fptr, "%s", command);
        fprintf(fptr, "%s", "\n");
        fclose(fptr);
    }
}

void addHisNode(ll index, char *command)
{
    historyTracker[index] = malloc(strlen(command) * sizeof(char));
    strcpy(historyTracker[index], command);

    // printf("added: %s\n", historyTracker[index]);
}

void fetchHistory()
{
    FILE *fp = fopen("/tmp/shellHistoryCommand4", "r");

    if (fp == NULL)
    {
        fp = fopen("/tmp/shellHistoryCommand4", "rb+");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    //Calculate total lines

    ll totalLines = 0;
    if (fp != NULL)
    {
        while ((read = getline(&line, &len, fp)) != -1)
        {
            if (read > 0)
            {
                totalLines += 1;
                // printf("%d %s\n", totalLines++, line);
            }
        }
    }

    //Set ptr to start
    fseek(fp, 0, SEEK_SET);

    //Insert commands that fall into recent 20 category into array
    ll threshold = 0;

    if (totalLines > HISTORY_MAX_SIZE)
    {
        threshold = totalLines - HISTORY_MAX_SIZE;

        ll i = 0;
        ll j = 0;

        if (fp != NULL)
        {
            while ((read = getline(&line, &len, fp)) != -1)
            {
                if (read > 0)
                {
                    if (line[read - 1] == '\n')
                    {
                        line[read - 1] = '\0';
                    }

                    i++;
                    // printf("%d %s\n", i++, line);
                    if (i > threshold)
                    {
                        addHisNode(j, line);
                        // printf("%d %s\n", j, line);
                        j++;
                    }
                }
            }
        }
    }
    else if (totalLines == 0)
    {
        printf("NO HISTORY\n");

        fclose(fp);
        if (line != NULL)
            free(line);
        return;
    }
    else if (totalLines <= HISTORY_MAX_SIZE)
    {
        ll i = 0;
        ll j = 0;

        if (fp != NULL)
        {
            while ((read = getline(&line, &len, fp)) != -1)
            {
                if (read > 0)
                {
                    if (line[read - 1] == '\n')
                    {
                        line[read - 1] = '\0';
                    }

                    i++;
                    // printf("%d %s\n", i++, line);
                    addHisNode(j, line);
                    // printf("%d %s\n", j, line);
                    j++;
                }
            }
        }
    }
    fclose(fp);
    if (line != NULL)
    {
        free(line);
    }
}

void executeHistoryCommand()
{
    //print only 10
    ll inQueue = -1;
    for (ll i = 0; i < HISTORY_MAX_SIZE; i++)
    {
        if (historyTracker[i] != NULL)
            inQueue = i;
    }

    if (inQueue == -1)
    {
        printf(RED "No History Found So Far\n");
    }

    if (inQueue < 10)
    {
        for (ll i = 0; i <= inQueue; i++)
        {
            printf("%s\n", historyTracker[i]);
        }
    }
    else if (inQueue > 10)
    {
        //inQueue = 19
        //print from  10 11 12 13 14 15 16 17 18 19

        ll start = inQueue - 10 + 1;
        for (ll i = 0; i < 10; i++)
        {
            printf("%s\n", historyTracker[start++]);
            // printf("%lld\n", start);
        }
    }
}