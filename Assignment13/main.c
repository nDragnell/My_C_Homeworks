#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef enum JobType{
    BACKEND_DEVELOPER,
    FRONTEND_DEVELOPER,
    FULLSTACK_DEVELOPER,
    MOBILE_DEVELOPER,
    EMBEDDED_SOFTWARE_ENGINEER,
    GAME_DEVELOPER,
    DEVOPS_ENGINEER,
    TEST_ENGINEER,
}JobType;

typedef struct{
    int day;
    int month;
    int year;
}Date;

typedef struct Person{
    int id;
    char name[20];
    char mail[30];
    JobType jobT;
    bool email_verified;
    Date date;
}Person;

void getInput(FILE *fp);

int main(){
    FILE *fp;

    fp=fopen("input.txt","r");
    getInput(fp);

    return 0;
}

void getInput(FILE *fp){
    char *temp;
    while(temp!=EOF){
        
            temp=(char*)malloc(100);
        
            fgets(temp,100,fp);

            char *token;
            
    }

}