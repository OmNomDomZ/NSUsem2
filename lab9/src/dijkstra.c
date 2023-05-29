#include "dijkstra.h"

typedef enum
{
    PATH_FOUND,
    PATH_NOT_FOUND
}PathToVertex;

static int pathComparison(uint64_t *distance, int32_t *adjMatrix, int num, int cur, int j)
{
    if ((distance[cur] + adjMatrix[cur * num + j]) <= distance[j])
    {
        return 1;
    }
    return 0;
}

static int FindMinEdge(uint64_t *distance, PathToVertex *path, int num)
{
    uint64_t min = ULLONG_MAX;
    int cur = 0;
    for (int i = 0; i < num; ++i)
    {
        if (distance[i] < min && path[i] == PATH_NOT_FOUND)
        {
            min = distance[i];
            cur = i;
        }
    }
    return cur;
}

static int printAnswer(uint64_t *distance, int *parents, int num, int checkOverflow, int start, int finish)
{
    for (int i = 0; i < num; ++i)
    {
        if (distance[i] <= INT_MAX)
        {
            printf("%lu ", distance[i]);
        }
        else
        {
            if (distance[i] != ULLONG_MAX)
            {
                printf("INT_MAX+ ");
            }
            else
            {
                printf("oo ");
            }
        }
    }
    printf("\n");

    if (checkOverflow == 1)
    {
        return (OVERFLOW);
    }
    else
    {
        if (distance[finish - 1] == ULLONG_MAX)
        {
            return (NO_PATH);
        }
        else
        {
            int cur = finish - 1;
            printf("%i ", cur + 1);
            while (cur != start - 1)
            {
                cur = parents[cur];
                printf("%i ", cur + 1);
            }
        }
    }
    return (SUCCESS);
}

void DijkstraAlgo(int32_t *adjMatrix, int num, int start, int finish)
{
    uint64_t *distance = malloc(num * sizeof(uint64_t));
    int *parents = malloc(num * sizeof(int));
    PathToVertex *path = malloc(num * sizeof(PathToVertex));
    int checkOverflow = 0;

    for (int i = 0; i < num; ++i)
    {
        distance[i] = (i == start - 1) ? 0 : ULLONG_MAX;
        path[i] = PATH_NOT_FOUND;
        parents[i] = i;
    }

    int cur = start - 1;
    for (int i = 0; i < num; ++i)
    {
        path[cur] = PATH_FOUND;
        for (int j = 0; j < num; ++j)
        {
            if ((adjMatrix[cur * num + j] != 0) && pathComparison(distance, adjMatrix, num, cur, j) && path[j] == PATH_NOT_FOUND)
            {
                if (distance[j] > INT_MAX && distance[j] != ULLONG_MAX && j == finish - 1)
                {
                    checkOverflow = 1;
                }
                distance[j] = (distance[cur] + adjMatrix[cur * num + j]);
                parents[j] = cur;
            }
        }
        cur = FindMinEdge(distance, path, num);
    }
    ExitCodes rc;
    if ((rc = printAnswer(distance, parents, num, checkOverflow, start, finish)) != SUCCESS)
    {
        printf("%s", ExitMessage[rc]);
    }
    free(distance);
    free(parents);
    free(path);
}
