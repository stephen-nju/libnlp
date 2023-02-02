#include "greatest.h"
#include "tokenizer.h"

#include <stdio.h>
#include <stdlib.h>
SUITE(libnlp_tokenizer_tests);

TEST test_tokenizer_create(void) {
    char *vocab_path = "E:\\Resources\\bert_model\\vocab.txt";
    bert_tokenizer_t *tokenier = nlp_bert_tokenizer_create(vocab_path, true);
}

SUITE(libnlp_tokenizer_tests) { RUN_TEST(test_tokenizer_create); }