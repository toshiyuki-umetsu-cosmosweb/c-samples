CFLAGS += -Wall
TEST_MEM_REGION_SRCS = test_mem_region.c \
                       mem_region_list.c \
		       make_argv.c
TEST_MEM_REGION_OBJS = $(TEST_MEM_REGION_SRCS:.c=.o)
OBJS += $(TEST_MEM_REGION_OBJS)
TARGETS = test_mem_region

all : $(TARGETS)


test_mem_region : $(TEST_MEM_REGION_OBJS) 
	$(CC) $(LDFLAGS) -o $@ $^

%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS) $(OBJS) 

