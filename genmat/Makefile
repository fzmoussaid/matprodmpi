TARGET=genmat
CFLAGS=-std=gnu99 -g -Wall -Wextra
LDFLAGS=-lm
GENGETOPT=gengetopt
CC=icc

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


genmat: $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	@$(CC) -MM $(CFLAGS) $*.c > $*.d
	$(CC) -c $(CFLAGS) $*.c -o $*.o

clean:
	$(RM) $(OBJ) $(DEP) *.d *.o

mrproper: clean
	$(RM) $(TARGET)

genopt: genmat.ggo
	$(GENGETOPT) < $^

