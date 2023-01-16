

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

int **create2DTable(int x, int y)
{
    int **tab = (int **)malloc(x * sizeof(int *));
    for (int i = 0; i < x; i++)
    {
        tab[i] = (int *)malloc(y * sizeof(int));
    }
    return tab;
}

void destroy2DTable(int **tab, int x)
{
    for (int i = 0; i < x; i++)
    {
        free(tab[i]);
    }
    free(tab);
}

int drawBoard(int **plansza, int **tab_bool_odkryte, int height, int width, int bombCount)
{

    int tilesToWin = (height) * (width)-bombCount;
    int toReturn = 0;
    for (int i = 0; i < height + 1; i++)
    {
        for (int j = 0; j < width + 1; j++)
        {
            if (i == 0)
            {
                if (j == 0)
                {
                    printf("   ");
                }
                else
                {
                    if(j<11){
                        printf(" %d ", j-1);
                    }
                    else{
                        printf("%d ", j-1);
                    }
                }
                continue;
            }
            if (j == 0)
            {
                if(i<11){
                    printf(" %d ", i-1);
                }
                else{
                    printf("%d ", i-1);
                }
                continue;
            }

            if (tab_bool_odkryte[i - 1][j - 1] == TRUE)
            {
                if (plansza[i - 1][j - 1] > 8)
                {
                    printf("[X]");
                    toReturn = 1;
                }
                else
                {
                    if(plansza[i - 1][j - 1] == 0)
                    {
                        printf("[ ]");
                    }
                    else
                    {
                        printf("[%d]", plansza[i - 1][j - 1]);
                    }
                    tilesToWin--;
                    if (tilesToWin == 0)
                    {
                        toReturn = 2;
                    }
                }
            }
            else
            {
                printf("[?]");
            }
        }
        printf("\n");
    }
    // return 0 gra trwa dalej 1 przegrana 2 wygrana
    return toReturn;
}

void placeBombsCountNeighbours(int bombCount, int **completeBoard, int height, int width)
{
    int x;
    int y;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            completeBoard[i][j] = 0;
        }
    }
    srand(time(NULL));
    int r = rand();

    for (int b = bombCount; b > 0; b--)
    {
        y = rand() % width;
        x = rand() % height;
        if (completeBoard[x][y] < 9)
        {
            completeBoard[x][y] = 9;
        }
        else
        {
            b++;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (completeBoard[i][j] > 8)
            {
                if (i > 0)
                {
                    completeBoard[i - 1][j]++;
                }
                if (i+1 < height)
                {
                    completeBoard[i + 1][j]++;
                }
                if (j > 0)
                {
                    completeBoard[i][j - 1]++;
                }
                if (j+1 < width)
                {
                    completeBoard[i][j + 1]++;
                }
                if (i > 0 && j > 0)
                {
                    completeBoard[i - 1][j - 1]++;
                }
                if (i+1 < height && j+1 < width)
                {
                    completeBoard[i + 1][j + 1]++;
                }
                if (i+1 < height && j > 0)
                {
                    completeBoard[i + 1][j - 1]++;
                }
                if (j+1 < width && i > 0)
                {
                    completeBoard[i - 1][j + 1]++;
                }
            }
        }
    }
}

void show0(int height, int width, int **tab, int **shownToPlayer, int i, int j)
{

    if (i < 0 || i >= height)
    {
        return;
    }
    if (j < 0 || j >= width)
    {
        return;
    }
    if (shownToPlayer[i][j] == TRUE)
    {
        return;
    }
    shownToPlayer[i][j] = TRUE;
    if (tab[i][j] != 0)
    {
        return;
    }
    show0(height, width, tab, shownToPlayer, i + 1, j);
    show0(height, width, tab, shownToPlayer, i - 1, j);
    show0(height, width, tab, shownToPlayer, i - 1, j - 1);
    show0(height, width, tab, shownToPlayer, i - 1, j + 1);
    show0(height, width, tab, shownToPlayer, i, j + 1);
    show0(height, width, tab, shownToPlayer, i, j - 1);
    show0(height, width, tab, shownToPlayer, i + 1, j + 1);
    show0(height, width, tab, shownToPlayer, i + 1, j - 1);
}

int main()
{
    // początek gry
    int height;
    int width;
    int bombCount;
    printf("wybierz rozmiary planszy: \n");
    printf("szerokosc: \n");
    scanf("%d", &width);
    printf("wysokosc:\n");
    scanf("%d", &height);
    int **tab = create2DTable(height, width);
    int **shownToPlayer = create2DTable(height, width);
    printf("wybierz liczbe bomb\n");
    scanf("%d", &bombCount);

    placeBombsCountNeighbours(bombCount, tab, height, width);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            shownToPlayer[i][j] = FALSE;
        }
    }
    int repeat = 0;
    int notNeeded;
    int x, y;

    do
    {
        repeat = drawBoard(tab, shownToPlayer, height, width, bombCount);
        if (repeat == 0)
        {
            printf("wybierz kolumne i szereg (x y)\n");
            scanf("%d %d", &x, &y);

            show0(height, width, tab, shownToPlayer, y, x);
        }
        else
        {
            printf("Game Over\n");
            if (repeat == 1)
            {
                printf("You lose\n");
                for (int i = 0; i < height; i++)
                {
                    for (int j = 0; j < width; j++)
                    {
                        shownToPlayer[i][j] = TRUE;
                    }
                }
                drawBoard(tab, shownToPlayer, height, width, bombCount);
            }
            else
            {
                printf("You win");
            }
        }

    } while (repeat == 0);
        scanf("%d", &notNeeded);
}
