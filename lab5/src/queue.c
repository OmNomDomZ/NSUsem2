#include "queue.h"

Queue_t *makeQueue(Node_t *Nodes, int countDiffSymbol)
{
    Queue_t *head = malloc(sizeof(Queue_t));
    head->next = NULL;
    head->val = calloc(1, sizeof(EncodeTree_t));
    head->val->frequency = Nodes[0].frequency;
    head->val->val = Nodes[0].symbol;

    for (int i = 1; i < countDiffSymbol; ++i)
    {
        Queue_t *newElement = malloc(sizeof(Queue_t));
        newElement->val = calloc(1, sizeof(EncodeTree_t));
        newElement->val->frequency = Nodes[i].frequency;
        newElement->val->val = Nodes[i].symbol;
        newElement->next = head;
        head = newElement;
    }
    return head;
}

EncodeTree_t *pop(Queue_t **head)
{
    Queue_t *cur = (*head);
    EncodeTree_t *value = cur->val;
    (*head) = (*head)->next;
    free(cur);
    return value;
}

Queue_t *unite(EncodeTree_t *arg1, EncodeTree_t *arg2)
{
    Queue_t *united = malloc(sizeof(Queue_t));
    united->val = malloc(sizeof(EncodeTree_t));
    united->val->left = arg1;
    united->val->right = arg2;
    united->val->frequency = arg1->frequency + arg2->frequency;
    united->val->val = 0;
    return united;
}

Queue_t *insert(Queue_t *head, Queue_t *unitedNodes)
{
    if (head->val->frequency >= unitedNodes->val->frequency)
    {
        unitedNodes->next = head;
        head = unitedNodes;
        return head;
    }
    else
    {
        Queue_t *tmp = head;
        while (tmp->next != NULL && tmp->next->val->frequency < unitedNodes->val->frequency)
        {
            tmp = tmp->next;
        }
        unitedNodes->next = tmp->next;
        tmp->next = unitedNodes;
        return head;
    }
}
