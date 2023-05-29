#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MaxSize 2000
#define Length 250 // 2000:8==250
#define MaxShift 7 // maximum bitwise shift for 1 in char
#define BitsInChar 8

typedef struct Stack Stack;
typedef struct Graph Graph;

struct Stack{
    int array[MaxSize];
    int stackLen;
};

struct Graph{
    char matrix[MaxSize][Length]; // adjacency matrix
    Stack *stack;
    char used[MaxSize]; // array for storing visited vertices
};

Graph *MakeGraph(Graph *g){
    g = calloc(1, sizeof(Graph));
    g->stack = calloc(1, sizeof(Stack));
    return g;
}

void Free(Graph *g){
    free(g->stack);
    free(g);
}

void CheckN(int N){
    if (N < 0 || N > 2000){
        printf("bad number of vertices");
        exit(0);
    }
}

void CheckM(int M, int N){
    if (M < 0 || M > (N * (N - 1) / 2)){
        printf("bad number of edges");
        exit(0);
    }
}

void CheckVertexes(int vertex1, int vertex2, int N){
    if (vertex1 < 1 || vertex1 > N || vertex2 < 1 || vertex2 > N){
        printf("bad vertex");
        exit(0);
    }
}

void Push(Stack *stack, int vertex){
    stack->array[stack->stackLen] = vertex;
    stack->stackLen++;
}

void Pop(Stack *stack){
    printf("%i ", stack->array[stack->stackLen - 1]);
    stack->stackLen--;
}


void AddEdge(char *matrix, int u, int v) {
    // add edge from u to v
    int cellNum = v / BitsInChar; // char number
    int bitNum = v % BitsInChar; // bit number
    matrix[u*Length + cellNum] |= (1 << (MaxShift - bitNum));
}

// checking there is 1 in the adjacency matrix
int CheckConnection(char *matrix, int line, int culumnNum, int numBit){
    if ( matrix[line*Length + culumnNum] & (1 << (MaxShift - numBit)) ){
        return 1;
    }
    else{
        return 0;
    }
}


void DFS(Graph *graph, int start, char *arr){
    graph->used[start] = 1; // the vertex is visited
    arr[start] = 1; // the vertex is visited

    for (int i = 0; i < Length; ++i){
        for (int j = 0; j < BitsInChar; ++j){

            if (CheckConnection(*graph->matrix, start, i, j) == 1){
                int newStart = i * BitsInChar + j;
                if ( !graph->used[newStart] && arr[newStart] != 1){
                    DFS(graph, newStart, arr);
                }
                else if (arr[newStart] == 1){
                    printf("impossible to sort");
                    Free(graph);
                    exit(0);
                }
            }
        }
    }
    Push(graph->stack, start + 1);
    arr[start] = 2; // the vertex is listed on the stack
}

int main() {

    int N; //vertices
    if (scanf("%i ", &N) == EOF){
        printf("bad number of lines");
        exit(0);
    }
    CheckN(N);

    int M; //edges
    if (scanf("%i", &M) == EOF){
        printf("bad number of lines");
        exit(0);
    }
    CheckM(M, N);

    Graph *graph = NULL;

    graph = MakeGraph(graph);

    int u, v;
    for (int i = 0; i < M; ++i){
        if (scanf("%i %i", &u, &v) == EOF && M != 0){
            printf("bad number of lines");
            Free(graph);
            exit(0);
        }
        CheckVertexes(u, v, N);
        AddEdge(*graph->matrix, u - 1, v - 1);
    }

    char arr[MaxSize]; //array to check the loop
    for (int i = 0; i < N; ++i){
        if (!graph->used[i]){
            memset(arr, 0, MaxSize);
            DFS(graph, i, arr);
        }
    }

    while (graph->stack->stackLen != 0){
        Pop(graph->stack);
    }

    Free(graph);
    return EXIT_SUCCESS;
}
