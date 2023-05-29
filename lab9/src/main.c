#include "global.h"
#include "enter.h"
#include "matrix.h"
#include "dijkstra.h"

char *ExitMessage[] =
{
    "",
    "bad number of vertices",
    "bad number of edges",
    "bad vertex",
    "bad length",
    "bad number of lines",
    "no path",
    "overflow"
};

int main()
{
    ExitCodes rc;
    int num;
    int start;
    int finish;
    int edges;

    if ((rc = (CheckNum(&num))) != SUCCESS)
    {
        printf("%s", ExitMessage[rc]);
        return 0;
    }
    if ((rc = (CheckStartFinish(&start, &finish, num))) != SUCCESS)
    {
        printf("%s", ExitMessage[rc]);
        return 0;
    }
    if ((rc = (CheckEdges(&edges, num))) != SUCCESS)
    {
        printf("%s", ExitMessage[rc]);
        return 0;
    }

    int32_t *adjMatrix = calloc(num * num, sizeof(int32_t));
    if ((rc = FillMatrix(adjMatrix, num, edges)) != SUCCESS)
    {
        free(adjMatrix);
        printf("%s", ExitMessage[rc]);
        return 0;
    }
    DijkstraAlgo(adjMatrix, num, start, finish);
    free(adjMatrix);
    return 0;
}
