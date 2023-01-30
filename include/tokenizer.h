/*
 * @Author: zhubin
 * @Date: 2023-01-19 17:39:24
 * @LastEditTime: 2023-01-30 14:48:56
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

void nlp_basic_tokenizer_edestory(basic_tokenizer_t *tokenizer);

typedef struct bert_tokenizer bert_tokenizer_t;
bert_tokenizer_t *nlp_bert_tokenizer_create(const char *vocab_path, bool do_lower_case);

void nlp_bert_tokenizer_destory(bert_tokenizer_t *tokenzier);

#ifdef __cplusplus
}
#endif
#endif// tokenizers header
