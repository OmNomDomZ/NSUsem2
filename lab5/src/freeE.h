#ifndef FREE_E
#define FREE_E

#include "global.h"

#define uc unsigned char

typedef struct ETree
{
    uc val;
    int32_t frequency;
    struct ETree *left;
    struct ETree *right;
} EncodeTree_t;

typedef struct Node
{
    uc symbol;
    int32_t frequency;
} Node_t;

void freeEncode(EncodeTree_t *root, Node_t *Nodes, int *ASCII, char *curCode, char **newCodes);

#endif
