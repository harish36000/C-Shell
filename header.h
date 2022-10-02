#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <termios.h>

#define MAX_LENGTH 256
#define ll long long int

#define IS_FILE 8
#define IS_DIR 4

#define HISTORY_MAX_SIZE 20
#define MAX_PROCESS_COUNT 1000

#define RED "\e[1;91m"
#define GREEN "\e[1;32m"
#define BLUE "\e[1;94m"
#define CYAN "\e[1;95m"
#define RESET "\e[0;0m"
#define WHITE "\e[1;97m"
#define YELLOW "\e[0;93m"

void shellInit();
bool isINT(ll length, char *s);

void executeCDCommand(char **passerParams, ll totalArguments);
void executeEchoCommand(char **passerParams, ll totalArguments);
void executePwdCommand();

bool regularFile(struct stat st);
char getCharFileType(mode_t m);
ll getBLKSIZE(bool Aflag, DIR *dir);
void printDateData(struct stat st);
void printInfo(bool dirFlag, char *path, char *name);
void lsExecute(bool Lflag, bool Aflag, char *dir, ll totalDir);
void ls(char **passerParams, ll totalArguments);

void processCommandsInQueue(char command[]);

void backgroundProcessExecution(char **passerParams, ll totalArguments);
void foregroundProcessExecution(char **passerParams, ll totalArguments);
void pinfoCommand(int pid);

void discoverCommand(char **passerParams, int totalArguments);
void printFilesAndDirectories(int file, int dir, char *path);
void seachWithFileName(char temp[MAX_LENGTH], char *path);

void executeHistoryCommand();
void addHisNode(ll index, char *command);
void addHisNodeEnd(char *command);
void fetchHistory();

void clearTerminos(const char *s);
void disableBufferlessTerminal();
void exposeBufferlessTerminal();

//utility functions
void processInputAndRemoveSpaces(char string[]);
char *combination(char **passerParams, ll n);
void backspace(char temp_string[], ll *position);
char *pathMatchUpdate(char *rootDir, char *currDir);
char **parseAndCountInputs(char string[], char delim[], ll *totalArguments);
void releaseMemoryAndFree(char ***mem, ll no);
void listFormAndCheck(char *path, char *name, ll df);
time_t getStartTime(pid_t pid);

typedef struct Process
{
    char *name;
    int jobNo;
    pid_t pid;
    time_t startTime;

} Process;

void backgroundNodeAddition(pid_t pid, char *processName);
void foregroundNodeAddition(pid_t pid, char *processName);
Process findBGProcessName(pid_t pid);
Process findFGProcessName(pid_t pid);
bool isProcessFG(pid_t pid, pid_t pgpid);
bool isProcessBG(pid_t pid);
void printAllProcessPINFO();
char getProcessStatus(pid_t pid);

void processIOCommands(char **passerParams, ll *totalArguments);
bool isCmdNonRepetitive(char *cmd, char *ref);
void dropOldestCmd();
void addHisNodeEnd(char *command);
void addHisNode(ll index, char *command);
void sortBgNodes();

void autoCompleteSelection(char inputCommand[MAX_LENGTH], int inputCommandPtr);
void executeJobsCommand(char **passerParams, ll totalArguments);
void signalRunningProcess(int processNo, int signal);
void executeFgCommand(int processNo);
void executeBgCommand(int processNo);

// Global Variables

extern char currentUsername[MAX_LENGTH];
extern char currentSystemName[MAX_LENGTH];
extern char currentDirectoryHome[MAX_LENGTH];
extern char previousDirectoryHome[MAX_LENGTH];
extern int droppedProcesses[MAX_LENGTH];

extern int foregroundProcess;
extern int parent;

extern Process bgProcess[MAX_PROCESS_COUNT];
extern Process fgProcess[MAX_PROCESS_COUNT];

extern ll totalBGProcess;
extern ll totalBGProcessHistory;
extern int droppedProcessesCounter;

extern char *historyTracker[HISTORY_MAX_SIZE + 1];
extern bool dontPrintPrompt;

#endif