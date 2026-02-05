#include <stdio.h>
#include <math.h>
double quadraticValue(int t);
double hmax();
double tflight();
int intervalYCalc();
void simulate();
void datapoint(int t,int row);
void simulatesave();
void datapointsave(int t, int row,FILE* floader);

int main(){
    double initial,gravity,height;
    while(1){
        printf("\nWelcome to the Rocket Launch System\n---------------------------------\n1. Enter launch parameters\n2. Simulate rocket trajectory\n3. Save trajectory data\n4. Exit.\nChoice: ");
        int choice;
        scanf("%d",&choice);/*Menu user interface*/
        switch (choice)/*menu control condition*/
        {
        case 1:/*The case provides to get input from user and records them to file and variables*/
            FILE *fsave;
            fsave=fopen("rocket_data.txt","w");

            printf("Enter initial velocity (m/s): ");  
            scanf("%lf",&initial);
            fprintf(fsave,"%.4f ",initial);

            printf("Enter gravity (m/s², default 9.8): ");
            scanf("%lf",&gravity);
            if (gravity<0)/*Gravity control*/
            {
                printf("Gravity cannot be smaller than zero.");
                return -1;
            }
            fprintf(fsave,"%.4f ",gravity);

            printf("Enter launch height (m): ");
            scanf("%lf",&height);
            fprintf(fsave,"%.4f ",height);
            fprintf(fsave,"\nEquation: h(t) = -%.3f * t^2 + %.3f * t + %.3f\n",(gravity/2),initial,height);
            printf("Rocket parameters saved to rocket_data.txt!");
            fclose(fsave);
            break;
        case 2 :/*At the case, the datas seems to user and the simulate function draws the graph*/
            printf("Reading rocket parameters from file...\n");
            printf("Equation: h(t) = -%.3f * t^2 + %.3f * t + %.3f\n",(gravity/2),initial,height); 
            simulate();
            break;
        case 3 :/*In the case, max altitude and total flight is calculated by functions ,than the graph saves to file */
            printf("Maximum altitude: %.2f meters\n",hmax());
            printf("Total flight duration: %.2f seconds\n",tflight());
            printf("Graph saved to trajectory.txt!\n");
            simulatesave();
            break;
        case 4 :/*The case closes the program*/
            printf("The program is closing...");
            return -1;
            break;
        default:/*If the user enter an unvalid value, the system wants again an input*/
            printf("Please enter a valid value to use system.\n");
        }
    }
    return 0;
}
double quadraticValue(int t){/*the function calculates to y value that is in the graph by using variables */
    double res;
    double initial,gravity,height;
    FILE *floader;
    floader=fopen("rocket_data.txt","r");
    fscanf(floader,"%lf %lf %lf ",&initial,&gravity,&height);
    fclose(floader);
    res=(-gravity*t*t/2)+(initial*t)+height;
    return res;
}
double hmax(){/*Max altitude calculates in this function*/
    double initial,gravity,height;
    FILE *floader;
    floader=fopen("rocket_data.txt","r");
    fscanf(floader,"%lf %lf %lf ",&initial,&gravity,&height);
    fclose(floader);
    double res;
    res=(((initial*initial)/(2*gravity))+height);
    return res;
}
double tflight(){/*Total flight calculates in this function*/
    double initial,gravity,height;
    FILE *floader;
    floader=fopen("rocket_data.txt","r");
    fscanf(floader,"%lf %lf %lf ",&initial,&gravity,&height);
    double delta=((initial*initial)+(2*gravity*height));
    if(delta<0){/*To do not get error,when using sqrt function*/
        printf("Delta cannot be smaller than zero. Delta:%.2lf\n",delta);
        return -1;
    }
    double res;
    res=((initial+sqrt(delta))/gravity);
    return res;
}
int intervalYCalc(){/*The function provides to determine intervals that is 
    at the between y axis values because if the value is very big, the program could give error*/
    int maxY=100;
    while(maxY<hmax())
        maxY=maxY+100;
    int intervalY;
    intervalY=maxY/10;
    return intervalY;
}
void simulate(){/*The functions draws the graph with the help of datapoint function*/
    printf("Simulating trajectory...\n");
    int yaxis=intervalYCalc();
    int t;
    for(int i=10;i>=0;i--){
        int row=yaxis*i;
        if(yaxis*i>=100) printf("%d |",row);
        else if(row==0)  printf("%d   |",row);
        else printf("%d  |",row);
        for(t=0;t<tflight();t++){
            datapoint(t,row);
        }
        if(i!=0)printf("\n");
        printf("\n");
    }
    printf("    |---");
    int xaxis=2;
    int xmax=(tflight()/2);
    for (int j = xmax;j>0;j--){
        printf("|---");
    }
    printf("|\n ");
    for (int j = 0;j<=xmax+1;j++){
        if(xaxis*j>10) printf("  %d",xaxis*j);
        else printf("   %d",xaxis*j);
    }
}
void datapoint(int t, int row) {/*The function is placing values to the graph with the helps of quadraticValue and intervalYClac functions*/
    int rounded;
    rounded = ((int)(quadraticValue(t) / intervalYCalc()) * intervalYCalc());
    if (row == rounded) {
        int control = 0,times=0;
        for(int j=t;j>0;j--){
            if(((int)(quadraticValue(t) / intervalYCalc()) * intervalYCalc())==((int)(quadraticValue(j) / intervalYCalc()) * intervalYCalc())){
                control++;
                if(control==2)
                    times=j;
            }
        }
        if(control<=1) {
            for(int i=0;i<t;i++){
                printf("  ");
            }
        }
        else {
            for(int z=0;z<(t-times);z++){
                printf("  ");
            }
        }
        printf("#");
    }
}
void simulatesave(){/*The function is same with the simulate function but this function draws to file*/
    FILE* floader;
    floader=fopen("trajectory.txt","w");
    if (floader == NULL) {
        printf("Error: The trajectory.txt file could not finded .\n");
        return;
    }
    int yaxis=intervalYCalc();
    int t;
    for(int i=10;i>=0;i--){
        int row=yaxis*i;
        if(yaxis*i>=100) fprintf(floader,"%d |",row);
        else if(row==0) fprintf(floader,"%d   |",row);
        else fprintf(floader,"%d  |",row);
        for(t=0;t<tflight();t++){
            datapointsave(t,row,floader);
        }
        if(i!=0)fprintf(floader,"\n");
        fprintf(floader,"\n");
    }
    fprintf(floader,"    |---");
    int xaxis=2;
    int xmax=(tflight()/2);
    for (int j = xmax;j>0;j--){
        fprintf(floader,"|---");
    }
    fprintf(floader,"|\n ");
    for (int j = 0;j<=xmax+1;j++){
        if(xaxis*j>10) fprintf(floader,"  %d",xaxis*j);
        else fprintf(floader,"   %d",xaxis*j);
    }
    fclose(floader);
}
void datapointsave(int t, int row,FILE* floader){/*The function is same with the datapoint function but this function draws to file*/
    int rounded;
    rounded = ((int)(quadraticValue(t) / intervalYCalc()) * intervalYCalc());
    if (row == rounded) {
        int control = 0,times=0;
        for(int j=t;j>=0;j--){
            if(((int)(quadraticValue(t) / intervalYCalc()) * intervalYCalc())==((int)(quadraticValue(j) / intervalYCalc()) * intervalYCalc())){
                control++;
                if(control==2){
                    times=j;
                }
            }
        }
        if(control<=1) {
            for(int i=0;i<t;i++){
                fprintf(floader,"  ");
            }
        }
        else {
            for(int z=0;z<(t-times);z++){
                fprintf(floader,"  ");
            }
        }
        fprintf(floader,"#");
    }
}