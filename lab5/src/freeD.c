#include "freeD.h"

void freeDecode(DecodeTree_t *root)
{
    if (root->left)
    {
        freeDecode(root->left);
    }
    if (root->right)
    {
        freeDecode(root->right);
    }
    free(root);
}
