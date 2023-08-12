#include <stddef.h>
#include <string.h>
#include "io_vector.h"

/**
 * @brief Initialize io vector list.
 *
 * @param list I/O vector list object.
 * @param vectors Vector entries of list.
 * @param count Count of vectors.
 */
void io_vector_list_init(struct io_vector_list *list, const struct io_vector *vectors, uint32_t count)
{
    list->vectors = vectors;
    list->count = count;
    return ;
}

/**
 * @brief Destory I/O vector list object.
 *
 * @param list I/O vector list object.
 */
void io_vector_list_destroy(struct io_vector_list *list)
{
    list->vectors = NULL;
    list->count = 0u;
    return ;
}

/**
 * @brief Read from I/O vector list.
 *
 * @param list I/O vector list object.
 * @param bufp Buffer to store read data.
 * @param bufsize Size of bufp.
 * @return On succcess, return read byte count.
 *         If list has no data, return 0.
 */
int32_t io_vector_list_read(const struct io_vector_list *list, uint32_t offset, void *bufp, uint32_t bufsize)
{
    uint8_t *wp = (uint8_t*)(bufp);
    uint32_t left = bufsize;
    uint32_t read_offs = offset;
    for (uint32_t n = 0; n < list->count; n++)
    {
	if (read_offs < list->vectors[n].length)
	{
	    uint32_t readable_len = list->vectors[n].length - read_offs;
	    uint32_t copy_len = (readable_len < left) ? readable_len : left;
	    memcpy(wp, (const uint8_t*)(list->vectors[n].addr) + read_offs, copy_len);
	    wp += copy_len;
	    left -= copy_len;
	    read_offs = 0;
	    if (left == 0u)
	    {
		break;
	    }
	}
	else
	{
	    read_offs -= list->vectors[n].length;
	}

    }
    return (int32_t)(bufsize - left);
}
