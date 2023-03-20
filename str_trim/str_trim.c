#include "str_trim.h"
#include <stddef.h>
#include <string.h>

/**
 * @brief sで指定される文字列の前後にある、スペース、タブ、改行コードを除いて返す。
 * @note
 * str_trim_with(s, " \t\r\n")のラッパー
 *
 * @param s 文字列(関数呼び出しでデータは破壊される)
 * @retval 前後のスペース、タブ、改行コードが取り除かれた文字列
 * @retval NULL 引数が不正な場合
 */
char *str_trim(char *s) { return str_trim_with(s, " \t\r\n"); }

/**
 * @brief sで指定される文字列の前後にある、delimを除外した文字列を返す。
 *
 * @param s 文字列(関数呼び出しでデータは破壊される)
 * @param charset 文字列セット
 * @retval 前後のcharsetで指定される文字が取り除かれた文字列
 * @retval NULL 引数が不正な場合
 */
char *str_trim_with(char *s, const char *charset) {
    char *retval;

    if ((s == NULL) || (charset == NULL)) {
        retval = NULL;
    } else {
        char *p = s;
        while ((*p != '\0')                      // 文字列の終端でない？
               && (strchr(charset, *p) != NULL)) { // 文字はデリミタに含まれる？
            *p = '\0';
            p++;
        }
        retval = p;
        size_t len = strlen(retval);
        while ((len > 0u)                                      // 文字列の終端でない？
               && (strchr(charset, retval[len - 1u]) != NULL)) { // 文字はデリミタに含まれる？
            retval[len - 1u] = '\0';
            len--;
        }
    }

    return retval;
}
