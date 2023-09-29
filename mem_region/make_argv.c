#include "make_argv.h"
#include <string.h>

int32_t make_argv(char *s, char **av, int32_t max_args) {
    if ((s == NULL) || (av == NULL) || (max_args < 0L)) {
        return -1L; // Invalid arguments.
    }

    int32_t arg_count = 0L;
    const char *delim = " \t\r\n";
    char *p = s;
    while (arg_count < max_args) {
        // Skip first delimiters.
        while ((*p != '\0') && (strchr(delim, *p) != NULL)) {
            p++;
        }
        if (*p == '\0') {
            break;
        }
        char *begin = p;

        if (arg_count == (max_args - 1L)) {
            av[arg_count] = begin;
        } else if ((*p == '\"') || (*p == '\'')) {
            char tail = *p;
            while ((*p != '\0') && (*p != tail)) {
                p++;
            }
            if (*p == '\0') {
                av[arg_count] = begin;
            } else {
                *p = '\0';
                p++;
                av[arg_count] = begin + 1;
            }
        } else {
            while ((*p != '\0') && (strchr(delim, *p) == NULL)) {
                p++;
            }
            if (*p != '\0') {
                *p = '\0';
                p++;
            }
            av[arg_count] = begin;
        }
        arg_count++;
    }

    return arg_count;
}
