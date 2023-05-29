#ifndef FREE_D
#define FREE_D

#include <stdlib.h>

#define uc unsigned char

typedef struct DTree
{
    uc val;
    struct DTree *left;
    struct DTree *right;
} DecodeTree_t;

void freeDecode(DecodeTree_t *root);

#endif
