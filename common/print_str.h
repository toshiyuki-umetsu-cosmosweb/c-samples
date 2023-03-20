#pragma once
/* @file 文字列を印字するための関数。
 * 改行などのコントロールコードを置き換えて出力する。
 */
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int print_str(const char *str);
int fprint_str(FILE *f, const char *str);

#ifdef __cplusplus
}
#endif