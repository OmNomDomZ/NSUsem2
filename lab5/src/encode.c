#include "encode.h"
#include "freeE.h"
#include "queue.h"

#define ALPHABET_SIZE 256
#define SIZE_OF_BYTE 8

static int *BuildASCII(int *count, uc *val)
{
    int *ASCII = calloc(ALPHABET_SIZE, sizeof(int));
    ASCII[(int)(*val)]++;
    while (!feof(in))
    {
        if (fread(val, sizeof(uc), 1, in) != 1)
        {
            break;
        }
        ASCII[(int)(*val)]++;
    }
    for (int i = 0; i < ALPHABET_SIZE; ++i)
    {
        if (ASCII[i] != 0)
        {
            (*count)++;
        }
    }
    return ASCII;
}

static void makeNodes(Node_t *Nodes, int *ASCII)
{
    int k = 0;
    for (int i = 0; i < ALPHABET_SIZE; ++i)
    {
        if (ASCII[i] != 0)
        {
            Nodes[k].frequency = ASCII[i];
            Nodes[k].symbol = (uc)i;
            k++;
        }
    }
}

static int compareFrequency(const void *a, const void *b)
{
    int val1 = ((Node_t *)a)->frequency;
    int val2 = ((Node_t *)b)->frequency;
    return val2 - val1;
}

static EncodeTree_t *buildTree(Node_t *Nodes, int countDiffSymbol)
{
    qsort(Nodes, countDiffSymbol, sizeof(Nodes), compareFrequency);
    if (countDiffSymbol == 1)
    {
        EncodeTree_t *root = calloc(1, sizeof(EncodeTree_t));
        root->frequency = Nodes[0].frequency;
        root->val = Nodes[0].symbol;
        return (root);
    }

    Queue_t *head = makeQueue(Nodes, countDiffSymbol);

    if (head->next)
    {
        EncodeTree_t *arg1;
        EncodeTree_t *arg2;
        while (head->next->next)
        {
            arg1 = pop(&head);
            arg2 = pop(&head);
            // объединяем arg1 & arg2
            Queue_t *unitedNodes = unite(arg1, arg2);
            // вставляем в head
            head = insert(head, unitedNodes);
        }
        arg1 = pop(&head);
        arg2 = pop(&head);
        EncodeTree_t *root = malloc(sizeof(EncodeTree_t));
        root->left = arg1;
        root->right = arg2;
        root->frequency = arg1->frequency + arg2->frequency;
        root->val = 0;
        return root;
    }
    return NULL;
}

static int isLeafE(EncodeTree_t *root)
{
    return ((!root->left && !root->right) ? 1 : 0);
}

static void makeCodes(EncodeTree_t *root, char **newCodes, char *curCode)
{
    int curCodeLen = strlen(curCode);
    if (isLeafE(root))
    {
        newCodes[root->val] = malloc(curCodeLen + 1);
        strcpy(newCodes[root->val], curCode);
    }
    else
    {
        curCode[curCodeLen] = '0';
        curCode[curCodeLen + 1] = '\0';
        if (root->left)
        {
            makeCodes(root->left, newCodes, curCode);
        }

        curCode[curCodeLen] = '1';
        curCode[curCodeLen + 1] = '\0';
        if (root->right)
        {
            makeCodes(root->right, newCodes, curCode);
        }
    }
}

static void bitWrite(int *pos, uc *byte, uc bit)
{
    if (bit == '1')
    {
        // записываем '1' побитово в byte на место pos
        *byte = (*byte) | (1 << ((SIZE_OF_BYTE - 1) - (*pos)));
    }
    (*pos)++;
    if (*pos == SIZE_OF_BYTE)
    {
        // записываем byte в out.txt
        fwrite(byte, sizeof(uc), 1, out);
        *pos = 0;
        *byte = 0;
    }
}

static void writeTree(EncodeTree_t *root, uc *byte, int *pos)
{
    if (isLeafE(root))
    {
        // запись в out.txt дерева
        bitWrite(pos, byte, '1');
        for (int i = 0; i < SIZE_OF_BYTE; ++i)
        {
            // записываем побитово значание листа root->val
            bitWrite(pos, byte, (((root->val) >> (SIZE_OF_BYTE - 1 - i)) & 1) + '0');
        }
    }
    else
    {
        bitWrite(pos, byte, '0');
        if (root->left)
        {
            writeTree(root->left, byte, pos);
        }
        if (root->right)
        {
            writeTree(root->right, byte, pos);
        }
    }
}

static void writeCodes(char **newCodes, uc *byte, int *pos, fpos_t startPos)
{
    uc get;
    fsetpos(in, &startPos);

    while (!feof(in))
    {
        if (fread(&get, sizeof(uc), 1, in) != 1)
        {
            break;
        }
        for (size_t i = 0; i < strlen(newCodes[get]); ++i)
        {
            bitWrite(pos, byte, newCodes[get][i]);
        }
    }
    if ((*pos) != 0)
    {
        fwrite(byte, sizeof(uc), 1, out);
    }
}

void encode()
{
    int countDiffSymbol = 0;
    uc curVal;
    fpos_t startPos;
    fgetpos(in, &startPos);
    if (fread(&curVal, sizeof(uc), 1, in) != 1)
    {
        return;
    }
    int *ASCII = BuildASCII(&countDiffSymbol, &curVal);
    Node_t *Nodes = calloc(countDiffSymbol, sizeof(Node_t));
    makeNodes(Nodes, ASCII);

    char *curCode = calloc(ALPHABET_SIZE, sizeof(char));
    char **newCodes = calloc(ALPHABET_SIZE, sizeof(char *));
    // build a tree
    EncodeTree_t *root = buildTree(Nodes, countDiffSymbol);
    if (root == NULL)
    {
        freeEncode(root, Nodes, ASCII, curCode, newCodes);
        return;
    }

    // создать новые коды для листьев
    makeCodes(root, newCodes, curCode);

    // количество букв в изначальном файле
    fwrite(&(root->frequency), sizeof(int32_t), 1, out);
    uc byte = 0;
    int pos = 0;
    writeTree(root, &byte, &pos);
    writeCodes(newCodes, &byte, &pos, startPos);
    freeEncode(root, Nodes, ASCII, curCode, newCodes);
}
