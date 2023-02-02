#include "msgqueue.h"
#include "strutils.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
int main()
{
    char s[100] = "dce中国";
    char *dst = " 子啊abc 0再啊";
    // printf("%d", strlen(dst));
    nlp_uint8_t *o = utf8str_cat(s, dst);
    printf("%s", o);
    free(o);
    return 0;
}
