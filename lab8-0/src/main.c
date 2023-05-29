#include "Kruskal.h"

int main(){
    int numOfVertices = 0;
    int numOfEdges = 0;

    if (scanf("%i %i", &numOfVertices, &numOfEdges) != 2){
        printf("bad number of lines");
        return 0;
    }
    if (CheckValues(numOfVertices, numOfEdges) != 0){
        return 0;
    }

    //система непересекающихся множеств (disjoin set union)
    int *dsu = malloc((numOfVertices + 1) * sizeof(int));
    Node_t *edges = malloc(numOfEdges * sizeof(Node_t));

    MST_t *MST = malloc(sizeof(MST_t));
    MST->edges = malloc(numOfEdges * sizeof(Node_t));
    MST->len = 0;

    if(ReadEdges(edges, numOfVertices, numOfEdges) != 0){
        Free(MST, edges, dsu);
        return 0;
    }
    qsort(edges, numOfEdges, sizeof(Node_t), compare);

    InitDSU(dsu, numOfVertices);
    KruskalAlgo(edges, MST, dsu, numOfVertices, numOfEdges);
    Print(MST, numOfVertices);

    Free(MST, edges, dsu);
    return EXIT_SUCCESS;
}
