#include <stdlib.h>
#include <stdio.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef struct Items Items;
typedef struct Queue Queue;

struct Items{
    int *weight;
    int *cost;
    int len;
};

struct Queue{
    int **item;
    int len;
};

Queue *MakeQueue(int N){
    Queue *q = malloc(sizeof(Queue));
    q->item = malloc(N * sizeof(int*));
    for (int i = 0; i < N; ++i){
        q->item[i] = calloc(2, sizeof(int)); //weight and cost of one item
    }
    q->len = 0;
    return q;
}

void FreeQueue(Queue *queue, int N){
    for (int i = 0; i < N; i++){
        free(queue->item[i]);
    }
    free(queue->item);
    free(queue);
}

Items *MakeData(int N){
    Items *d = malloc(sizeof(Items));
    d->weight = malloc(N * sizeof(int));
    d->cost = malloc(N * sizeof(int));
    d->len = 0;
    return d;
}

void FreeData(Items *data){
    free(data->weight);
    free(data->cost);
    free(data);
}

void FreeMatrix(int **matrix, int N){
    for (int i = 0; i < N; ++i){
        free(matrix[i]);
    }
    free(matrix);
}

void PushData(Items *data, int w, int c){
    data->weight[data->len] = w;
    data->cost[data->len] = c;
    data->len++;
}

void PushQueue(const Items *data, Queue *queue, int i){
    queue->item[queue->len][0] = data->weight[i];
    queue->item[queue->len][1] = data->cost[i];
    queue->len++;
}

void FillMatrix(const Items *data, int **matrix, int N, int W){
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < W; ++j){
            if (i == 0 || j == 0){
                matrix[i][j] = 0;
                continue;
            }
            // оставляем вес без измений
            int lastValue = matrix[i - 1][j];
            if (j  >= data->weight[i - 1]){
                // пробуем поместить в рюкзак другой предмет
                int newValue = matrix[i - 1][j - data->weight[i - 1]] + data->cost[i - 1];
                matrix[i][j] = MAX(lastValue, newValue);
            }
            else{
                matrix[i][j] = lastValue;
            }
        }
    }
}

void TakenItems(const Items *data, Queue *queue, int **matrix, int N, int W){
    int column = W - 1;
    for (int i = N; i > 0; i--){
        if (matrix[i][column] != matrix[i - 1][column]){
            PushQueue(data, queue, i - 1);
            column -= data->weight[i-1];
        }
    }
}

int main(void) {
    int N;
    if (scanf("%i", &N) == EOF){
        return 0;
    }

    int W;
    if (scanf("%i", &W) == EOF){
        return 0;
    }

    Items *data;
    data = MakeData(N);

    int w, c;
    for (int i = 0; i < N; ++i){
        if (scanf("%i %i", &w, &c) == EOF){
            FreeData(data);
            return 0;
        }
        PushData(data, w, c);
    }

    int **matrix = malloc(sizeof(*matrix) * (N + 1));
    for (int i = 0; i < (N + 1); ++i){
        matrix[i] = malloc(sizeof(**matrix) * (W + 1));
    }

    FillMatrix(data, matrix, N + 1, W + 1);

    Queue *queue;
    queue = MakeQueue(N);

    TakenItems(data, queue, matrix, N, W + 1);

    int maxCost = matrix[N][W];
    printf("%i\n", maxCost);
    for (int i = queue->len - 1; i >= 0; --i){
        printf("%i %i\n", queue->item[i][0], queue->item[i][1]);
    }

    FreeQueue(queue, N);
    FreeData(data);
    FreeMatrix(matrix, N + 1);
    return EXIT_SUCCESS;
}
