#include "Kruskal.h"

#define MAX 5000

int compare(const void *i, const void *j){
    int l = ((Node_t *)i)->length;
    int r = ((Node_t *)j)->length;
    return l - r;
}

int CheckValues(int numOfVer, int numOfEdg){
    if (numOfVer < 0 || numOfVer > MAX){
        printf("bad number of vertices");
        return EXIT_FAILURE;
    }
    if (numOfEdg < 0 || numOfEdg > (numOfVer * (numOfVer - 1) / 2)) {
        printf("bad number of edges");
        return EXIT_FAILURE;
    }
    if(numOfEdg == 0 && numOfVer == 0){
        printf("no spanning tree");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int ReadEdges(Node_t *edges, int numOfVer, int NumOfEdg){
    int16_t x, y;
    uint64_t length;
    for (int i = 0; i < NumOfEdg; ++i){
        if (scanf("%hi %hi %lu", &x, &y, &length) != 3){
            printf("bad number of lines");
            return EXIT_FAILURE;
        }
        if (x < 1 || x > numOfVer || y < 1 || y > numOfVer){
            printf("bad vertex");
            return EXIT_FAILURE;
        }
        if (length > INT_MAX){
            printf("bad length");
            return EXIT_FAILURE;
        }
        else{
            edges[i].x = x;
            edges[i].y = y;
            edges[i].length = length;
        }
    }
    return EXIT_SUCCESS;
}

void InitDSU(int *dsu, int numOfVer){
    for (int i = 0; i < numOfVer + 1; ++i){
        dsu[i] = i;
    }
}

int Find(int *dsu, int vertex){
    return (dsu[vertex]);
}

void UniteSets(int *dsu, int cno1, int cno2, int numOfVer){
    for (int i = 0; i < numOfVer + 1; ++i){
        if (dsu[i] == cno2){
            dsu[i] = cno1;
          }
    }
}

void KruskalAlgo (Node_t *edges, MST_t *MST,int *dsu, int numOfVer, int NumOfEdg){

    for (int i = 0; i < NumOfEdg; ++i){
        int cno1, cno2;
        cno1 = Find(dsu, edges[i].x);
        cno2 = Find(dsu, edges[i].y);

        if (cno1 != cno2){
            MST->edges[MST->len] = edges[i];
            MST->len++;
            UniteSets(dsu, cno1, cno2, numOfVer);
            //функция для объединения множеств
        }
    }
}

void Print(MST_t *MST, int numOfVer){
    if (MST->len < numOfVer - 1){
        printf("no spanning tree");
    }
    else{
        for (int i = 0; i < MST->len; ++i){
            printf("%i %i\n", MST->edges[i].x, MST->edges[i].y);
        }
    }
}

void Free(MST_t *MST, Node_t *edges, int *dsu){
    free(MST->edges);
    free(MST);
    free(edges);
    free(dsu);
}
