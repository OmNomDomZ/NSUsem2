#ifndef GLOBAL
#define GLOBAL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

extern FILE *in;
extern FILE *out;

typedef enum
{
    SUCCESS,
    FAILURE
} ExitCodes;

#endif
