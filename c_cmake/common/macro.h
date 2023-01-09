#ifndef _COMMON_MACRO_H_
#define _COMMON_MACRO_H_

#include <stdio.h>

#define CHECK_RET(exp) do { \
    if (exp) { \
        fprintf(stderr, "%s:%d: ret code non-zero\n", __FILE__, __LINE__); \
    } \
} while(0);

#endif
