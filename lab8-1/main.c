#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>

#define MAX_NUMBER_VERTEX 5000

typedef struct Edge Edge_t;
typedef struct MST MST_t;

struct Edge{
    int16_t x;
    int16_t y;
};

typedef enum {
    SUCCESS,
    BAD_LENGTH,
    BAD_LINES,
    BAD_VERTICES,
    BAD_VERTEX,
    BAD_EDGES,
    NO_SPAN_TREE
}ExitCodes;

char* ExitMessage[] = {
    "",
    "bad length",
    "bad number of lines",
    "bad number of vertices",
    "bad vertex",
    "bad number of edges",
    "no spanning tree"
};

typedef enum{
    VERTEX_IN_MST,
    VERTEX_NOT_IN_MST
} PositionVertex;

int CheckValues(int *numOfVer, int *numOfEdg){
    if (scanf("%i %i", numOfVer, numOfEdg) != 2){
        return (BAD_LINES);
    }
    if (*numOfVer < 0 || *numOfVer > MAX_NUMBER_VERTEX){
        return (BAD_VERTICES);
    }
    if (*numOfEdg < 0 || *numOfEdg > (*numOfVer * (*numOfVer - 1) / 2)) {
        return (BAD_EDGES);
    }
    if(*numOfEdg == 0 && *numOfVer != 1){
        return (NO_SPAN_TREE);
    }
    if (*numOfEdg < (*numOfVer - 1)){
        return (NO_SPAN_TREE);
    }
    return (SUCCESS);
}

void AddEdge(uint32_t *adjacencyMatrix, int x, int y, int numOfVer, int length){
    adjacencyMatrix[x * numOfVer + y] = length;
}

int ReadEdges(uint32_t *adjacencyMatrix, int numOfVer, int NumOfEdg){
    int x, y;
    uint64_t length;
    for (int i = 0; i < NumOfEdg; ++i){
        if (scanf("%i %i %lu", &x, &y, &length) != 3){
            return (BAD_LINES);
        }
        if (x < 1 || x > numOfVer || y < 1 || y > numOfVer){
            return (BAD_VERTEX);
        }
        if (length > INT_MAX){
            return (BAD_LENGTH);
        }
        AddEdge(adjacencyMatrix, x - 1, y - 1, numOfVer, length);
        AddEdge(adjacencyMatrix, y - 1, x - 1, numOfVer, length);
    }
    return (SUCCESS);
}

int FindMinEdgeFrom(uint32_t* distanceToMST, int numOfVer){
    uint32_t min = distanceToMST[0];
    int from = 0;
    for (int j = 1; j < numOfVer; ++j){
        if (distanceToMST[j] < min){
            min = distanceToMST[j];
            from = j;
        }
    }
    return from;
}

int PrimAlgo(Edge_t *EdgesInMST, uint32_t *adjacencyMatrix, int numOfVer){
    uint32_t* distanceToMST = malloc(numOfVer * sizeof(uint32_t));
    int* from = malloc(numOfVer * sizeof(int));
    PositionVertex* vertexInMST = malloc(numOfVer * sizeof(PositionVertex));
    for (int i = 0; i < numOfVer; ++i){
        distanceToMST[i] = (uint32_t)INT_MAX + 1;
        vertexInMST[i] = VERTEX_NOT_IN_MST;
    }
    vertexInMST[0] = VERTEX_IN_MST;
    int countEdge = 0;
    int cur = 0; //current Vertex
    uint32_t currentCell = 0;
    for (int i = 0; i < numOfVer - 1; ++i){
        for (int j = 1; j < numOfVer; ++j){
            currentCell = adjacencyMatrix[cur * numOfVer + j];
            if (currentCell < distanceToMST[j] &&
                currentCell != 0 &&
                distanceToMST[j] != UINT32_MAX)
            {
                distanceToMST[j] = currentCell;
                from[j] = cur;
                vertexInMST[j] = VERTEX_IN_MST;
            }
        }
        distanceToMST[cur] = UINT32_MAX;
        int newVertex = FindMinEdgeFrom(distanceToMST, numOfVer);
        EdgesInMST[countEdge].x = from[newVertex] + 1;
        EdgesInMST[countEdge].y = newVertex + 1;
        countEdge++;
        cur = newVertex;
    }
    for (int i = 0; i < numOfVer; ++i){
        if (vertexInMST[i] == VERTEX_NOT_IN_MST){
            free(distanceToMST);
            free(from);
            free(vertexInMST);
            return (NO_SPAN_TREE);
        }
    }
    free(distanceToMST);
    free(from);
    free(vertexInMST);
    return (SUCCESS);
}

int main(){
    int numOfVertices;
    int numOfEdges;
    ExitCodes rc;
    if ((rc = CheckValues(&numOfVertices, &numOfEdges)) != SUCCESS){
        printf("%s", ExitMessage[rc]);
        return 0;
    }

    uint32_t *adjacencyMatrix = calloc(numOfVertices * numOfVertices, sizeof(uint32_t));

    if ((rc = ReadEdges(adjacencyMatrix, numOfVertices, numOfEdges)) != SUCCESS){
        printf("%s", ExitMessage[rc]);
        free(adjacencyMatrix);
        return 0;
    }

    Edge_t *EdgesInMST = malloc((numOfVertices - 1) * sizeof(Edge_t));

    if ((rc = PrimAlgo(EdgesInMST, adjacencyMatrix, numOfVertices)) != SUCCESS){
        printf("%s", ExitMessage[rc]);
        free(EdgesInMST);
        free(adjacencyMatrix);
        return 0;
    }

    for(int i = 0; i < numOfVertices - 1; ++i){
        printf("%d %d\n", EdgesInMST[i].x, EdgesInMST[i].y);
    }
    free(EdgesInMST);
    free(adjacencyMatrix);
    return 0;
}
