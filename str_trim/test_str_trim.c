#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../common/print_str.h"
#include "str_trim.h"

/**
 * テスト文字列
 */
static const char *TestStrings[] = {
    "no-trimdata-string",
    "  head-trimdata-string",
    "tail-trimdata-string    ",
    "    both-trimdata-string    ",
    "  char-code-test \t\r\n",
    "  ユニコードテスト　 　",
    "  wahaha    wahahaa   ",
};
/**
 * テスト文字列の数
 */
static uint32_t TestStringCount = sizeof(TestStrings) / sizeof(const char *);

static bool do_test_proc(const char *test_str);

/**
 * @brief アプリケーションのエントリポイント
 *
 * @param argc 引数の数
 * @param argv 引数配列
 */
int main(int argc, char **argv) {
    int retval = EXIT_SUCCESS;

    if (argc > 1L) {
        for (int32_t i = 1L; i < argc; i++) {
            if (!do_test_proc(argv[i])) {
                retval = EXIT_FAILURE;
            }
        }
    } else {
        for (uint32_t i = 0u; i < TestStringCount; i++) {
            if (!do_test_proc(TestStrings[i])) {
                retval = EXIT_FAILURE;
            }
        }
    }

    return retval;
}
/**
 * @brief テストを実行する。
 * 
 * @param test_str テスト文字列
 * @retval true - 成功した場合
 * @retval false - 失敗した場合
 */
static bool do_test_proc(const char *test_str) {
    bool retval;
    
    size_t len = strlen(test_str);
    char *buf = (char*)(malloc(len + 1u));
    if (buf == NULL) {
        retval = false;
    } else {
	strncpy(buf, test_str, len);
	buf[len] = '\0';

        const char *result_str = str_trim(buf);
	putchar('\"'); 
	print_str(test_str);
	printf("\" --(str_trim)-> \"");
	if (result_str != NULL) {
	    print_str(result_str);
	} else {
	    printf("fail");
	}
	printf("\"\n");
        free(buf);
        
        retval = true;
    }

    return retval;
}
