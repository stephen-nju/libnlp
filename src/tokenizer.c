/*
 * @Author: zhubin
 * @Date: 2023-01-19 17:39:38
 * @LastEditTime: 2023-02-02 11:28:13
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
};


void nlp_basic_tokenizer_destory(basic_tokenizer_t *tokenizer) { free(tokenizer); }
bert_tokenizer_t *nlp_bert_tokenizer_create(const char *vocab_path, bool do_lower_case) {
    bert_tokenizer_t *tokenizer;
    tokenizer = (bert_tokenizer_t *)malloc(sizeof(bert_tokenizer_t));
    if (tokenizer == NULL) return NULL;
    map_handle_t *vocab = map_create();
    FILE *vocab_file = fopen(vocab_path, "r");
    if (vocab_file != NULL) {
        char *rawline;
        nlp_size_t index = 0;
        nlp_size_t len = 1024;
        rawline = (char *)malloc(sizeof(char) * len);
        while (readline(&rawline, &len, vocab_file)) {
            // hashmap_set(vocab, rawline, strlen(rawline), index);
            // hashmap_set(invert_vocab, index, sizeof(index), rawline);
            index++;
            printf("%s\n", rawline);
        }
        // tokenizer->vocab = vocab;
        // tokenizer->inv_vocab = invert_vocab;
    } else {
        fclose(vocab_file);
        exit(1);
    }


    return tokenizer;
}
