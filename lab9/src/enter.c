#include "enter.h"

#define MAX_VAL 5000

int CheckNum(int *num)
{
    if (scanf("%i", num) != 1)
    {
        return (FAILURE);
    }
    if ((*num) > MAX_VAL || (*num) < 0)
    {
        return (BAD_NUM_OF_VER);
    }
    return (SUCCESS);
}

int CheckStartFinish(int *start, int *finish, int num)
{
    if (scanf("%i", start) != 1)
    {
        return (BAD_VER);
    }
    if (scanf("%i", finish) != 1)
    {
        return (BAD_VER);
    }
    if ((*start) < 1 || (*start) > num)
    {
        return (BAD_VER);
    }
    if ((*finish) < 1 || (*finish) > num)
    {
        return (BAD_VER);
    }
    return (SUCCESS);
}

int CheckEdges(int *edges, int num)
{
    if (scanf("%i", edges) != 1)
    {
        return (FAILURE);
    }
    if ((*edges) > ((num - 1) * num / 2) || (*edges) < 0)
    {
        return (BAD_NUM_OF_EDG);
    }
    return (SUCCESS);
}
