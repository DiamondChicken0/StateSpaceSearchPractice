#include <stdio.h>
#include <stdlib.h>

#define N 4
#define NxN 16 // Must be square of N to work


typedef struct node 
{
    int tiles[N][N];
    int depth;
    struct node *parent;
    struct node *next;
    int row0;
    int col0;
};

struct node *open;
struct node *closed;

void printANode(struct node *inp)
{
    printf("+-------------------+\n");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("| %.2i ", inp->tiles[i][j]);
        }
        printf("|\n+-------------------+\n");
    }
}

// 0 : Diff
// 1 : Chill
int compareNodes(struct node *first, struct node *second)
{
    for(int i = 0; i < NxN; i++)
    {
        if (first->tiles[i/N][i%N] != second->tiles[i/N][i%N])
            return 0;
    }
    return 1;
}

void assign0(struct node *inp)
{
    for(int i = 0; i < NxN; i++)
    {
        if (!inp->tiles[i/N][i%N])
        {
            inp->row0 = i/N;
            inp->col0 = i%N;
        }
    }
}

void swapTiles(struct node *inp, int oldR, int oldC, int newR, int newC)
{
    int tmp = inp->tiles[oldR][oldC];
    inp->tiles[oldR][oldC] = inp->tiles[newR][newC];
    inp->tiles[newR][newC] = tmp;
}
void closedAdd(struct node *inp)
{
    inp->next = closed;
    closed = inp;
}
void stackAdd(struct node *inp)
{
    
    if (!open)
    {
        open = inp;
        return;
    }
    inp->next = open;
    open = inp;
}

// 0 : Garbage, throw it out
// 1 : Good
int filter(struct node *cmp)
{
    struct node *chk = open;
    //printf("checking a node\n");
    while (chk)
    {
        if (compareNodes(chk, cmp))
            return 0;
        chk = chk->next;
    }
    struct node *chkCl = closed;
    //printf("checking a node\n");
    while (chkCl)
    {
        if (compareNodes(chkCl, cmp))
            return 0;
        chkCl = chkCl->next;
    }
    //printf("it was good :)\n");
    return 1;
}

void expand()
{
    /*struct node *queued = open;
    open = open->next; // Dequeue open

    queued->next = NULL;*/

    struct node *stack = open;
    open = open->next;
    stack->next = NULL;

    assign0(stack);
    closedAdd(stack);

    struct node *up = malloc(sizeof(struct node));
    *up = *stack;
    // UP
    //struct node up = *stack;
    //printf("Checking row0 %i\n", up.row0);
    if (up->row0 != 0)
    {
        swapTiles(up, up->row0, up->col0, up->row0 - 1, up->col0);
        if (filter(up))
        {
            up->parent = stack;
            assign0(up);
            stackAdd(up);
        }
    }
    
    // DOWN
    struct node *down = malloc(sizeof(struct node));
    *down = *stack;
    if (down->row0 != N-1)
    {
        swapTiles(down, down->row0, down->col0, down->row0 + 1, down->col0);
        if (filter(down))
        {
            down->parent = stack;
            assign0(down);
            stackAdd(down);
        }
    }

    // LEFT
    struct node *left = malloc(sizeof(struct node));
    *left = *stack;
    if (left->col0 != 0)
    {
        swapTiles(left, left->row0, left->col0, left->row0, left->col0 - 1);
        if (filter(left))
        {
            left->parent = stack;
            assign0(left);
            stackAdd(left);
        }
    }

    // RIGHT
    struct node *right = malloc(sizeof(struct node));
    *right = *stack;
    if (right->col0 != N-1)
    {
        swapTiles(right, right->row0, right->col0, right->row0, right->col0 + 1);
        if (filter(right))
        {
            right->parent = stack;
            assign0(right);
            stackAdd(right);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != NxN + 1)
    {
        printf("Invalid amount of arguements provided, please provide %i entries. %i provided", NxN, argc);
        return 100;
    }

    struct node *curr = (struct node *)malloc(sizeof(struct node));
    for (int i = 0; i < NxN; i++)
    {
        curr->tiles[i/N][i%N] = atoi(argv[i + 1]);
    }
    assign0(curr);

    printf("Accpeted input of \n");
    printANode(curr);

    struct node goal;
    for (int i = 1; i < NxN; i++)
        goal.tiles[(i-1)/N][(i-1)%N] = i;
    goal.tiles[N-1][N-1] = 0;

    goal.row0 = N-1;
    goal.col0 = N-1;

    printf("Goal of \n");
    printANode(&goal);

    if (compareNodes(curr,&goal))
    {
        printf("Goal and Initial state are the same, puzzle is solved\n");
        return 0;
    }

    open = curr;
    //closed = curr;
    printf("Starting to solve\n");
    while (!compareNodes(curr,&goal) && open)
    {
        //printf("Running\n");
        printANode(curr);
        expand();
        curr = open;
    }
    if (compareNodes(curr, &goal))
    {
        printf("Done\n");
        printANode(curr);
    }
    else
    {
        printf("Failed, ended on\n");
        printANode(curr);
    }

    return 0;
}
