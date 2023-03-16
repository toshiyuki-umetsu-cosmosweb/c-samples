#include "parse_utf8.h"
#include <stdint.h>

/**
 * @brief UTF-8文字列データを解析する。
 *
 * @param str UTF-8 文字列(ヌル文字終端すること)
 * @param callback strから文字を読み取ったときに呼び出すコールバック関数
 * @param arg callbackに渡す引数(NULL可)
 * @retval true 解析に成功した場合
 * @retval false 解析に失敗した場合
 */
bool parse_utf8(const char *str, utf8_parser_callback_t callback, void *arg) {
    bool retval;
    if ((str == NULL) || (callback == NULL)) {
        retval = false;
    } else {
        const char *p = str;

        char char_data[8u]; // UTF-8の規格上4バイトまでしかありえないが、8バイトの領域としておく。
        int32_t pos = 0u;
        while (*p != '\0') { // 文字列終端でない？
            uint8_t d = *p;

            if ((d < 0x80u)                  // ASCIIコード？
                || ((d & 0xC0u) == 0xC0u)) { // UTF8開始コード
                if (pos > 0u) {
                    char_data[pos] = '\0';
                    callback(arg, char_data, pos);
                    pos = 0u;
                }
                char_data[pos] = d;
                pos = 1u;
            } else {
                if ((pos > 0u)                             // 2バイト目以降？
                    && (pos < (sizeof(char_data) - 1u))) { // 領域内？
                    char_data[pos] = d;
                }
                pos++;
            }
            p++;
        }
        if (pos > 0u) {
            char_data[pos] = '\0';
            callback(arg, char_data, pos);
        }
        retval = true;
    }

    return retval;
}
