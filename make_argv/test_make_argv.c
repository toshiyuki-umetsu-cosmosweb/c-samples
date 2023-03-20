#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/print_str.h"
#include "make_argv.h"

#define MAX_TOKENS (8u)

/**
 * 組み込みテストコード
 */
static const char *TestStrings[] = {
    "no-seperated:hogehoge",
    "two-tokens:token1 token2",
    "delimiter-check:1 2\t3\r4\n5",
    "check-max-token-count: 1 2 3 4 5 6 7 8 9 10 11 12",
    "check-quotation \"1 2\" 3 '4 5 6' 7 '8 9 10 11 12\"",
    "   has   meny           spaces.",
    "'single quotation strings.\'",
    "not-closed-quotation: 1 \"2 3 4 5 6\n",
    "UTF-8文字列: これは ソースコードの  文字コードに   依存してしまう　ので　よろしくない\n",
    "こういう 🐇 ソースコードは　危険 じゃないだろうか。\n",
    "このような、\t文字コードを  変えるだけで   正常に動かなくなる  ソフトを書いては   いけません。\n"};
static const uint32_t TestStringCount = sizeof(TestStrings) / sizeof(const char *);

static bool do_test(const char *test_str);

/**
 * @brief アプリケーションのエントリポイント
 * 引数を指定された場合には、指定された引数をトークン分割して標準出力に出力する。
 * 引数を指定されない場合には、テスト文字列に対してトークン分割して標準出力に出力する。
 *
 * @param argc 引数の数
 * @param argv 引数配列
 * @retval 0 - 成功
 * @retval 1 - 失敗
 */
int main(int argc, char **argv) {
    int retval = EXIT_SUCCESS;

    if (argc > 1L) {
        for (int i = 1L; i < argc; i++) {
            if (!do_test(argv[i])) {
                retval = EXIT_FAILURE;
            }
        }
    } else {
        for (uint32_t i = 0u; i < TestStringCount; i++) {
            if (!do_test(TestStrings[i])) {
                retval = EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}

/**
 * 文字列の分割テストを実行する。
 *
 * @param test_str テスト文字列
 * @retval true 成功した場合
 * @retval false エラーが発生した場合
 */
static bool do_test(const char *test_str) {
    bool retval;

    size_t len = strlen(test_str);
    char *buf = (char *)(malloc(len + 1u)); // 終端ヌル文字領域を含めてバッファを確保
    if (buf == NULL) {
        fprintf(stderr, "Could not allocate memory. %s\n", strerror(errno));
        retval = false;
    } else {
        strncpy(buf, test_str, len + 1u);
        buf[len] = '\0';

        char *tokens[MAX_TOKENS];

        int32_t token_count = make_argv(buf, tokens, MAX_TOKENS);
        if (token_count >= 0L) {
            putchar('\"');
            print_str(test_str);
            putchar('\"');
            putchar('\n');
	    printf("  token_count = %d\n", token_count);
            for (int32_t i = 0L; i < token_count; i++) {
                printf("  token[%d]=\"", i);
                print_str(tokens[i]);
                printf("\"\n");
            }
            retval = true;
        } else {
            retval = false;
        }
        free(buf);
    }
    return retval;
}

