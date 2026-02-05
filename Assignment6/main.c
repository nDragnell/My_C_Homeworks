#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void adminMode();
void playerMode(int* (*gen_code)(int, int, int, int), int* (*get_input)(int, int, int, int));
int* generate_code(int codeLength,int min,int max,int allowDuplicates);
int* get_guess(int codeLength,int min,int max,int allowDuplicates);
int compare_code(int *codeArr,int *guessArr,int codeLength,FILE *fileL,int counterAttempt,int *score);
int displayScore(char *feedback);
void gameLog(int* codeArr,int codeLength,int min,int max,int allowDuplicates,int attempts);
int main(){
    char ch, h;
    int flag=1;
    printf("Welcome to the Codebreaker game!\n");
    do
    {
        printf("\nPlease choose a mode:\n");
        printf("A) Admin mode\n");
        printf("P) Player mode\n");
        ch = getchar();
        while ((h=getchar()) != '\n');/*To consume the newline character left in the input buffer*/
        if ('A'==ch||'a'==ch)
        {
            adminMode();
        }
        else if ('P'==ch||'p'==ch)
        {
            playerMode(generate_code,get_guess);
            flag=0;/*Set flag to 0 to exit the loop*/
        }   
        else
        {
            printf("Invalid input. Please enter A or P.\n");
        }
    }
    while(flag);

    return 0;  
}
void adminMode(){
    printf("Admin mode\n");
    int codeLength,flag=1;
    while(flag){
        printf("Please enter the code length: ");
        scanf("%d", &codeLength);
        if(codeLength<4||codeLength>10)
            printf("Invalid code length. Please enter a number between 4 and 10.\n");
        else
            flag=0;
    }
    /*Range*/
    flag=1;
    int min, max;
    while(flag){
        printf("Please enter the range of the code.\nMin:");
        scanf("%d", &min);
        printf("\nMax:");
        scanf("%d", &max);
        flag--;
        if(min<0||max>9||min>max){
            printf("Invalid range. Please enter a number between 0 and 9.\n");
            flag++;
        }
    }
    /*Duplicate*/
    flag=1;
    int allowDuplicates;
    do{
        printf("Allow duplicates? (1 for yes, 0 for no): ");
        scanf("%d", &allowDuplicates);
        if(allowDuplicates==1){
            printf("Duplicates are allowed.\n");
            flag=0;/*Set flag to 0 to exit the loop*/
        }
        else if(allowDuplicates==0){
            if(codeLength>max-min+1){
                printf("The code length is greater than the range. Duplicates are not allowed.\n");
                allowDuplicates=1;/*Set allowDuplicates to 1 to allow duplicates*/
                flag=0;/*Set flag to 0 to exit the loop*/
            }
            else
                printf("Duplicates are not allowed.\n");
                flag=0;/*Set flag to 0 to exit the loop*/
        }
        else{
            printf("Invalid input. Please enter 1 or 0.\n");
        }
    }while(flag);
    
    /*Attempt*/
    int attempts;
    do{
        printf("Please enter the number of attempts: ");
        scanf("%d", &attempts);
        if(attempts<1){
            printf("Invalid number of attempts. Please enter a number greater than 0.\n");
        }
        else{
            printf("Number of attempts: %d\n", attempts);
            flag=0;/*Set flag to 0 to exit the loop*/
        }
    }while(flag);
    
    /*Points*/
    int pointsCorrect, pointsMisplaced, penaltyWrong;
    printf("Points for correct digits: ");
    scanf("%d", &pointsCorrect);
    printf("\nPoints for misplaced digits: ");
    scanf("%d", &pointsMisplaced);
    printf("\nPenalty for wrong guesses: ");
    scanf("%d", &penaltyWrong);

    /*Save to config file*/
    FILE *file = fopen("vault_config.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return ;
    }
    fprintf(file, "CODE_LENGTH=%d\n", codeLength);
    fprintf(file, "DIGIT_MIN=%d\nDIGIT_MAX=%d\n", min, max);
    fprintf(file, "ALLOW_DUPLICATES=%d\n", allowDuplicates);
    fprintf(file, "MAX_ATTEMPTS=%d\n", attempts);
    fprintf(file, "POINTS_CORRECT=%d\n", pointsCorrect);
    fprintf(file, "POINTS_MISPLACED=%d\n", pointsMisplaced);
    fprintf(file, "PENALTY_WRONG=%d\n", penaltyWrong);
    fclose(file);
    return ;
}
void playerMode(int* (*gen_code)(int, int, int, int), int* (*get_input)(int, int, int, int)){
    
    FILE *file = fopen("vault_config.txt", "r");
    int* codeArr,*guessArr;
    int codeLength, min, max, allowDuplicates, attempts,counterAttempt=0, flag2= 1, score=0;
    char ch;

    printf("Welcome to the game!\nThe rules:\n");
    
    if (file == NULL) {
        printf("Error opening file!\n");
        return ;
    }

    while(fscanf(file, "%c", &ch) == 1) {  /* Returns 1 if successfully read a character*/
        printf("%c", ch);
    }

    rewind(file); /* go back to the beginning of the file*/

    fscanf(file, "CODE_LENGTH=%d\n", &codeLength);
    fscanf(file, "DIGIT_MIN=%d\n", &min);
    fscanf(file, "DIGIT_MAX=%d\n", &max);
    fscanf(file, "ALLOW_DUPLICATES=%d\n", &allowDuplicates);
    fscanf(file, "MAX_ATTEMPTS=%d\n", &attempts);
    
    codeArr = gen_code(codeLength, min, max, allowDuplicates); /*Generate the code*/
    /*Log the game*/
    FILE *fileL ;
    fileL= fopen("game_log.txt", "w");
    if (fileL == NULL) {
        printf("Error opening file!\n");
        return ;
    }
    fprintf(fileL, "--- Vault Codebreaker Game Log ---\n");
    fprintf(fileL, "Date: %s %s\n", __DATE__ , __TIME__);
    fprintf(fileL,"Secret Code: ");
    for(int i=0;i<codeLength;i++){
        fprintf(fileL,"%d",codeArr[i]);
    }
    fprintf(fileL,"\nCode Length: %d\n",codeLength);
    fprintf(fileL,"Digit Range: %d-%d\n",min,max);
    fprintf(fileL,"Duplicates Allowed: %d\n",allowDuplicates);
    fprintf(fileL,"Max Attempts: %d\n\n",attempts);
    printf("\nPlease enter your guess:\n");
    do
    {
        guessArr = get_input(codeLength, min, max, allowDuplicates); /*Compare the code and guess*/
        printf("\n");
        flag2 = compare_code(codeArr,guessArr,codeLength,fileL,counterAttempt,&score);
        counterAttempt++;
        if(counterAttempt==attempts){
            printf("You have reached the maximum number of attempts.\n");
            printf("The code was: ");
            for(int i=0;i<codeLength;i++){
                printf("%d",codeArr[i]);
            }
            printf("\nGame Over!\n");
            flag2=0;/*Set flag2 to 0 to exit the loop*/
            
        }
        if(flag2!=0)
            printf("\nPlease enter your guess:\n");
    } while (flag2 != 0);
    fprintf(fileL,"\nFinal Score: %d\n",score);/*Print the score to a file*/

    if(90<score){
        fprintf(fileL,"Rank: Code Master!\n");
    }
    else if(69<score){
        fprintf(fileL,"Rank: Cipher Hunter!\n");
    }
    else if(49<score){
        fprintf(fileL,"Rank: Number Sleuth!\n");
    }
    else if(29<score){
        fprintf(fileL,"Rank: Safe Kicker !\n");
    }
    else if(9<score){
        fprintf(fileL,"Rank: Lucky Breaker!\n");
    }
    else{
        fprintf(fileL,"Rank: Code Potato!\n");
    }
    fprintf(fileL,"--------------------------------\n");
    /*Free the memory*/
    free(codeArr);
    free(guessArr);
    /*Close the file*/
    fclose(file);
    fclose(fileL);
}
int* generate_code(int codeLength,int min,int max,int allowDuplicates){
    FILE *file = fopen("vault_config.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return NULL;
    }

    fscanf(file, "CODE_LENGTH=%d\n", &codeLength);
    fscanf(file, "DIGIT_MIN=%d\n", &min);
    fscanf(file, "DIGIT_MAX=%d\n", &max);
    fscanf(file, "ALLOW_DUPLICATES=%d\n", &allowDuplicates);
    fclose(file);

    int *codeArr=(int*)malloc(codeLength*sizeof(int));/*Dynamic memory allocation*/

    srand(time(NULL));/*Start random value creator*/

    int randomValue,i,j;
    for(i=0;i<codeLength;i++){
        randomValue=(rand()%(max-min+1))+min;

        if(allowDuplicates==0){

            for(j=0;j<i;j++){

                if(codeArr[j]==randomValue){
                    randomValue=(rand()%(max-min+1))+min;
                    j=-1;/*Reset j to -1 to check all previous values again*/
                }
            }
        }
        codeArr[i]=randomValue;
    }
    /*Print the code to a file*/
    FILE *file1 = fopen("vault_code.txt", "w");
    for(i=0;i<codeLength;i++){
        fprintf(file1,"%d",codeArr[i]);
    }
    fclose(file1);

    return codeArr;
}
int* get_guess(int codeLength,int min,int max,int allowDuplicates){
    FILE *file = fopen("vault_config.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return NULL;
    }

    fscanf(file, "CODE_LENGTH=%d\n", &codeLength);
    fscanf(file, "DIGIT_MIN=%d\n", &min);
    fscanf(file, "DIGIT_MAX=%d\n", &max);
    fscanf(file, "ALLOW_DUPLICATES=%d\n", &allowDuplicates);
    fclose(file);

    int *guessArr=(int*)malloc(codeLength*sizeof(int));/*Dynamic memory allocation*/

    char ch;
    int i,flag=1,digit;
    do {
        i = 0;
        
        while ((ch = getchar()) != '\n' && ch != EOF) { /*To consume the newline character left in the input buffer*/
            if (ch == '\0' || ch == ' ') {
                continue;
            }

            digit = ch - '0'; /*Convert char to int*/

            if (digit < min || digit > max) {
                printf("Invalid guess. Please enter a number between %d and %d.\n", min, max);
                i = 0;
                while ((ch = getchar()) != '\n' && ch != EOF); /*To consume the newline character left in the input buffer*/
                break;
            }

            if (allowDuplicates == 0) {
                int duplicate = 0;
                for (int j = 0; j < i; j++) {
                    if (guessArr[j] == digit) {
                        duplicate = 1;
                        break;
                    }
                }
                if (duplicate) {
                    printf("Invalid guess. Please enter a unique number.\n");
                    i = 0;
                    while ((ch = getchar()) != '\n' && ch != EOF);
                    break;
                }
            }

            guessArr[i] = digit; /*Store the guess in the array*/
            
            
            i++;
        }

        if (i != codeLength) {
            printf("Invalid guess. %d digits entered. Please enter a number of length %d.\n", i, codeLength);
            i = 0; /*Reset i to 0 to check all previous values again*/
        } 
        
        else {
            flag = 0; /*Set flag to 0 to exit the loop*/
        }

    } 
    while (flag); /*Check if the guess is valid*/

    return guessArr;   
}
int compare_code(int *codeArr,int *guessArr,int codeLength,FILE *fileL,int counterAttempt, int *score){/*Compare the code and guess*/
    char* feedback;
    int flag = 0;
    feedback=(char*)malloc(codeLength*sizeof(char));/*Dynamic memory allocation*/
    int i,j,correct=0,misplaced=0,wrong=0;
    fprintf(fileL,"Attempt %d: ",counterAttempt+1);/*Print the attempt number to a file*/
    for(i=0;i<codeLength;i++){
        
        fprintf(fileL,"%d",guessArr[i]);/*Print the guess to a file*/
        if(codeArr[i]==guessArr[i]){/*Correct*/
            feedback[i]='C';
            flag++;
        }
        else{
            int counter=0;

            for(j=0;j<codeLength;j++){
                if(codeArr[i]==guessArr[j]){/*Misplaced*/
                    feedback[i]='M';
                }

                else{
                    counter++;

                    if(counter==codeLength){
                        feedback[i]='W';/*Wrong*/
                    }
                }
            }
        }
    } 
    fprintf(fileL," => Feedback: ");/*Print the feedback to a file*/
    for(i=0;i<codeLength;i++){
        fprintf(fileL,"%c ",feedback[i]);/*Print the guess to a file*/
    }
    
    *score+=displayScore(feedback);
    printf("Your score is: %d\n",*score);/*Display the score*/
    fprintf(fileL,"| Score: %d\n",*score);/*Print the score to a file*/

    return flag == codeLength ? 0: 1;/*Return the feedback*/
}
int displayScore(char *feedback){
    FILE *file = fopen("vault_config.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return -1;
    }

    int codeLength,pointsCorrect, pointsMisplaced, penaltyWrong, trash,i;
    fscanf(file, "CODE_LENGTH=%d\n", &codeLength) ;
    fscanf(file, "DIGIT_MIN=%d\n", &trash) ;
    fscanf(file, "DIGIT_MAX=%d\n", &trash) ;
    fscanf(file, "ALLOW_DUPLICATES=%d\n", &trash); 
    fscanf(file, "MAX_ATTEMPTS=%d\n", &trash) ;
    fscanf(file, "POINTS_CORRECT=%d\n", &pointsCorrect); 
    fscanf(file, "POINTS_MISPLACED=%d\n", &pointsMisplaced); 
    fscanf(file, "PENALTY_WRONG=%d\n", &penaltyWrong) ;
    fclose(file);

    int correct=0,misplaced=0,wrong=0,score=0;
    for (i = 0; i < codeLength; i++)
    {   
        if(feedback[i]=='C'){
            correct++;
        }
        else if(feedback[i]=='M'){
            misplaced++;
        }
        else if(feedback[i]=='W'){
            wrong++;
        }
    }
    score=(correct*pointsCorrect)+(misplaced*pointsMisplaced)-(wrong*penaltyWrong);/*Calculate the score*/
    return score;/*Return the score*/
}
