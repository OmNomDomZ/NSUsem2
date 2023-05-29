#include "BTree.h"

Node_t *CreateNode(int degree)
{
    Node_t *node = malloc(sizeof(Node_t));
    node->isLeaf = 1;
    node->numOfKeys = 0;
    node->keys = malloc((2*degree-1) * sizeof(int));
    node->children = NULL;
    return node;
}

void MoveKeys(Node_t *old, Node_t *new, int indexOld, int indexNew)
{
    int index = new->numOfKeys - indexNew;
    memmove(new->keys + indexNew, old->keys + indexOld, index * sizeof(int));
    if (old->isLeaf == 0)
    {
        memmove(new->children + indexNew, old->children + indexOld, (index + 1) * sizeof(Node_t *));
    }
}

void Split(Node_t *node, int degree, int index)
{
    Node_t *old = node->children[index];
    Node_t *new = CreateNode(degree);
    if (old->isLeaf == 0)
    {
        new->isLeaf = 0;
        new->children = malloc((2*degree) * sizeof(Node_t*));
    }
    new->numOfKeys = degree - 1;
    MoveKeys(old, new, degree, 0);
    old->numOfKeys = degree - 1;
    node->numOfKeys++;
    MoveKeys(node, node, index, index + 1);
    node->keys[index] = old->keys[degree - 1];
    node->children[index + 1] = new;
}

int GetIndex(Node_t *node, int cur)
{
    int left = 0;
    int right = node->numOfKeys - 1;
    while (left <= right)
    {
        int middle = (left + right) / 2;
        if (cur <= node->keys[middle])
        {
            right = middle - 1;
        }
        else
        {
            left = middle + 1;
        }
    }
    return right + 1;
}

void InsertInNode(Node_t *node, int cur)
{
    int index = GetIndex(node, cur);
    node->numOfKeys++;
    MoveKeys(node, node, index, index + 1);
    node->keys[index] = cur;
}

void InsertNonFull(Node_t *node, int degree, int cur)
{
    if (node->isLeaf == 1)
    {
        InsertInNode(node, cur);
    }
    else
    {
        int index = GetIndex(node, cur);
        if (node->children[index]->numOfKeys == (2*degree-1))
        {
            Split(node, degree, index);
        }
        InsertNonFull(node->children[GetIndex(node, cur)], degree, cur);
    }
}

void InsertInTree(BTree_t *tree, int cur)
{
    if (tree->node->numOfKeys == 2*(tree->degree)-1)
    {
        Node_t *newNode = CreateNode(tree->degree);
        newNode->children = malloc(2*(tree->degree) *sizeof(Node_t));
        newNode->isLeaf = 0;

        newNode->children[0] = tree->node;
        Split(newNode, tree->degree, 0);
        tree->node = newNode;
        tree->height++;
    }
    InsertNonFull(tree->node, tree->degree, cur);
}

void DeleteTree(Node_t *node)
{
    free(node->keys);
    if (node->isLeaf == 1)
    {
        free(node);
        return;
    }
    else
    {
        for (int i = 0; i <= node->numOfKeys; ++i)
        {
            DeleteTree(node->children[i]);
        }
        free(node->children);
        free(node);
    }
}
