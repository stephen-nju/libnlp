/*
 * @Author: zhubin
 * @Date: 2023-01-19 17:39:38
 * @LastEditTime: 2023-01-30 16:44:53
 * @FilePath: \libnlp\src\tokenizer.c
 * @Description:
 *
 * Copyright (c) 2023 by zhubin, All Rights Reserved.
 */
#include "tokenizer.h"

#include "map.h"
#include "readutils.h"
#include "strutils.h"

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

struct bert_tokenizer
{
    bool do_lower_case;
    hashmap *vocab;
    hashmap *inv_vocab;
};


void nlp_basic_tokenizer_destory(basic_tokenizer_t *tokenizer) { free(tokenizer); }
bert_tokenizer_t *nlp_bert_tokenizer_create(const char *vocab_path, bool do_lower_case) {
    bert_tokenizer_t *tokenizer;
    tokenizer = (bert_tokenizer_t *)malloc(sizeof(bert_tokenizer_t));
    hashmap *vocab = hashmap_create();
    FILE *vocab_file = fopen(vocab_path, "r");
    if (vocab_file != NULL) {
        char *rawline;
        nlp_size_t index = 0;
        nlp_size_t len = 1024;
        rawline = (char *)malloc(sizeof(char) * len);
        while (readline(&rawline, &len, vocab_file)) {
            // nlp_size_t l = utf8str_split(rawline, "\t",);
            printf("%s", rawline);
        }
    } else {
        fclose(vocab_file);
        exit(1);
    }


    return tokenizer;
}
