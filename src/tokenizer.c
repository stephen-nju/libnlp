/*
 * @Author: zhubin
 * @Date: 2023-01-19 17:39:38
 * @LastEditTime: 2023-01-29 16:23:34
 * @FilePath: \libnlp\src\tokenizer.c
 * @Description:
 *
 * Copyright (c) 2023 by zhubin, All Rights Reserved.
 */
#include "tokenizer.h"

#include <stdlib.h>
struct basic_tokenizer
{
    bool do_lower_case;
};

basic_tokenizer_t *nlp_basic_tokenizer_create(bool do_lower_case) {
    basic_tokenizer_t *tokenizer;
    tokenizer = (basic_tokenizer_t *)malloc(sizeof(basic_tokenizer_t));
    tokenizer->do_lower_case = do_lower_case;
    return tokenizer;
}

void nlp_basic_tokenizer_destory(basic_tokenizer_t *tokenizer) { free(tokenizer); }