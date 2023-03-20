CC = clang
SRCS = test_str_trim.c str_trim.c ../common/print_str.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -g -Wall
LDFLAGS = -g
TARGET = str_trim.exe

.PHONY : all clean

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS) *.o



