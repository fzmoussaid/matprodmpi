TARGET=matprod
CFLAGS=-std=gnu99 -g -Wall -Wextra \
	 $(shell pkg-config --cflags glib-2.0)
LDFLAGS=-lm $(shell pkg-config --libs glib-2.0) 
GENGETOPT=gengetopt
CC=mpiicc

ifdef DEBUG
CFLAGS+=-ggdb -O0 -DDEBUG=1
else
CFLAGS+=-O3
endif

ifeq ($(strip $(MKLROOT)),)
LDFLAGS+=-lopenblas
else
LDFLAGS+= -Wl,--start-group ${MKLROOT}/lib/intel64/libmkl_intel_lp64.a ${MKLROOT}/lib/intel64/libmkl_intel_thread.a ${MKLROOT}/lib/intel64/libmkl_core.a ${MKLROOT}/lib/intel64/libmkl_blacs_intelmpi_lp64.a -Wl,--end-group -liomp5 -lpthread -lm -ldl
CFLAGS+= -I${MKLROOT}/include

endif

SRC=    cmdline.c \
	main.c \
	perf/perf.c \
	error.c \
	input.c \
	util.c \
	matprod-mpi.c \
	fox_mult.c \
	equation.c

OBJ=$(SRC:.c=.o)
DEP=$(SRC:.c=.d)

all: $(TARGET)

-include $(DEP)

matprod: $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	@$(CC) -MM $(CFLAGS) $*.c > $*.d
	$(CC) -c $(CFLAGS) $*.c -o $*.o

clean:
	$(RM) $(OBJ) $(DEP) *.d *.o

mrproper: clean
	$(RM) $(TARGET)

genopt: matprod.ggo
	$(GENGETOPT) -u"MatrixA MatrixB" < $^

