/*
 * @Author: zhubin
 * @Date: 2023-01-13 16:25:58
 * @LastEditTime: 2023-01-19 16:49:39
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

TEST test_libnlp(void) {
    const nlp_uint8_t *s = "abcde中国人在北京";
    const nlp_uint8_t *d = "中国人";
    // nlp_int32_t *dst = (nlp_int32_t *)malloc(sizeof(nlp_int32_t));
    // nlp_size_t d = utf8str_iterate("中", -1, dst);
    // printf("%zu", d);
    nlp_uint8_t** dst=(nlp_uint8_t**)malloc(sizeof(nlp_uint8_t));
    nlp_uint8_t *ret = utf8str_split(s, d,dst);
    printf("%s", ret);
    PASS();
}
SUITE(libnlp_strutils_tests) { RUN_TEST(test_libnlp); }