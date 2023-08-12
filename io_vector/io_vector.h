#ifndef IO_VECTOR_H
#define IO_VECTOR_H

#include <stdint.h>

struct io_vector {
    void *addr; // Address
    uint32_t length; // Length
};

struct io_vector_list {
    const struct io_vector *vectors; // Vector
    uint32_t count; // Vector count.
};

void io_vector_list_init(struct io_vector_list *list, const struct io_vector *vectors, uint32_t count);
void io_vector_list_destroy(struct io_vector_list *list);

int32_t io_vector_list_read(const struct io_vector_list *list, uint32_t offset, void *bufp, uint32_t bufsize);


#endif // IOVECTOR_H
