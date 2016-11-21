TARGET=matprod_seq
CFLAGS=-std=gnu99 -g -Wall -Wextra $(shell pkg-config --cflags glib-2.0)
LDFLAGS=-lm -l$(BLASLIB) $(shell pkg-config --libs glib-2.0) 
GENGETOPT=gengetopt
CC=mpicc

ifdef DEBUG
CFLAGS+=-ggdb -O0 -DDEBUG=1
else
CFLAGS+=-O3
endif

ifeq ($(strip $(BLASLIB)),)
BLASLIB=openblas
endif

SRC=    ../cmdline.c \
	main.c \
	../perf/perf.c \
	../error.c \
	../input.c \
	../util.c \
	../fox_mult.c \
	../matprod-mpi.c

OBJ=$(SRC:.c=.o)
DEP=$(SRC:.c=.d)

all: $(TARGET)

-include $(DEP)

matprod_seq: $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	@$(CC) -MM $(CFLAGS) $*.c > $*.d
	$(CC) -c $(CFLAGS) $*.c -o $*.o

clean:
	$(RM) $(TARGET) $(OBJ) $(DEP) *.d *.o

mrproper: clean
	$(RM) $(TARGET)
