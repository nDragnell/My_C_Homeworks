#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

enum Color {
    RGB,
    BGR,
    GRAY
};

int randomLine1(int values[][80], FILE *fp);
int randomLine2_3_4(int values[][80], FILE *fp);
void push(char *stack, char ch);
void pop();
void max_operation(int values[][80], int chunkSize, int lineSize, int chunkLine, int result[80]);
void avg_operation(int values[][80], int chunkSize, int lineSize, int chunkLine, int result[80]);
void print_output_file(FILE *fp, int values[][80], char stack[], int chunkSize, int lineSize, 
    void (*op1)(int[][80], int, int, int ,int[]), void (*op2)(int[][80], int, int, int ,int[]), enum Color color);

#define stackSize 4
int topStack = 0;

int main() {
    int values[4][80] = {0};
    char stack[stackSize] = {0};
    char ch;
    char strInp[10];
    int chunkSize,lineSize,i;
    int lineOut[3][80] = {0};

    srand(time(NULL));

    FILE *inpf;
    inpf = fopen("input.txt", "w");
    chunkSize = randomLine1(values, inpf);
    lineSize = randomLine2_3_4(values, inpf);
    fclose(inpf);

    printf("Please enter your operation (For average 'a' or for maximum 'm'): \n");
    for (i = 0; i < 3; ) {
        printf("Operation %d (a/m, undo: x): ", i+1);
        scanf(" %c", &ch);
        if (ch == 'a' || ch == 'A' || ch == 'm' || ch == 'M') {
            push(stack, ch);
            topStack++;
            i++;
        } else if ((ch == 'x' || ch == 'X') && topStack > 0) {
            pop();
            i--;
            printf("Last operation removed.\n");
        } else {
            printf("Invalid input.\n");
        }
    }
    

    enum Color color;
    FILE *outf;
    outf = fopen("output.txt", "w");
    while (1) {
        printf("Please enter one of the following options: 'rgb', 'bgr' or 'gray': ");
        scanf("%s", strInp);
        if (strcmp(strInp, "rgb") == 0) {
            color = RGB;
            break;
        } else if (strcmp(strInp, "bgr") == 0) {
            color = BGR;
            break;
        } else if (strcmp(strInp, "gray") == 0) {
            color = GRAY;
            break;
        } else {
            printf("Invalid input.\n");
        }
    }
    print_output_file(outf, values, stack, chunkSize, lineSize, avg_operation, max_operation, color);
    fclose(outf);

    return 0;
}

int randomLine1(int values[][80], FILE *fp) {
    int times = ((rand() % 3) + 1) * 3;
    for (int i = 0; i < times; i++) {
        values[0][i] = rand() % 100 + 1;
        fprintf(fp, "%d ", values[0][i]);
    }
    fprintf(fp, "\n");
    return times;
}

int randomLine2_3_4(int values[][80], FILE *fp) {
    int times = (((rand() % 3) + 1) * 30) - 10;
    for (int j = 1; j < 4; j++) {
        for (int i = 0; i < times; i++) {
            values[j][i] = rand() % 100 + 1;
            fprintf(fp, "%d ", values[j][i]);
        }
        fprintf(fp, "\n");
    }
    return times;
}

void push(char *stack, char ch) {
    stack[topStack] = ch;
}

void pop() {
    if (topStack > 0)
        topStack--;
}

void max_operation(int values[][80], int chunkSize, int lineSize, int chunkLine, int result[80]) {
    int max, temp;
    for (int i = 0; i < lineSize - chunkSize + 1; i++) {
        max = 0;
        for (int j = 0; j < chunkSize; j++) {
            temp = values[0][j] * values[chunkLine][i + j];
            if (temp > max)
                max = temp;
        }
        result[i] = max;
    }
}

void avg_operation(int values[][80], int chunkSize, int lineSize, int chunkLine, int result[80]) {
    int sum;
    for (int i = 0; i < lineSize - chunkSize + 1; i++) {
        sum = 0;
        for (int j = 0; j < chunkSize; j++) {
            sum += values[0][j] * values[chunkLine][i + j];
        }
        result[i] = sum / chunkSize;
    }
}

void print_output_file(FILE *fp, int values[][80], char stack[], int chunkSize, int lineSize, 
    void (*op1)(int[][80], int, int, int ,int[]), void (*op2)(int[][80], int, int, int ,int[]), 
    enum Color color) {
    int lineOut[3][80];
    for (int i = 0; i < 3; i++) {
        if (stack[i] == 'A' || stack[i] == 'a')
            op1(values, chunkSize, lineSize, i + 1, lineOut[i]); 
        else if (stack[i] == 'M' || stack[i] == 'm')
            op2(values, chunkSize, lineSize, i + 1, lineOut[i]);
    }
    
    if (color == RGB) {
        fprintf(fp, "RGB:-> ");
        for (int i = 0; i < lineSize - chunkSize + 1; i++) {
            if (i > 0) 
                fprintf(fp, ",");
            fprintf(fp, "[%d,%d,%d]", lineOut[0][i], lineOut[1][i], lineOut[2][i]);
        }
        fprintf(fp, "\n");
    } else if (color == BGR) {
        fprintf(fp, "BGR:-> ");
        for (int i = 0; i < lineSize - chunkSize + 1; i++) {
            if (i > 0) 
                fprintf(fp, ",");
            fprintf(fp, "[%d,%d,%d]", lineOut[2][i], lineOut[1][i], lineOut[0][i]);
        }
        fprintf(fp, "\n");
    } else if (color == GRAY) {
        fprintf(fp, "GRAY:->\n");
        for (int j = 0; j < 3; j++) {
            fprintf(fp,"[");
            for (int i = 0; i < lineSize - chunkSize + 1; i++) {
                if (i > 0) 
                    fprintf(fp, ",");
                fprintf(fp, "%d", lineOut[j][i]);
            }
            fprintf(fp, "]\n");
        }
    }
}
