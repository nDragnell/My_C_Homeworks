#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void shipPlacer(char area[10][10],char ship4[10][10],char ship3_1[10][10],char ship3_2[10][10],char ship2[10][10],FILE *fp);/* Ship placement function */
void Placer4(char area[10][10],char ship4[10][10],FILE *fp);/* Place 4-cell ship */
void Placer3_1(char area[10][10],char ship3_1[10][10],FILE *fp);/* Place 3-cell ship 1 */
void Placer3_2(char area[10][10],char ship3_2[10][10],FILE *fp);/* Place 3-cell ship 2 */
void Placer2(char area[10][10],char ship2[10][10],FILE *fp);/* Place 2-cell ship */
void SunkController(char area[10][10],char ship4[10][10],char ship3_1[10][10],char ship3_2[10][10],char ship2[10][10]);/* Control if ships are sunk */
int gameFinishController(char area[10][10]);/* Check if the game is finished */
void gameCondititon(char area[10][10]);/* Display game board */
int main(){
    int finishFlag=1;
    do
    {
        char area [10][10],ship4[10][10],ship3_1[10][10],ship3_2[10][10],ship2[10][10]; 
        int i,j;
        for(i=0;i<10;i++){
            for(j=0;j<10;j++){
                area[i][j]='-';
            }
        }
        FILE *fp=fopen("ships.txt","w"); 
        if(fp==NULL){
            printf("Error opening file.\n");
            return -1;
        }
        fprintf(fp,"Y X S\n- - -\n");/*Header of file*/
        FILE *fp2=fopen("battleship_log.txt","w");
        if(fp2==NULL){
            printf("Error opening file.\n");
            return -1;
        }
        int shots=0;
        shipPlacer(area,ship4,ship3_1,ship3_2,ship2,fp);/* Ship placement function */
        fclose(fp);/*Close file pointer*/
        printf("Welcome to the Battleship game!\n");
        gameCondititon(area);/* Show initial game board */
        while(gameFinishController(area)){
            char ch;
            int coor[2] = {0, 0};  
            printf("Enter coordinates (X to quit): ");
            for(i = 0; i < 2;){/*Taking input*/
                ch = getchar();
                if (ch == 'X' || ch == 'x')
                    return -1;
                if('0' <= ch && ch <= '9'){
                    coor[i]=ch-'0';   
                    i++;
                }
                else if(ch==' '||ch=='\n'){
                   /* Ignore spaces and newlines */
                }
                else{
                    printf("Invalid input(%c). Please enter a number between 0 and 9.\n",ch);
                    i--;
                }
            }
            while((ch = getchar()) != '\n');/*Buffer Cleaner*/
            int x=coor[0];
            int y=coor[1];
            fprintf(fp2,"Shot: %d %d - ",y,x);
            shots++;
            if(area[y][x]=='S'){/*Hit condition*/
                printf("HITS!\n");
                fprintf(fp2,"HITS!\n");
                area[y][x]='X';
                gameCondititon(area);
                SunkController(area,ship4,ship3_1,ship3_2,ship2);
            }
            else if(area[y][x]=='-'){/*Miss condition*/
                area[y][x]='O';
                printf("MISS!\n");
                fprintf(fp2,"MISS!\n");
                gameCondititon(area);
            }
            fclose(fp2);/*Close file pointer*/
        } 
        printf("All ships are sunk! Total shots :%d",shots);
        printf("\nPress 'N to play again or 'X' to exit: ");
        char ch2;
        while(ch2=getchar()){
            if(ch2=='N'||ch2=='n'){
                finishFlag=1;/*Do while condition is true*/
                break;
            }
            else if(ch2=='X'||ch2=='x'){
                finishFlag=0;/*Do while condition is false*/
                break;
            }
            else{
                printf("Invalid input. Please enter 'N' to play again or 'X' to exit: ");
            }
        }
        
        
    }
    while(finishFlag);
    return 0;
}
/*      if direction==0-->right
        if direction==1-->down
        if direction==2-->left
        if direction==3-->up    */
void Placer4(char area[10][10],char ship4[10][10],FILE *fp){/*Placer of ship 4*/
    int i,shipx4,shipy4,direction4;
    direction4 = (rand() % 4);/*random direction*/
    shipy4 = (rand() % 4)+3;/*random ship x axis value at between [3,6]*/
    shipx4 = (rand() % 4)+3;/*random ship y axis value at between [3,6]*/
    if(direction4==0)/*Place of ship 4*/
        for(i=0;i<4;i++){
            area[shipy4][shipx4+i]='S';/*To place of ships refer to random values*/
            ship4[shipy4][shipx4+i]='S';/*To control place of ships for sunk condition*/
            fprintf(fp,"%d %d 4\n",shipy4,shipx4+i);
        }
    if(direction4==2)
        for(i=0;i<4;i++){
            area[shipy4][shipx4-i]='S';
            ship4[shipy4][shipx4-i]='S';
            fprintf(fp,"%d %d 4\n",shipy4,shipx4-i);
    }
    if(direction4==1)
        for(i=0;i<4;i++){
            area[shipy4-i][shipx4]='S';
            ship4[shipy4-i][shipx4]='S';
            fprintf(fp,"%d %d 4\n",shipy4-i,shipx4);
        }
    if(direction4==3)
        for(i=0;i<4;i++){
            area[shipy4+i][shipx4]='S';
            ship4[shipy4+i][shipx4]='S';
            fprintf(fp,"%d %d 4\n",shipy4+i,shipx4);
        }
}
/**/
void Placer3_1(char area[10][10],char ship3_1[10][10],FILE *fp){
    int i,j,shipx3_1,shipy3_1,direction3_1;
    direction3_1 = (rand() % 4);
    shipy3_1 = (rand() % 6)+2;
    shipx3_1 = (rand() % 6)+2;

    if(direction3_1==0){/*Place of ship 3_1*/
        for(i=0;i<3;i++){
            if((area[shipy3_1][shipx3_1+i])=='S'){/*If there is any ship, again place if*/
                for(j=0;j<i;j++){
                    area[shipy3_1][shipx3_1+j]='-';/*To delete wrong ships*/
                }
                return Placer3_1(area,ship3_1,fp);/*Recursive function*/
            } 
            else{
                area[shipy3_1][shipx3_1+i]='S';
                ship3_1[shipy3_1][shipx3_1+i]='S';
                fprintf(fp,"%d %d 3\n",shipy3_1,shipx3_1+i);
            }
        }
    }
    if(direction3_1==2){
        for(i=0;i<3;i++){
            if((area[shipy3_1][shipx3_1-i])=='S'){
                for(j=0;j<i;j++){
                    area[shipy3_1][shipx3_1-j]='-';
                }
                return Placer3_1(area,ship3_1,fp);
            }
            else{
                area[shipy3_1][shipx3_1-i]='S';
                ship3_1[shipy3_1][shipx3_1-i]='S';
                fprintf(fp,"%d %d 3\n",shipy3_1,shipx3_1-i);
            }
        }
    }
    if(direction3_1==1){
        for(i=0;i<3;i++){
            if((area[shipy3_1-i][shipx3_1])=='S'){
                for(j=0;j<i;j++){
                    area[shipy3_1-j][shipx3_1]='-';
                }
                return Placer3_1(area,ship3_1,fp);
            }
            else{
                area[shipy3_1-i][shipx3_1]='S';
                ship3_1[shipy3_1-i][shipx3_1]='S';
                fprintf(fp,"%d %d 3\n",shipy3_1-i,shipx3_1);
            }
        }
    }
    if(direction3_1==3){
        for(i=0;i<3;i++){
            if((area[shipy3_1+i][shipx3_1])=='S'){
                for(j=0;j<i;j++){
                    area[shipy3_1+j][shipx3_1]='-';
                }
                return Placer3_1(area,ship3_1,fp);
            }
            else{
                area[shipy3_1+i][shipx3_1]='S';
                ship3_1[shipy3_1+i][shipx3_1]='S';
                fprintf(fp,"%d %d 3\n",shipy3_1+i,shipx3_1);
            }
        }
    }
}
/**/
void Placer3_2(char area[10][10],char ship3_2[10][10],FILE *fp){
    int i,j,shipx3_2,shipy3_2,direction3_2;
    direction3_2 = (rand() % 4);
    shipy3_2 = (rand() % 6)+2;  
    shipx3_2 = (rand() % 6)+2;
    if(direction3_2==0){/*Place of ship 3_2*/
        for(i=0;i<3;i++){
            if((area[shipy3_2][shipx3_2+i])=='S'){
                for(j=0;j<i;j++){
                    area[shipy3_2][shipx3_2+j]='-';
                }
                return Placer3_2(area,ship3_2,fp);
            } 
            else{
                area[shipy3_2][shipx3_2+i]='S';
                ship3_2[shipy3_2][shipx3_2+i]='S';
                fprintf(fp,"%d %d 3\n",shipy3_2,shipx3_2+i);
            }
        }
    }
    if(direction3_2==2){
        for(i=0;i<3;i++){
            if((area[shipy3_2][shipx3_2-i])=='S'){
                for(j=0;j<i;j++){
                    area[shipy3_2][shipx3_2-j]='-';
                }
                return Placer3_2(area,ship3_2,fp);
            }
            else{
                area[shipy3_2][shipx3_2-i]='S';
                ship3_2[shipy3_2][shipx3_2-i]='S';
                fprintf(fp,"%d %d 3\n",shipy3_2,shipx3_2-i);
            }
        }
    }
    if(direction3_2==1){
        for(i=0;i<3;i++){
            if((area[shipy3_2-i][shipx3_2])=='S'){
                for(j=0;j<i;j++){
                    area[shipy3_2-j][shipx3_2]='-';
                }
                return Placer3_2(area,ship3_2,fp);
            }
            else{
            area[shipy3_2-i][shipx3_2]='S';
            ship3_2[shipy3_2-i][shipx3_2]='S';
            fprintf(fp,"%d %d 3\n",shipy3_2-i,shipx3_2);
            }
        }
    }
    if(direction3_2==3){
        for(i=0;i<3;i++){
            if((area[shipy3_2+i][shipx3_2])=='S'){
                for(j=0;j<i;j++){
                    area[shipy3_2+j][shipx3_2]='-';
                }
                return Placer3_2(area,ship3_2,fp);
            }
            else{
                area[shipy3_2+i][shipx3_2]='S';
                ship3_2[shipy3_2+i][shipx3_2]='S';
                fprintf(fp,"%d %d 3\n",shipy3_2+i,shipx3_2);
            }
        }
    }
}
/**/
void Placer2(char area[10][10],char ship2[10][10],FILE *fp){
    int i,j,shipx2,shipy2,direction2;
    direction2 = (rand() % 4);
    shipy2 = (rand() % 8)+1;
    shipx2 = (rand() % 8)+1;
    if(direction2==0){/*Place of ship 2*/
        for(i=0;i<2;i++){
            if((area[shipy2][shipx2+i])=='S'){
                for(j=0;j<i;j++){
                    area[shipy2][shipx2+j]='-';
                }
                return Placer2(area,ship2,fp);
            } 
            else{
                area[shipy2][shipx2+i]='S';
                ship2[shipy2][shipx2+i]='S';
                fprintf(fp,"%d %d 2\n",shipy2,shipx2+i);
            }
        }
    }
    if(direction2==2){
        for(i=0;i<2;i++){
            if((area[shipy2][shipx2-i])=='S'){
                for(j=0;j<i;j++){
                    area[shipy2][shipx2-j]='-';
                }
                return Placer2(area,ship2,fp);
            }
            else{
                area[shipy2][shipx2-i]='S';
                ship2[shipy2][shipx2-i]='S';
                fprintf(fp,"%d %d 2\n",shipy2,shipx2-i);
            }
        }
    }
    if(direction2==1){
        for(i=0;i<2;i++){
            if((area[shipy2-i][shipx2])=='S'){
                for(j=0;j<i;j++){
                    area[shipy2-j][shipx2]='-';
                }
                return Placer2(area,ship2,fp); 
            }
            else{
                area[shipy2-i][shipx2]='S';
                ship2[shipy2-i][shipx2]='S';
                fprintf(fp,"%d %d 2\n",shipy2-i,shipx2);
            }
        }    
    }
    if(direction2==3){
        for(i=0;i<2;i++){
            if((area[shipy2+i][shipx2])=='S'){
                for(j=0;j<i;j++){
                    area[shipy2+j][shipx2]='-';
                }
                return Placer2(area,ship2,fp);
            }
            else{
                area[shipy2+i][shipx2]='S';
                ship2[shipy2+i][shipx2]='S';
                fprintf(fp,"%d %d 2\n",shipy2+i,shipx2);
            }
        }
    }
}
void shipPlacer(char area[10][10],char ship4[10][10],char ship3_1[10][10],char ship3_2[10][10],char ship2[10][10],FILE *fp){
    srand(time(NULL));/*Start random value creator*/
    Placer4(area,ship4,fp);/*For size 4*/
    Placer3_1(area,ship3_1,fp);/*For size 3_1*/
    Placer3_2(area,ship3_2,fp);/*For size 3_2*/
    Placer2(area,ship2,fp);/*For size 2*/
}
void SunkController(char area[10][10],char ship4[10][10],char ship3_1[10][10],char ship3_2[10][10],char ship2[10][10]){
    int i,j,count4=4,count3_1=3,count3_2=3,count2=2;
    for(i=0;i<10;i++){/*Size Control Counter*/
        for(j=0;j<10;j++){
            if(area[i][j]=='X'&&ship4[i][j]=='S'){
                count4--;
                if(count4==0){
                    printf("Congratulations! You sank a 4-cell ship!\n");
                    count4=4;/*counter reset*/
                    ship4[i][j]='-';/*imposible rewrite */
                }
            }
            else if(area[i][j]=='X'&&ship3_1[i][j]=='S'){
                count3_1--;
                if(count3_1==0){
                    printf("Congratulations! You sank a 3-cell ship!\n");
                    count3_1=3;/*counter reset*/
                    ship3_1[i][j]='-';/*imposible rewrite */
                }
            }
            else if(area[i][j]=='X'&&ship3_2[i][j]=='S'){
                count3_2--;
                if(count3_2==0){
                    printf("Congratulations! You sank a 3-cell ship!\n");
                    count3_2=3;/*counter reset*/
                    ship3_2[i][j]='-';/*imposible rewrite */
                }
            }
            else if(area[i][j]=='X'&&ship2[i][j]=='S'){
                count2--;
                if(count2==0){
                    printf("Congratulations! You sank a 2-cell ship!\n");
                    count2=2;/*counter reset*/
                    ship2[i][j]='-';/*imposible rewrite */
                }
            }
        }
    }
}
int gameFinishController(char area[10][10]){
    int i,j;
    for(j=0;j<10;j++){
        for(i=0;i<10;i++){
            if(area[j][i]=='S')
                return 1;/*İf there is any ship in the area,game is continue*/
        }
    }
    return 0;/*There is no ships*/
}
void gameCondititon(char area[10][10]){
    int i,j;
    printf("  ");
    for(i=0;i<10;i++)
        printf("%d ",i);
    printf("\n");
    for(i=0;i<10;i++){
        printf("%d ",i);
        for( j=0;j<10;j++){/*Print area to user*/
            if(area[i][j]=='S')
            {
                printf("- ");/*To hide ships*/
            }
            else  
                printf("%c ",area[i][j]);
        }
        printf("\n");
    }
}