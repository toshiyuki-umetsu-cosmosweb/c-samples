#include <stddef.h>
#include <string.h>
#include "make_argv.h"

/**
 * @desc str をデリミタで区切り、argvで指定される配列にトークンを格納する。
 * strは最大で(max_args - 1)で指定される数だけ分割される。
 * デリミタはスペース、タブ、CR、LFを対象として実装している。
 * 配列の最後はNULLを格納する。
 *
 * @param str 文字列。ヌル文字('\0')終端されたもの。
 *            本関数に渡すと領域データは破壊される。
 * @param argv 分割したトークンを格納する配列
 * @param max_args argvの配列に格納可能な最大数
 *                 配列の最後にNULLを入れるので、予測される最大分割数+1の値が必要。
 * @retval 分割されたトークン数
 */
int32_t make_argv(char *str, char **argv, int32_t max_args) {
    if ((str != NULL) && (argv == NULL) && (max_args <= 0L)) {
        return -1L;
    }

    char *p = str;
    int32_t args = 0L;
    const char *delim = " \t\r\n"; // トークン区切り文字として、スペース,
                                   // タブ、CR, LFを対象とする。

    while ((*p != '\0') // 文字列の終端でない？
	    && ((args + 1u) < max_args)) { // トークン配列に空きがある？
        while ((*p != '\0')                      // 文字列の終端でない？
               && (strchr(delim, *p) != NULL)) { // この文字はデリミタに含まれる？
            p++;
        }
        if (*p == '\0') { // 文字列の終端？
            break;        // 文字列の最後まで区切り文字が続いた
        }

        if ((*p == '\'') || (*p == '\"')) { // シングルクォートまたはダブルクォート？
            char *end = strchr(p + 1u, *p); // 次の文字の位置から、同じ文字が出る位置を取得
            if (end != NULL) {              // 同じ文字があった？
                char *token = p + 1u;
                argv[args] = token;
                args++;
                *end = '\0';
            } else {
                // Note : 文字列終端までクローズする文字がないので、
                //        文字列終端までをトークンとして取り出す。
                char *token = p;
                argv[args] = token;
                args++;
                p = token + strlen(token); // 結果的に p は文字列終端コードを指す。
            }
        } else {
            char *token = p;
            while ((*p != '\0')                      // 文字列の終端でない？
                   && (strchr(delim, *p) == NULL)) { // デリミタに該当する文字でない？
                p++;
            }
            if (*p != '\0') {
                *p = '\0';
                p++;
            }
            argv[args] = token;
            args++;
        }
    }

    argv[args] = NULL; // 最後のトークンはNULLを入れる。

    return args;
}
