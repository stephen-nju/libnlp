#include "greatest.h"
#include "strutils.h"

SUITE(libnlp_strutils_tests);

TEST test_libnlp(void)
{
    printf("success");
    PASS();
}
SUITE(libnlp_strutils_tests) { RUN_TEST(test_libnlp); }