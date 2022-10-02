# README

## Assignment 2

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

## Assumptions

1. Max length of input command and path  is 256 bytes
2. Maximum number of history commands stored at a time is 20.
   Out of which, 10 most recent commands will be printed
3. Maximum number of processes that can run at a time is 1000



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

