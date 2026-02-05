#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum columns{
    O= 'O',
    X= 'X',
    EMPTY= '.',
}col;

typedef struct area{
    char** board;
    int areaLen;
    char userC;
    char compC;
    int inpRow;
    int inpCol;
    int directions[8]; 
}area;

void areaFiller(area* b);
void areaPrinter(area* a);
int getUserMove(area *a, char** inp, int areaLen, int *counter);
void playToMove(area *a, char player, char opponent, int directions[8]);
void charToInt(area *a, char* inp);
int isMoveLegal(area *a, char player, char opponent, int *counter);
int playComputerMove(area *a, int *counter);
void cellCounter(area *a);

int main(){
    area *a;
    char *inp;
    int size,i;
    int flag;
    flag=0;
    int isFinish;
    isFinish = 1;
    int *counter;

    counter = (int*)malloc(sizeof(int));
    a=(area*)malloc(sizeof(area));
    inp = (char*)malloc(sizeof(char)); 

    a->compC=X;
    a->userC=O;

    printf("Welcome to the Game!\n\n");
    printf("%c repesents the computer cells and %c represents the user cells\n",a->compC,a->userC);

    do{
        printf("How much size have board?\n");
        scanf("%d",&size);

        if(size%2==0&&3<size&&size<21){

            printf("New board size %dx%d\n",size,size);
            a->areaLen=size;

            flag=1;
        }

        else{
            printf("Please enter size again!\n");
        }

    }
    while(!flag);

    areaFiller(a);
    areaPrinter(a);
     
    for(i=1;i<a->areaLen*a->areaLen/2-1;i++){
        printf("--------------------------------\nTURN %d\n", i);
        isFinish=getUserMove(a, &inp, a->areaLen, counter);
        printf("\nYour move is: %s\n", inp);
        areaPrinter(a);

        isFinish=playComputerMove(a, counter);
        areaPrinter(a);
        if(isFinish == 0) {
            printf("Game Over! No more legal moves available.\n");
            cellCounter(a);
            return 0;
        }
    }

    free(counter);
    free(inp);
    for(i=0;i<a->areaLen;i++)
        free(*(a->board+i));
    free(a->board);

    return 0;
}

void areaFiller(area* a){
    int i,j;

    char **tempBoard;
    tempBoard=(char**)malloc((a->areaLen)*sizeof(char*));
    a->board=tempBoard;

    for(i=0;i<a->areaLen;i++){

        *(tempBoard+i)=(char*)malloc(a->areaLen);

        for(j=0;j<a->areaLen;j++){

            if((i==(a->areaLen/2-1))&&(j==(a->areaLen/2-1))){
                *(*(tempBoard+i)+j)=a->compC;
            }

            else if((i==(a->areaLen/2))&&(j==(a->areaLen/2))){
                *(*(tempBoard+i)+j)=a->compC;
            }

            else if((i==(a->areaLen/2-1))&&(j==(a->areaLen/2))){
                *(*(tempBoard+i)+j)=a->userC;
            }

            else if((i==(a->areaLen/2))&&(j==(a->areaLen/2-1))){
                *(*(tempBoard+i)+j)=a->userC;
            }

            else{
                *(*(tempBoard+i)+j)=EMPTY;
            }

        }
    }
}

void areaPrinter(area* a){
    int i,j;
    char row0='a';

    printf("   ");

    for(j=0;j<a->areaLen;j++){
            
        printf("%c  ",row0+j);
    }
    printf("\n");
    
    for(i=0;i<a->areaLen;i++){
        if(i>8){
            printf("%d ",i+1);
        }
        else{
            printf("%d  ",i+1);
        }

        for(j=0;j<a->areaLen;j++){
            printf("%c  ",*(*(a->board+i)+j));
        }

        if(i>8){
            printf("%d ",i+1);
        }
        else{
            printf("%d  ",i+1);
        }

        printf("\n");
    }

    printf("   ");
    for(j=0;j<a->areaLen;j++){
            
        printf("%c  ",row0+j);
    }

    printf("\n");
}

int getUserMove(area *a, char** inp, int areaLen, int *counter) {
    char temp;
    char ch;
    int isLegal,val,reminder,flag,i,j;
    flag=1;
    do{
        printf("Please enter your move (e.g., 4b): ");
        scanf("%d%c",&val,&temp);
        while (getchar() != '\n');

        if(0<val&&val<areaLen+1 && 'a'<=temp&&temp<'a'+areaLen)
            flag=0;
    }
    while(flag);

    if (val < 10) {
        *inp = realloc(*inp, 3);  
        sprintf(*inp, "%d%c", val, temp); 
    } 
    else {
        *inp = realloc(*inp, 4);  
        sprintf(*inp, "%d%c", val, temp); 
    }

    charToInt(a,*inp);
    
    isLegal=isMoveLegal(a, a->userC, a->compC, counter);
    
    if(isLegal==1){
        playToMove(a, a->userC, a->compC, a->directions);
        return 1;
    }
    else{
        for(i=0;i<a->areaLen-1;i++){

            for(j=0;j<a->areaLen-1;j++){
                a->inpRow=i;
                a->inpCol=j;

                if(isMoveLegal(a, a->compC, a->userC, counter)){
                    flag=1;
                }
            }
        }
        if(flag==1){
            printf("The move is not legal! Please enter again your move.\n");
            return getUserMove(a, inp,areaLen, counter);
        }
        else{
            printf("The player has legal move.\n");
            return 0;
        }
    } 
}

int isMoveLegal(area *a, char player, char opponent, int *counter) {
    int i,r,c;
    int directions[8]={0}; 

    r=a->inpRow;
    c=a->inpCol;
    *counter = 1;
    
    if(a->board[r][c]!=EMPTY){
        return 0; 
    }

    for(i=0;i<a->areaLen-1;i++){
        if(c-1>=0 && a->board[r][c-1]==opponent && directions[0]==0){

            if(c-i-1>=0 && a->board[r][c-i-1]==player){
                directions[0]=1;
            }

            else if(c-i-1>=0 && a->board[r][c-i-1]==player){

                (*counter)++;
            }

            else{
                directions[0]=-1;
            }
        }

        if(c+1<a->areaLen &&a->board[r][c+1]==opponent  && directions[1]==0){

            if(c+i+1 < a->areaLen && a->board[r][c+i+1]==player){
                directions[1]=1;
            }

            else if(c+i+1 < a->areaLen && a->board[r][c+i+1]==opponent){

                (*counter)++;
            }

            else{
                directions[1]=-1;
            }
        }

        if(r-1>=0 && a->board[r-1][c]==opponent && directions[2]==0){

            if(r-i-1>=0 && a->board[r-i-1][c]==player){
                directions[2]=1;
            }

            else if(r-i-1>=0 && a->board[r-i-1][c]==opponent){

                (*counter)++;
            }

            else{
                directions[2]=-1;
            }
        }

        if(r+1 < a->areaLen && a->board[r+1][c]==opponent && directions[3]==0){

            if(c+i+1 < a->areaLen && a->board[r+i+1][c]==player ){
                directions[3]=1;
            }

            else if(c+i+1 < a->areaLen && a->board[r+i+1][c]==opponent){

                (*counter)++;
            }

            else{
                directions[3]=-1;
            }
        }
        
        if(r-1>=0 && c-1>=0 && a->board[r-1][c-1]==opponent && directions[4]==0){

            if(c-i-1>=0 && r-i-1>=0 && a->board[r-i-1][c-i-1]==player){
                directions[4]=1;
            }

            else if(c-i-1>=0 && r-i-1>=0 && a->board[r-i-1][c-i-1]==player){

                (*counter)++;
            }

            else{
                directions[4]=-1;
            }
        }
        
        if(r+1 < a->areaLen && c+1 < a->areaLen && a->board[r+1][c+1]==opponent && directions[5]==0){

            if(c+i+1 < a->areaLen && r+i+1 < a->areaLen && a->board[r+i+1][c+i+1]==player){
                directions[5]=1;
            }

            else if(c+i+1 < a->areaLen && r+i+1 < a->areaLen && a->board[r+i+1][c+i+1]==opponent){

                (*counter)++;
            }

            else{
                directions[5]=-1;
            }
        }

        if(r+1 < a->areaLen && c-1>=0 && a->board[r+1][c-1]==opponent && directions[6]==0){

            if(c-i-1>=0 && r+i+1 < a->areaLen && a->board[r+i+1][c-i-1]==player){
                directions[6]=1;
            }

            else if(c-i-1>=0 && r+i+1 < a->areaLen && a->board[r+i+1][c-i-1]==opponent){

                (*counter)++;
            }

            else{
                directions[6]=-1;
            }
        }

        if(r-1>=0 && r+1 < a->areaLen && a->board[r-1][c+1]==opponent && directions[7]==0){

            if(c+i+1 < a->areaLen && r-i-1>=0 && a->board[r-i-1][c+i+1]==player){
                directions[7]=1;
            }

            else if(c+i+1 < a->areaLen && r-i-1>=0 && a->board[r-i-1][c+i+1]==opponent){

                (*counter)++;
            }

            else{
                directions[7]=-1;
            }
        }

    }

    memcpy(a->directions, directions, sizeof(directions));

    if(directions[0]==1 ||
    directions[1]==1 ||
    directions[2]==1 ||
    directions[3]==1 ||
    directions[4]==1 ||
    directions[5]==1 ||
    directions[6]==1 ||
    directions[7]==1){
        a->inpCol=c;
        a->inpRow=r;
        return 1; 
    }
    else{
        return 0; 
    }
}

void playToMove(area *a, char player, char opponent, int directions[8]) {
    int i;
    int r,c;
    r = a->inpRow;
    c = a->inpCol;
    printf("C: %d\n",c);

    if(directions[0]==1)
        for(i=0; (r) < a->areaLen && (c-i-1) >= 0 ;i++)
            if(a->board[r][c-i-1]==opponent)
                a->board[r][c-i-1]=player;
            else 
                break;

    if(directions[1]==1)
        for(i=0; (r) < a->areaLen && (c+i+1) < a->areaLen ;i++)
            if(a->board[r][c+i+1]==opponent)
                a->board[r][c+i+1]=player;
            else 
                break;

    if(directions[2]==1)
        for(i=0; (r-i-1) >= 0 && (c) < a->areaLen ;i++)
            if(a->board[r-i-1][c]==opponent)
                a->board[r-i-1][c]=player;
            else 
                break;

    if(directions[3]==1)
        for(i=0; (r+i+1) < a->areaLen && (c) < a->areaLen ;i++)
            if(a->board[r+i+1][c]==opponent)
                a->board[r+i+1][c]=player;
            else 
                break;

    if(directions[4]==1)
        for(i=0; (r-i-1) >= 0 && (c-i-1) >= 0 ;i++)
            if(a->board[r-i-1][c-i-1]==opponent)
                a->board[r-i-1][c-i-1]=player;
            else 
                break;

    if(directions[5]==1)
        for(i=0; (r+i+1) < a->areaLen && (c+i+1) < a->areaLen ;i++)
            if(a->board[r+i+1][c+i+1]==opponent)
                a->board[r+i+1][c+i+1]=player;
            else 
                break;

    if(directions[6]==1)
        for(i=0; (r+i+1) < a->areaLen && (c-i-1) >= 0 ;i++)
            if(a->board[r+i+1][c-i-1]==opponent)
                a->board[r+i+1][c-i-1]=player;
            else 
                break;

    if(directions[7]==1)
        for(i=0; (r-i-1) >= 0 && (c+i+1) < a->areaLen ;i++)
            if(a->board[r-i-1][c+i+1]==opponent)
                a->board[r-i-1][c+i+1]=player;
            else 
                break;

    a->board[a->inpRow][a->inpCol]=player;
}

void charToInt(area *a, char* inp){

    if(strlen(inp)==3){

        a->inpRow = (inp[0] - '0') * 10 + (inp[1] - '0')-1;
        a->inpCol = inp[2] - 'a';
    }

    else if(strlen(inp)==2){

        a->inpRow=inp[0]-'0'-1;
        a->inpCol=inp[1]-'a';
    }

    else{
        printf("Error!!!\n");
    }
}

int playComputerMove(area *a, int *counter) {
    int i,j,curRow,curCol,curCounter;
    int bestFlags[8] = {0}; 
    curCounter=0;

    for(i=0;i<a->areaLen-1;i++){

        for(j=0;j<a->areaLen-1;j++){
            a->inpRow=i;
            a->inpCol=j;

            if(isMoveLegal(a, a->compC, a->userC, counter)){
                if(curCounter < *counter){
                    curCounter = *counter;
                    curRow = i;
                    curCol = j;
                    memcpy(bestFlags, a->directions, sizeof(bestFlags));
                }
            }
        }
    }
    if(curCounter > 0) {
        a->inpRow = curRow;
        a->inpCol = curCol;
        memcpy(a->directions, bestFlags, sizeof(bestFlags));
        playToMove(a, a->compC, a->userC, a->directions);
        printf("Computer played at: %d%c\n", a->inpRow+1, 'a' + a->inpCol);
    }
    else {
        printf("\nComputer has no legal move.\n");
        return 0; 
    }
}

void cellCounter(area *a) {
    int i, j, userCount = 0, compCount = 0;

    for (i = 0; i < a->areaLen; i++) {
        for (j = 0; j < a->areaLen; j++) {

            if (a->board[i][j] == a->userC) {
                userCount++;
            } 
            else if (a->board[i][j] == a->compC) {
                compCount++;
            }
        }
    }
    printf("User cells: %d, Computer cells: %d\n", userCount, compCount);
    
    if(userCount > compCount) {
        printf("User wins!\n");
    } 
    else if(compCount > userCount) {
        printf("Computer wins!\n");
    } 
    else {
        printf("It's a draw!\n");
    }
}