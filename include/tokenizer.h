/*
 * @Author: zhubin
 * @Date: 2023-01-19 17:39:24
 * @LastEditTime: 2023-01-29 17:19:05
 * @FilePath: \libnlp\include\tokenizer.h
 * @Description:
 *
 * Copyright (c) 2023 by zhubin, All Rights Reserved.
 */
#ifndef TOKENIZER_H
#define TOKENIZER_H
#include "common.h"

#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

extern int tokenizer_version;
typedef struct basic_tokenizer basic_tokenizer_t;
basic_tokenizer_t *nlp_basic_tokenizer_create(bool do_lower_case);

void nlp_basic_tokenizer_destory(basic_tokenizer_t *tokenizer);

#ifdef __cplusplus
}
#endif
#endif// tokenizers header
