# README

## Assignment 3

A basic user defined interactive shell program that can  create and manage new processes.



## Functions Provided

1. Provides a prompt with username, systemname and current working directory
2. Supports basic commands like 
   - `cd`
   - `echo`
   - `pwd`
3. Supports `ls` command with -l and -a flag
4. Can run process in foreground and background
5. Supports printing information regarding various running processes - `pinfo` command
6. Finished background processes are displayed with
   - process Name
   - proccess ID
   - time taken to run
7. Supports an equivalent of find command, implemented as `discover` command
8. Supports printing most recent 10 commands, implemented as `history`
9. Implemented Input/Output Redirection using `<`, `>` and `>>`
10. Implemented piping of commands
11. Implemented user defined commands like
    - `jobs`
    - `sig`
    - `fg`
    - `bg`
12. Implemented signal handling that handles keyboard interrupts like Ctrl + C, Ctrl + Z and Ctrl + D
13. Implemented Autocomplete feature that autocompletes file and directory name when tab key is hit.

## Assumptions

1. Max length of input command and path  is 256 bytes
2. Maximum number of history commands stored at a time is 20.
   Out of which, 10 most recent commands will be printed
3. Maximum number of processes that can run at a time is 1000
4. After ith job is completed, new job will be assigned i+1. No new job will be assigned job number as i again.



## File Structure

- README.md

- makefile - for compiling all file

- discover.c 

  contains code for executing `discover` command and helper functions like seachWithFileName() and printFilesAndDirectories

- executeCommands.c 

  contains code for executing the input command and for 

  - `cd`
  - `echo`
  - `pwd` commands

- fgAndBg.c

  Contains code for creating and tracking foreground and background processes

- header.h

  Contains various header files needed for execution

- historyMgmt.c

  Contains code for fetching session history and tracking history of user commands

- ls.c

  Contains code and helper functions for executing `ls` command

- main.c

  Contains code for taking user instructions and printing basic details - userName, current working directory

- pinfo.c

  Contains code for printing process information

- utilsFun.c

  Contains code for string functions used for  parsing input, compressing path, removing character etc

- exposeBufferlessTerminal.c

  Contains code for moving the terminal to raw mode. 
  
- autoComplete.c

  - Contains code for autoComplete function and all necessary functions needed for autcomplete feature

- iocommands.c

  - Contains code for processing input and output files

- userDefinedCommands.c

  - Contains code for executing jobs, sig, fg and bg command

