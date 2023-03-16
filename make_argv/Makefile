CC = clang
SRCS = test_make_argv.c make_argv.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -g -Wall
LDFLAGS = -g
TARGET = make_argv.exe

.PHONY : all clean

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) *.o



