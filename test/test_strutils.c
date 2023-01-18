#include "greatest.h"
#include "strutils.h"
#include "utf8proc.h"
#include <stdio.h>
#include <stdlib.h>

SUITE(libnlp_strutils_tests);

TEST test_libnlp(void)
{
    const nlp_uint8_t *s="abcde中国";
    nlp_int32_t *dst=(nlp_int32_t *) malloc(sizeof(nlp_int32_t));
    nlp_size_t d=utf8str_iterate("中",-1,dst);
    printf("%zu",d);
    nlp_uint8_t* ret=utf8str_chr(s,*dst);
    printf("%s",ret);
    PASS();
}
SUITE(libnlp_strutils_tests) { RUN_TEST(test_libnlp); }