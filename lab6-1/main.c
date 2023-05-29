#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LEFT 0
#define RIGHT 1

typedef struct Node Node_t;

enum Color{
    RED = 0,
    BLACK = 1
};

struct Node {
    int value;
    char color;
    Node_t *child[2];
};

Node_t *MakeNode(Node_t *n, int value, int color){
    n->value = value;
    n->color = color;
    n->child[LEFT] = NULL;
    n->child[RIGHT] = NULL;
    return n;
}

bool Red(Node_t *node){
    if (node == NULL){
        return false;
    }
    return (node->color == RED);
}

void ColorFlip(Node_t *node){
    node->color = node->color ^ 1;
    node->child[LEFT]->color = node->child[LEFT]->color ^ 1;
    node->child[RIGHT]->color = node->child[RIGHT]->color ^ 1;
}

Node_t *Rotate(Node_t *node, bool dir){
    Node_t *temp = node->child[!dir];
    node->child[!dir] = temp->child[dir];
    temp->child[dir] = node;

    temp->color = node->color;
    node->color = RED;
    return temp;
}

Node_t *DoubleRotate(Node_t *node, bool dir){
    node->child[!dir] = Rotate(node->child[!dir], !dir);
    return Rotate(node, dir);
}

Node_t *InsertFixUp(Node_t *node, bool dir){
    //one of the children is red
    if (Red(node->child[dir])){
        //1 : both child red
        if (Red(node->child[!dir])){
            if (Red(node->child[dir]->child[dir]) || Red(node->child[dir]->child[!dir])){
                ColorFlip(node);
            }
        }
        else {
            // any child has 2 reds in a row
            if (Red(node->child[dir]->child[dir])){
                node = Rotate(node, !dir);
            }
            else if(Red(node->child[dir]->child[!dir])){
                node = DoubleRotate(node, !dir);
            }
        }
    }
    return node;
}

Node_t *Insert(Node_t *tree, Node_t *node){
    if (tree == NULL){
        tree = node;
        return tree;
    }
    //left = 0, right = 1
    bool dir = (node->value > tree->value);

    tree->child[dir] = Insert(tree->child[dir], node);

    tree = InsertFixUp(tree, dir);
    return tree;
}

int BlackHeight(Node_t *node){
    int height = 0;
    while (node){
        if (node->color == BLACK){
            height++;
        }
        node = node->child[LEFT];
        if (!node){
            height++;
        }
    }
    return height;
}

int main(void){
    int N;
    if (scanf("%i", &N) == EOF){
        return 0;
    }

    Node_t *tree = NULL;
    Node_t *nodes = malloc(N * sizeof(Node_t));


    for (int i = 0; i < N; ++i){
        int value;
        if (scanf("%i", &value) != 1){
            free(nodes);
            return 0;
        }
        Node_t *n = MakeNode(&nodes[i], value, RED);
        tree = Insert(tree, n);
        tree->color = BLACK;
    }
    int blackHeight = BlackHeight(tree);
    printf("%i", blackHeight);
    free(nodes);
    return EXIT_SUCCESS;
}
