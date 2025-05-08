#include <stdio.h>
#include <stdlib.h>

#define N 4
#define NxN 16


typedef struct node 
{
    int tiles[N][N];
    int depth;
    struct node *parent;
    struct node *next;
};

struct node *open;

void printANode(struct node inp)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%i", inp.tiles[i][j]);
        }
        printf("\n");
    }
}
int main(int argc, char **argv)
{
    if (argc != NxN + 1)
    {
        printf("Invalid amount of arguements provided, please provide %i entries. %i provided", NxN, argc);
        return 100;
    }
    struct node init;
    for (int i = 0; i < NxN; i++)
    {
        init.tiles[i/N][i%N] = atoi(argv[i + 1]);
    }

    printANode(init);
}
