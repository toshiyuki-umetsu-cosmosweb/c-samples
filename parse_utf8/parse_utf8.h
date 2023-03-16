#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef void (*utf8_parser_callback_t)(void *arg, const char *pchar, size_t data_len);

#ifdef __cplusplus
extern "C" {
#endif

bool parse_utf8(const char *str, utf8_parser_callback_t callback, void *arg);

#ifdef __cplusplus
}
#endif
