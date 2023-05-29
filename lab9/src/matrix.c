#include "matrix.h"

static void AddEdge(int32_t *adjMatrix, int x, int y, int num, int length)
{
    adjMatrix[x * num + y] = length;
}

int FillMatrix(int32_t *adjMatrix, int num, int edges)
{
    int x, y;
    uint64_t length;
    for (int i = 0; i < edges; ++i){
        if (scanf("%i %i %lu", &x, &y, &length) != 3){
            return (BAD_NUM_OF_LINES);
        }
        if (x < 1 || x > num || y < 1 || y > num){
            return (BAD_VER);
        }
        if (length > INT_MAX){
            return (BAD_LENGTH);
        }
        AddEdge(adjMatrix, x - 1, y - 1, num, length);
        AddEdge(adjMatrix, y - 1, x - 1, num, length);
    }
    return (SUCCESS);
}
