#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define MAX_N 100000

typedef struct Point
{
    int x, y;
}Point_t;

double Rotate(Point_t a, Point_t b, Point_t c)
{
    return (((double)b.x - a.x) * ((double)c.y - b.y) - ((double)b.y - a.y) * ((double)c.x - b.x));
}

void Swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void FindBottomPoint(Point_t points[], int listOfPoints[], int N)
{
    for (int i = 1; i < N; i++)
    {
        if ((points[listOfPoints[i]].x < points[listOfPoints[0]].x)
        || (points[listOfPoints[i]].x == points[listOfPoints[0]].x && points[listOfPoints[i]].y < points[listOfPoints[0]].y))
        {
            Swap(listOfPoints + i, listOfPoints);
        }
    }
    listOfPoints[N] = listOfPoints[0];
}

void SortPoints(int *listOfPoints, Point_t *points, int first, int last)
{
    if (first >= last)
    {
        return;
    }
    int left = first, right = last;
    Point_t lowest = points[listOfPoints[0]];
    Point_t middle = points[listOfPoints[(left + right) / 2]];

    do
    {
        while (1)
        {
            double r = Rotate(lowest, middle, points[listOfPoints[left]]);
            if (r >= 0)
            {
                break;
            }
            left++;
        }
        while (Rotate(lowest, middle, points[listOfPoints[right]]) > 0)
        {
            right--;
        }

        if (left <= right)
        {
            Swap(listOfPoints + left, listOfPoints + right);
            left++;
            right--;
        }
    } while (left <= right);
    SortPoints(listOfPoints, points, first, right);
    SortPoints(listOfPoints, points, left, last);
}

void PrintPoints(Point_t points[], int size, int convexHull[])
{
    for (int i = 0; i < size; i++)
    {
        printf("%i %i\n", points[convexHull[i]].x, points[convexHull[i]].y);
    }
}

void GeneralCase(Point_t *points, int *listOfPoints, int *convexHull, int N, int *size)
{
    convexHull[0] = listOfPoints[0];
    convexHull[1] = listOfPoints[1];
    int j = 2;
    double rot;
    Point_t A, B, C;
    for (int i = 2; i <= N; i++)
    {
        while (j > 1)
        {
            A = points[convexHull[j - 2]];
            B = points[convexHull[j - 1]];
            C = points[listOfPoints[i]];
            rot = Rotate(A, B, C);
            if (rot > 0)
            {
                break;
            }
            j--;
        }
        convexHull[j] = listOfPoints[i];
        j++;
    }
    if (j > 2)
    {
        *size = j - 1;
    }
    else
    {
        *size = j;
    }
}

void OneLineCase(Point_t *points, int *listOfPoints, int *convexHull, int N, int *size)
{
    if (points[listOfPoints[0]].x == points[listOfPoints[N - 1]].x)
    {
        double maxY = (double)points[listOfPoints[0]].y;
        int IndMaxY = 0;
        for (int i = 1; i < N; i++)
        {
            if (points[listOfPoints[i]].y > maxY)
            {
                maxY = points[listOfPoints[i]].y;
                IndMaxY = i;
            }
        }
        convexHull[0] = listOfPoints[0];
        *size = 2;
        convexHull[1] = listOfPoints[IndMaxY];
    }
    else
    {
        double maxX = (double)points[listOfPoints[0]].x;
        int IndMaxX = 0;
        for (int i = 1; i < N; i++)
        {
            if (points[listOfPoints[i]].x > maxX)
            {
                maxX = points[listOfPoints[i]].x;
                IndMaxX = i;
            }
        }
        convexHull[0] = listOfPoints[0];
        convexHull[1] = listOfPoints[IndMaxX];
        *size = 2;
    }
}

void GrahamScan(Point_t *points, int *listOfPoints, int *convexHull, int N, int *size)
{
    if (N == 0)
    {
        return;
    }
    bool oneLineFlag = 1;
    for (int i = 0; i < N; i++)
    {
        convexHull[0] = listOfPoints[0];
        convexHull[1] = listOfPoints[1];
        int j = 2;
        if (Rotate(points[convexHull[j - 2]], points[convexHull[j - 1]], points[listOfPoints[i]]) != 0)
        {
            oneLineFlag = 0;
            break;
        }
    }
    if (oneLineFlag == 1)
        OneLineCase(points, listOfPoints, convexHull, N, size);
    else
        GeneralCase(points, listOfPoints, convexHull, N, size);
}

int main()
{
    int N;
    if (scanf("%i", &N) == EOF)
    {
        return 0;
    }

    if (N < 0 || N > MAX_N)
    {
        printf("bad number of points\n");
        return 0;
    }

    Point_t *points = malloc(N * sizeof(Point_t));
    for (int i = 0; i < N; i++)
    {
        if (scanf("%i %i", &points[i].x, &points[i].y) != 2)
        {
            printf("bad number of lines\n");
            return 0;
        }
    }
    if (N == 1)
    {
        printf("%i %i\n", points[0].x, points[0].y);
        free(points);
        return 0;
    }
    int *listOfPoints = malloc((N + 1) * sizeof(int));
    if (listOfPoints == NULL)
    {
        free(points);
        return 0;
    }
    for (int i = 0; i < N; i++)
    {
        listOfPoints[i] = i;
    }
    FindBottomPoint(points, listOfPoints, N);
    SortPoints(listOfPoints, points, 1, N - 1);
    int *convexHull = malloc((N + 1) * sizeof(int));
    if (convexHull == NULL)
    {
        free(points);
        free(listOfPoints);
        return 0;
    }
    int size = 0;
    GrahamScan(points, listOfPoints, convexHull, N, &size);
    PrintPoints(points, size, convexHull);
    free(points);
    free(listOfPoints);
    free(convexHull);
    return 0;
}
