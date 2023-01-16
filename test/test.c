#include "greatest.h"

SUITE_EXTERN(libnlp_strutils_tests);

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(libnlp_strutils_tests);
    GREATEST_MAIN_END();
}