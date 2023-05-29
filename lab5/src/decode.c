#include "decode.h"
#include "freeD.h"
#include "global.h"

#define SIZE_OF_BYTE 8

static DecodeTree_t *addNewEdge(int val)
{
    DecodeTree_t *edge = calloc(1, sizeof(DecodeTree_t));
    if (val != 0)
    {
        edge->val = val;
    }
    return edge;
}

static ExitCodes writeVal(unsigned char *byte, int *pos, uint8_t *value)
{
    uint8_t val = 0;
    val = val | ((*byte) << (*pos));
    if (fread(byte, sizeof(uc), 1, in) != 1)
    {
        return FAILURE;
    }
    val = val | ((*byte) >> (SIZE_OF_BYTE - (*pos)));
    if ((*pos) == SIZE_OF_BYTE)
    {
        if (fread(byte, sizeof(uc), 1, in) != 1)
        {
            return FAILURE;
        }
        (*pos) = 0;
    }
    (*value) = val;
    return SUCCESS;
}

static DecodeTree_t *recoveryTree(uc *curByte, int *pos)
{
    ExitCodes rc;
    (*pos)++;
    if ((*pos) > SIZE_OF_BYTE)
    {
        if (fread(curByte, sizeof(uc), 1, in) != 1)
        {
            return NULL;
        }
        (*pos) = 1;
    }
    if ((((*curByte) >> (SIZE_OF_BYTE - (*pos)) & 1) + '0') == '1')
    {
        uint8_t value;
        if ((rc = writeVal(curByte, pos, &value)) == FAILURE)
        {
            return NULL;
        }
        return addNewEdge(value);
    }
    else
    {
        DecodeTree_t *root = addNewEdge(0);
        root->left = recoveryTree(curByte, pos);
        root->right = recoveryTree(curByte, pos);
        return root;
    }
}

static int isLeafD(DecodeTree_t *root)
{
   return ((!root->left && !root->right) ? 1 : 0);
}

static ExitCodes recoveryText(DecodeTree_t *root, uc *curByte, int *pos)
{
    while (!isLeafD(root))
    {
        (*pos)++;
        if ((*pos) > SIZE_OF_BYTE)
        {
            if (fread(curByte, sizeof(uc), 1, in) != 1)
            {
                return FAILURE;
            }
            (*pos) = 1;
        }
        if (((((*curByte) >> (SIZE_OF_BYTE - (*pos))) & 1) + '0') == '1')
        {
            root = root->right;
        }
        if (((((*curByte) >> (SIZE_OF_BYTE - (*pos))) & 1) + '0') == '0')
        {
            root = root->left;
        }
    }
    fwrite(&(root->val), sizeof(uc), 1, out);
    return SUCCESS;
}

void decode()
{
    //num of symbols in text
    ExitCodes rc;
    int32_t symNum = 0;
    if (fread(&symNum, sizeof(int32_t), 1, in) != 1)
    {
        return;
    }
    uc curByte = 0;
    if (fread(&curByte, sizeof(uc), 1, in) != 1)
    {
        return;
    }
    int pos = 0;
    DecodeTree_t *root = recoveryTree(&curByte, &pos);
    if (root == NULL)
    {
        return;
    }
    for (int decodedSym = 0; decodedSym < symNum; ++decodedSym)
    {
        if((rc = recoveryText(root, &curByte, &pos)) == FAILURE)
        {
            freeDecode(root);
            return;
        }
    }
    freeDecode(root);
}
