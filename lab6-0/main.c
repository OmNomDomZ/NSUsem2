#include <stdio.h>
#include <stdlib.h>

// #define HERE do{fprintf(stderr, "HERE %i\n", __LINE__); fflush(0); }while(0)

typedef struct AVL AVL;

struct AVL{
    int key; //значение узла
    int height; //высота поддерева с корнем в узле value
    AVL* left; //указатель на левое поддерево
    AVL* right; //указатель на правое поддерево
};

AVL *newNode(AVL *node, int k) {
    node->key = k;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

int height(AVL *t){
    return t ? t->height : 0;
}

int balanceValue(AVL *t){ //проверяем балансировку дерева
    return height(t->right)-height(t->left);
}

void fixHeight(AVL *t){//вычисляем высоту узла
    int hl = height(t->left);//высота левого поддерева
    int hr = height(t->right);//высота левого поддерева
    if (hl > hr) {t->height = hl + 1;}
    else {t->height = hr + 1;}
}

AVL *rotateRight(AVL *t){
    AVL *q = t->left;
    t->left = q->right;
    q->right = t;
    fixHeight(t);
    fixHeight(q);
    return q;
}

AVL *rotateLeft(AVL *t){
    AVL *q = t->right;
    t->right = q->left;
    q->left = t;
    fixHeight(t);
    fixHeight(q);
    return q;
}

AVL *balance(AVL *t){ //балансировка дерева
    fixHeight(t);
    if (balanceValue(t) == 2){ //перевешивает правое поддерево
        if (balanceValue(t->right) < 0){ //левое поддерево правого поддерева перевешивает (совершаем большой поворот)
            t->right = rotateRight(t->right);
        }
        return rotateLeft(t);
    }
    if (balanceValue(t) == -2){//перевешивает левое поддерево (далее аналогично тому что выше)
        if (balanceValue(t->left) > 0){
            t->left = rotateLeft(t->left);
        }
        return rotateRight(t);
    }
    return t;
}

AVL *Insert(AVL *t, AVL *node){
    if (t == NULL) {
        t =  node;
        return t;
    }
    if (node->key < t->key){
        t->left = Insert(t->left, node);
    }
    else{
        t->right = Insert(t->right, node);
    }
    return balance(t);
}

int main(void){
    int N;
    if (scanf("%i", &N) == 0){exit(0);}

    AVL *t = calloc(N, sizeof(AVL));

    AVL *tree = NULL;
    for (int i = 0; i < N; ++i){
        int newKey;
        if (!(scanf("%i", &newKey))){
            exit(0);
        }
        AVL *node = newNode(&t[i], newKey);//создаем новый узел
        tree = Insert(tree, node);
    }
    printf("%i\n", height(tree));
    free(t);
    return 0;
}
