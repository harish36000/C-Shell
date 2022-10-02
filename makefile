CC=gcc
CFLAGS=-I.
DEPS = header.h
OBJ = main.c discover.c executeCommands.c fgAndBg.c historyMgmt.c ls.c pinfo.c utilsFun.c exposeBufferlessTerminal.c userDefinedCommands.c ioCommands.c autoComplete.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

final: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) 