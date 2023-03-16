CC = clang
SRCS = test_parse_utf8.c parse_utf8.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -g -Wall
LDFLAGS = -g
TARGET = parse_utf8.exe

.PHONY : all clean

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) *.o



