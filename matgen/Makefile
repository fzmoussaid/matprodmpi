TARGET=matgen
CFLAGS=-std=gnu99 -g -Wall -Wextra
LDFLAGS=-lm
GENGETOPT=gengetopt
CC=gcc

ifdef DEBUG
CFLAGS+=-ggdb -O0 -DDEBUG=1
else
CFLAGS+=-O3
endif

SRC=    cmdline.c \
	main.c \
	../util.c \

OBJ=$(SRC:.c=.o)
DEP=$(SRC:.c=.d)

all: $(TARGET)

-include $(DEP)


matgen: $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	@$(CC) -MM $(CFLAGS) $*.c > $*.d
	$(CC) -c $(CFLAGS) $*.c -o $*.o

clean:
	$(RM) $(TARGET) $(OBJ) $(DEP) *.d *.o

mrproper: clean
	$(RM) $(TARGET)

genopt: matgen.ggo
	$(GENGETOPT) < $^

