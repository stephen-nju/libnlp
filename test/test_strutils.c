/*
 * @Author: zhubin
 * @Date: 2023-01-13 16:25:58
 * @LastEditTime: 2023-01-31 11:45:23
 * @FilePath: \libnlp\test\test_strutils.c
 * @Descript
 *
 * Copyright (c) 2023 by zhubin, All Rights Reserved.
 */
#include "common.h"
#include "greatest.h"
#include "strutils.h"
#include "utf8proc.h"

#include <stdio.h>
#include <stdlib.h>

SUITE(libnlp_strutils_tests);

TEST test_utf8str_split(void) {
    const nlp_uint8_t *s = "abacad";
    const nlp_uint8_t *d = "a";
    // nlp_int32_t *dst = (nlp_int32_t *)malloc(sizeof(nlp_int32_t));
    // nlp_size_t d = utf8str_iterate("中", -1, dst);
    // printf("%zu", d);
    // nlp_uint8_t** dst=(nlp_uint8_t**)malloc(sizeof(nlp_uint8_t));
    nlp_int32_t len = -1;
    nlp_uint8_t **ret = utf8str_split(s, d, &len);
    printf("%d\n", len);
    // printf("%s\n", ret[0]);
    PASS();
}

TEST test_utf8str_rstrip(void) {
    const nlp_uint8_t *s = "adc dca  \n";
    nlp_uint8_t *ret = utf8str_rstrip(s);
    printf("%s", (char *)ret);

    PASS();
}

SUITE(libnlp_strutils_tests) {
    RUN_TEST(test_utf8str_split);
    RUN_TEST(test_utf8str_rstrip);
}