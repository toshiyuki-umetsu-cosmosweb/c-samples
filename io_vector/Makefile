SRCS = io_vector.c main.c
OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -ggdb -O0

.PHONY : all clean

all: vectest

vectest : $(OBJS)
	$(CC) -o $@ $(OBJS)

%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f vectest $(OBJS)


