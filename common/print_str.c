#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "print_str.h"

/**
 * @brief strで指定される文字列を標準出力に出力する。
 *        出力する際、コントロールコードはキャレット記法などに置き換えて出力する。
 * @param str 文字列(ASCIIまたはUTF-8を想定)
 * @retval 成功した場合、書き込んだ文字数(UTF-8だとバイト数)を返す。
 *         失敗した場合、エラー番号をセットして-1を返す。
 */
int print_str(const char *str) { return fprint_str(stdout, str); }

/**
 * @brief fで指定されるファイルストリームにstrで指定される文字列を出力する。
 *        出力する際、コントロールコードはキャレット記法などに置き換えて出力する。
 * @param f ファイルストリーム
 * @param str 文字列(ASCIIまたはUTF-8を想定)
 * @retval 成功した場合、書き込んだ文字数(UTF-8だとバイト数)を返す。
 *          失敗した場合、エラー番号をセットして-1を返す。
 */
int fprint_str(FILE *f, const char *str) {
    int retval = 0L;

    if ((f == NULL) || (str == NULL)) {
        errno = EINVAL;
        retval = -1L;
    } else {

        const char *p = str;

        char utf8_char[12u];
        int32_t utf8_pos = 0L;
        while (*p != '\0') { // 文字列終端でない？
            uint8_t d = *p;
            if (d < 0x80) { // ASCIIコード？
                if (utf8_pos > 0u) {
                    utf8_char[utf8_pos] = '\0';
                    printf("%s", utf8_char);
                    retval += (int)(utf8_pos);
                    utf8_pos = 0u;
                }
                if (d < 0x20u) { // Ctrlコード？
                    switch (d) {
                    case '\t':
                        printf("\\t");
                        break;
                    case '\r':
                        printf("\\r");
                        break;
                    case '\n':
                        printf("\\n");
                        break;
                    default:
                        putchar('^'); // キャレット表記
                        putchar(d + 0x40u);
                        break;
                    }
                    retval += 2L;
                } else {
                    putchar(d);
                    retval += 1L;
                }
            } else {
                if ((d & 0xC0u) == 0xC0u) { // UTF-8先頭？
                    if (utf8_pos > 0u) {
                        utf8_char[utf8_pos] = '\0';
                        printf("%s", utf8_char);
                        retval += (int)(utf8_pos);
                    }
                    utf8_pos = 0u;
                    utf8_char[utf8_pos] = d;
                    utf8_pos++;
                } else {
                    if ((utf8_pos > 0)                              // 2バイト目以降？
                        && (utf8_pos < (sizeof(utf8_char) - 1u))) { // 領域内？
                        utf8_char[utf8_pos] = d;
                    }
                    utf8_pos++;
                }
            }
            p++;
        }
        if (utf8_pos > 0u) {
            utf8_char[utf8_pos] = '\0';
            printf("%s", utf8_char);
            retval += (int)(utf8_pos);
        }
    }

    return retval;
}
