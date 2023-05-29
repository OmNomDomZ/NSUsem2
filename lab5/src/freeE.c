#include "freeE.h"

#define ALPHABET_SIZE 256

static void freeTree(EncodeTree_t *root)
{
    if (root->left)
    {
        freeTree(root->left);
    }
    if (root->right)
    {
        freeTree(root->right);
    }
    free(root);
}

void freeEncode(EncodeTree_t *root, Node_t *Nodes, int *ASCII, char *curCode, char **newCodes)
{
    for (int i = 0; i < ALPHABET_SIZE; ++i)
    {
        if (newCodes[i] != NULL)
        {
            free(newCodes[i]);
        }
    }
    free(newCodes);
    freeTree(root);
    free(Nodes);
    free(curCode);
    free(ASCII);
}
