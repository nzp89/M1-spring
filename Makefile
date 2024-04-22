# Makefile for compiling aeif_1neuron.c and aeif.c

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGETS = aeif_1neuron aeif
SRCS = aeif_1neuron.c aeif.c
OBJS = $(SRCS:.c=.o)
LIBS = -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGETS)

aeif_1neuron: aeif_1neuron.o
	$(CC) $(CFLAGS) aeif_1neuron.o -o aeif_1neuron $(LIBS)

aeif: aeif.o
	$(CC) $(CFLAGS) aeif.o -o aeif $(LIBS)

clean:
	rm -f $(OBJS) $(TARGETS)

run_aeif_1neuron: aeif_1neuron
	./aeif_1neuron

run_aeif: aeif
	./aeif

.PHONY: all clean run_aeif_1neuron run_aeif
