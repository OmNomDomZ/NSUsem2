#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

typedef struct Node Node_t;
typedef struct MST MST_t;

struct Node{
    int16_t x;
    int16_t y;
    int length;
};

struct MST{
    Node_t *edges;
    int len;
};

int compare(const void *a, const void *b);
int CheckValues(int numOfVer, int numOfEdg);
int ReadEdges(Node_t *edges, int numOfVer, int NumOfEdg);
int Find(int *dsu, int vertex);

void InitDSU(int *dsu, int numOfVer);
void UniteSets(int *dsu, int cno1, int cno2, int numOfVer);
void KruskalAlgo (Node_t *edges, MST_t *MST,int *dsu, int numOfVer, int NumOfEdg);
void Print(MST_t *MST, int numOfVer);
void Free(MST_t *MST, Node_t *edges, int *dsu);

#endif
