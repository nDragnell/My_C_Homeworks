#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MOVES 100

int move_rows[MAX_MOVES];
int move_cols[MAX_MOVES];
int move_top = 0;

void push(int row, int col) {
    if (move_top < MAX_MOVES) {
        move_rows[move_top] = row;
        move_cols[move_top] = col;
        move_top++;
    }
}

void pop() {
    if (move_top > 0) move_top--;
}

int isEmpty() {
    return move_top == 0;
}

void areaFiller(int** area, int size) {
    int i, j;
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            area[i][j] = 0;
}

void randomMinePlacer(int** area, int size) {
    int minesTimes,count=0;
    minesTimes = (size * size) / 4;
    while (count < minesTimes) {
        int row,col;
        row = rand() % size;
        col = rand() % size;
        if (area[row][col] != -1) {
            area[row][col] = -1;
            count++;
        }
    }
}

void valuePlacer(int** area, int size) {
    int i, j, k, l;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (area[i][j] == -1) 
                continue;
            int count;
            count = 0;
            for (k = -1; k <= 1; k++) {
                for (l = -1; l <= 1; l++) {
                    int ni,nj;
                    ni = i + k;
                    nj = j + l;
                    if (ni >= 0 && ni < size && nj >= 0 && nj < size && area[ni][nj] == -1)
                        count++;
                }
            }
            area[i][j] = count;
        }
    }
}

void printBoard(int** visible, int size) {
    int i, j;
    printf("\n    ");
    for (j = 0; j < size; j++) {
        printf("%2d ", j);
    }
    printf("\n   ");
    for (j = 0; j < size; j++) {
        printf("---");
    }
    printf("\n");

    for (i = 0; i < size; i++) {
        printf("%2d| ", i); 
        for (j = 0; j < size; j++) {
            if (visible[i][j] == -2) printf("#  ");
            else if (visible[i][j] == -1) printf("X  ");
            else printf("%d  ", visible[i][j]);
        }
        printf("\n");
    }
}

void floodFill(int** area, int** visible, int size, int row, int col) {
    int i, j;
    if (row < 0 || row >= size || col < 0 || col >= size) return;
    if (visible[row][col] != -2) return;

    visible[row][col] = area[row][col];
    if (area[row][col] == 0) {
        for (i = -1; i <= 1; i++)
            for (j = -1; j <= 1; j++)
                if (!(i == 0 && j == 0))
                    floodFill(area, visible, size, row + i, col + j);
    }
}

int myStrCmp(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        if (*s1 != *s2) return 0;
        s1++; s2++;
    }
    return *s1 == '\0' && *s2 == '\0';
}

int main() {
    srand(time(NULL));
    int i,j;
    int size = (rand() % 9) + 2;
    int** area = (int**)malloc(size * sizeof(int*));
    int** areaVisible = (int**)malloc(size * sizeof(int*));
    for (i = 0; i < size; i++) {
        area[i] = (int*)malloc(size * sizeof(int));
        areaVisible[i] = (int*)malloc(size * sizeof(int));
        for (j = 0; j < size; j++)
            areaVisible[i][j] = -2; 
    }

    areaFiller(area, size);
    randomMinePlacer(area, size);
    valuePlacer(area, size);

    FILE* mapFile = fopen("map.txt", "w");
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if(area[i][j] <0) 
                fprintf(mapFile, " * ");
            else
                fprintf(mapFile, " . ");
        }
        fprintf(mapFile, "\n");
    }
    fclose(mapFile);

    char input[10];
    int row, col;
    int gameOver = 0;

    while (!gameOver) {
        printBoard(areaVisible, size);
        printf("Enter your move (row col) or 'undo': ");
        fgets(input, sizeof(input), stdin);

        if (myStrCmp(input, "undo\n"))
        {
            if (!isEmpty()) {
                pop();
                for (i = 0; i < size; i++)
                    for (j = 0; j < size; j++)
                        areaVisible[i][j] = -2;
                for (i = 0; i < move_top; i++) {
                    int r,c;
                    r = move_rows[i];
                    c = move_cols[i];
                    floodFill(area, areaVisible, size, r, c);
                }
            } else {
                printf("No moves to undo.\n");
            }
        } 
        else {
            if (sscanf(input, "%d %d", &row, &col) != 2 || row < 0 || row >= size || col < 0 || col >= size) {
                printf("Invalid input.\n");
                continue;
            }
            if (area[row][col] == -1) {
                areaVisible[row][col] = -1;
                printBoard(areaVisible, size);
                printf("BOOM! You hit a mine. Game Over.\n");
                gameOver = 1;
            } 
            else {
                push(row, col);
                floodFill(area, areaVisible, size, row, col);

                int opened = 0, total = size * size;
                int mines = (size * size) / 4;
            
                for (i = 0; i < size; i++) {
                    for (j = 0; j < size; j++) {
                        if (areaVisible[i][j] != -2 && areaVisible[i][j] != -1)
                            opened++;
                    }
                }
                if (opened == total - mines) {
                    printBoard(areaVisible, size);
                    printf("Congratulations! You win!\n");
                    break; 
                }
            }
            
        }
    }

    FILE* moveFile = fopen("moves.txt", "w");
    fprintf(moveFile, "    ---GAME MOVES---\n");
    for (i = 0; i < move_top; i++)
        fprintf(moveFile, "Move %d: [Row: %d, Column: %d]\n",i+1, move_rows[i], move_cols[i]);
    fprintf(moveFile, "Total Moves: %d\n",i);
    fclose(moveFile);

    for (i = 0; i < size; i++) {
        free(area[i]);
        free(areaVisible[i]);
    }
    free(area);
    free(areaVisible);
    return 0;
}