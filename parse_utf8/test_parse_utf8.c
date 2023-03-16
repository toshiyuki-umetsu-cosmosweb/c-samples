#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_utf8.h"

static const char *TestStrings[] = {
    "This is ascii only string.\n",
    "これはＵＴＦ－８文字列。ソースコードの文字コードそのまま\n",
};

static uint32_t TestStringCount = sizeof(TestStrings) / sizeof(const char *);

static void on_utf8_char(void *arg, const char *pchar, size_t data_len);

int main(int argc, char **argv) {
    int retval = EXIT_SUCCESS;
    if (argc > 1L) {
        for (int i = 1L; i < argc; i++) {
            if (!parse_utf8(argv[i], on_utf8_char, NULL)) {
                retval = EXIT_FAILURE;
            }
        }
    } else {
        for (uint32_t i = 0u; i < TestStringCount; i++) {
            if (!parse_utf8(TestStrings[i], on_utf8_char, NULL)) {
                retval = EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}

static void on_utf8_char(void *arg, const char *pchar, size_t data_len) { fprintf(stdout, "%s", pchar); }
