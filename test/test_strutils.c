#include "greatest.h"
#include "strutils.h"
#include "utf8proc.h"
#include <stdio.h>

SUITE(libnlp_strutils_tests);

TEST test_libnlp(void)
{
    const nlp_uint8_t *s="abcde中国";
    // bool ret=utf8str_chr(s,42);
    // printf("%d",ret);
    PASS();
}
SUITE(libnlp_strutils_tests) { RUN_TEST(test_libnlp); }