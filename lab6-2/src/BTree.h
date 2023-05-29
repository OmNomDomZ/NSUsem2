#ifndef BTREE
#define BTREE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node
{
    int *keys;
    struct Node **children;
    int numOfKeys;
    bool isLeaf;
}Node_t;

typedef struct BTree
{
    Node_t *node;
    int degree;
    int height;
}BTree_t;

void InsertInTree(BTree_t *tree, int cur);
void DeleteTree(Node_t *node);
Node_t *CreateNode(int degree);

#endif
