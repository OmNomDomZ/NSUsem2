#ifndef GLOBAL
#define GLOBAL

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

typedef enum
{
    SUCCESS,
    BAD_NUM_OF_VER,
    BAD_NUM_OF_EDG,
    BAD_VER,
    BAD_LENGTH,
    BAD_NUM_OF_LINES,
    NO_PATH,
    OVERFLOW,
    FAILURE
}ExitCodes;

extern char *ExitMessage[];

#endif
