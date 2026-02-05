#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int width, height, playerX, playerY, doorX, doorY;
char input, trash;
int main()
{
    FILE *startf;
    startf = fopen("game_state.txt", "r");
    if (startf == NULL || fscanf(startf, "%c", &trash) == EOF)
    {
        /*Start random value creator*/
        srand(time(NULL));
        /*New board*/
        if (startf == NULL)
        {
            printf("Generating a new board...\n");
        }
        /*Take input*/
        printf("Enter the width and height of board: ");
        scanf("%d %d", &width, &height);
        /*width and height must be greater than 0*/
        if (width < 0 || height < 0)
        {
            printf("Width and height must be greater than 0.\n");
            return 1;
        }
        /*Give four random numbers*/
        playerX = (rand() % width + 1);
        playerY = (rand() % height + 1);
        doorX = (rand() % width + 1);
        doorY = (rand() % height + 1);
        printf("Width: %d, Height: %d Player: (%d,%d), Door: (%d,%d)\n", width, height, playerX, playerY, doorX, doorY);
        /*File records*/
        FILE *fp;
        fp = fopen("game_state.txt", "w");
        fprintf(fp, "%d %d %d %d %d %d", width, height, playerX, playerY, doorX, doorY);
        fclose(fp);
    }
    else
    {
        /*Data control from file*/
        FILE *controlf;
        controlf = fopen("game_state.txt", "r");
        fscanf(controlf, "%d %d %d %d %d %d", &width, &height, &playerX, &playerY, &doorX, &doorY);
        fclose(controlf);

        printf("Player: (%d,%d), Door: (%d,%d)\n", playerX, playerY, doorX, doorY);
    }
    /*Win and start if random player coordinates equal to random door coordinates.*/
    if (doorX == playerX && doorY == playerY)
    {
        printf("Congratulations! You escaped!\n");
        printf("Generating a new board...\n");

        /*To start file in "w" mode revome to file concent*/
        FILE *closef;
        closef = fopen("game_state.txt", "w");
        if (closef != NULL)
        {
            fclose(closef);
        }
        return 1;
    }
    else
    {
        /*Take movement info from user*/
        printf("Move (WASD): ");
        scanf(" %c", &input);

        /*Steps*/
        if (input == 'A')
        {
            playerX--;
            if (playerX < 1)
            {
                printf("The player cannot pass through the wall!\n");
                playerX++;
            }
        }
        else if (input == 'D')
        {
            playerX++;
            if (playerX > width)
            {
                printf("The player cannot pass through the wall!\n");
                playerX--;
            }
        }
        else if (input == 'W')
        {
            playerY++;
            if (playerY > height)
            {
                printf("The player cannot pass through the wall!\n");
                playerY--;
            }
        }
        else if (input == 'S')
        {
            playerY--;
            if (playerY < 1)
            {
                printf("The player cannot pass through the wall!\n");
                playerY++;
            }
        }
        else
        {
            printf("Unvalid movement info.Please use big characther.\n");
        }
        printf("Game continues...\n");
    }
    /*Win and start*/
    if (doorX == playerX && doorY == playerY)
    {
        printf("Congratulations! You escaped!\n");
        printf("Generating a new board...\n");

        /*To start file in "w" mode revome to file concent*/
        FILE *closef;
        closef = fopen("game_state.txt", "w");
        if (closef != NULL)
        {
            fclose(closef);
        }
        return 1;
    }
    /*Datas are saved to file*/
    FILE *savef;
    savef = fopen("game_state.txt", "w");
    if (savef != NULL)
    {
        fprintf(savef, "%d %d %d %d %d %d", width, height, playerX, playerY, doorX, doorY);
        fclose(savef);
    }
    return 0;
}