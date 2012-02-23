# Makefile for YESS project
# Team Amazonians
# Tyler Davis
# Partner: Caleb Lee

CC 		= gcc
CFLAGS  = -O3
LD 		= $(CC)
LDFLAGS =
RM		= rm

EXE		= yess
SRCS    = decodeStage.c dump.c executeStage.c fetchStage.c loader.c main.c memory.c memoryStage.c registers.c tools.c writebackStage.c
OBJS	= ${SRCS:.c=.o}

.SUFFIXES:

.SUFFIXES: .o .c

.c.o:
	$(CC) $(CFLAGS) -c $<

all: $(EXE)

$(EXE): $(OBJS)
		$(LD) -o $@ $(OBJS)

debug: 

clean:
	-$(RM) -f $(EXE) $(OBJS)

decodeStage.o: decodeStage.h decodeStage.c

dump.o: dump.h dump.c

executeStage.o: executeStage.h executeStage.c

fetchStage.o: types.h fetchStage.h fetchStage.c

loader.o: types.h loader.h loader.c

main.o: types.h tools.h memory.h dump.h main.c

memory.o: memory.h memory.c

memoryStage.o: memoryStage.h memoryStage.c

registers.o: registers.h registers.c

tools.o: types.h tools.h tools.c

writebackStage.o: writebackStage.h writebackStage.c
