#include <stdlib.h>

#include "BTree.h"

int main(void)
{
    int degree;
    if (scanf("%i", &degree) != 1)
    {
        return 0;
    }
    int numOfKeys;
    if (scanf("%i", &numOfKeys) != 1)
    {
        return 0;
    }

    if (numOfKeys == 0)
    {
        printf("0");
        return 0;
    }

    BTree_t *tree = calloc(1, sizeof(BTree_t));
    tree->degree = degree;

    Node_t *node = CreateNode(tree->degree);
    tree->node = node;
    int cur;
    for (int i = 0; i < numOfKeys; ++i)
    {
        if (scanf("%i", &cur) != 1)
        {
            DeleteTree(tree->node);
            free(tree);
            return 0;
        }
        InsertInTree(tree, cur);
    }
    printf("%i", tree->height + 1);
    DeleteTree(tree->node);
    free(tree);
    return 0;
}
