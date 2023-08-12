#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "io_vector.h"


int main(int ac, char **av)
{
    struct io_vector vectors[32];
    struct io_vector_list list;

    int count = ((ac - 1) <= 32) ? (ac - 1) : 32;
    for (int i = 0; i < count; i++) {
	vectors[i].addr = av[i + 1];
	vectors[i].length = strlen(av[i + 1]);
    }

    io_vector_list_init(&list, vectors, count);

    char buf[4];
    uint32_t offset = 0;

    for (;;) {
	int32_t read_len = io_vector_list_read(&list, offset, buf, (uint32_t)(sizeof(buf)));
	if (read_len <= 0L)
	{
	    break;
	}
	else
	{
	    for (int32_t i = 0; i < read_len; i++)
	    {
		fputc(buf[i], stdout);
	    }
	    fputc('\n', stdout);
	}

	offset += (uint32_t)(read_len);
    }

    io_vector_list_destroy(&list);

    return 0;
}
