#ifndef READ_UTILS_H
#define READ_UTILS_H

#include "common.h"
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

LIBNLP_DLLEXPORT size_t readline(char **__restrict line, size_t *__restrict len, FILE *__restrict fp);
#ifdef __cplusplus
}
#endif
#endif