#ifndef __LIB_DEBUG_H
#define __LIB_DEBUG_H
#include <stdio.h>


#define ASSERT(CONDITION)                                       \
        if (CONDITION) { } else {                               \
                printf("assertion `%s' failed.", #CONDITION);   \
        }


#endif /* lib/debug.h */		