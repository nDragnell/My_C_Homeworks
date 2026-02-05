#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxCharCountInTheText 1000000
#define maxWordsCount 20000
#define maxWordLength 20
#define maxUniqueCharCount 128
#define vectorLen 10000

void Parselization(FILE *fp, int *embeddingLen, int *pairs, 
    char queWords[][maxWordLength], char ansWords[][maxWordLength], 
    char *uniqueChars, int *indexUnit, int *qIndex, int *aIndex);
int isUnique(char *str,char *uniqueChars, int index);
void queWordsParser(char *token, char queWords[][maxWordLength], int *index, int *embeddingLen);
void ansWordsParser(char *token, char ansWords[][maxWordLength], int *index, int *embeddingLen);
char *wordToValue(char *inp, char *out, char *uniqueChars);
char *mystrtok(char *inp, char bracket, char *out);

int main()
{
    char uniqueChars[maxUniqueCharCount];
    char queWords[maxWordsCount][maxWordLength];
    char ansWords[maxWordsCount][maxWordLength];
    int embeddingLen;
    int pairs;
    int indexUnit;
    int qIndex;
    int aIndex;
    int i;
    indexUnit=0;
    qIndex=0;
    aIndex=0;

    FILE *readf;
    FILE *savef;

    readf=fopen("database.txt","r");
    if(readf==NULL){
        printf("There is no file.\n");
        return 1;
    }
    
    savef=fopen("embeddings.txt","w");
    if(savef==NULL){
        printf("There is no file.\n");
        return 1;
    }

    Parselization(readf, &embeddingLen, &pairs, queWords, ansWords, 
        uniqueChars, &indexUnit, &qIndex, &aIndex);

    fclose(readf);
    
    fprintf(savef,"Metadata: \n");/*Printing process*/
    fprintf(savef,"Maximum embedding length: %d \n",embeddingLen);
    fprintf(savef,"Embedding dimension: %d",indexUnit);
    fprintf(savef,"\nNumber of QA pairs: %d\n",pairs/2);
    
    qIndex=0;
    aIndex=0;
    
    char word[vectorLen];
    
    while(qIndex < vectorLen && aIndex < vectorLen && 
        strcmp(queWords[qIndex], "") != 0 
        && 
        strcmp(ansWords[aIndex], "") != 0) {

        fprintf(savef,"\nQuestion:");
        while(qIndex < vectorLen && strcmp(queWords[qIndex], "\n") != 0) {

            if(strcmp(queWords[qIndex],"00000")==0){
                for(i=0;i<indexUnit;i++){
                    fprintf(savef,"0");
                }
                qIndex++;
            }

            else{
                wordToValue(queWords[qIndex], word, uniqueChars);
                fprintf(savef,"%s", word);
                qIndex++;
            }
        }
        qIndex++; 
    
        fprintf(savef,"\nAnswer:");
        while(aIndex < vectorLen && strcmp(ansWords[aIndex], "\n") != 0) {

            if(strcmp(ansWords[aIndex],"00000")==0){
                for(i=0;i<indexUnit;i++){
                    fprintf(savef,"0");
                }
                aIndex++;
            }
            else{
                wordToValue(ansWords[aIndex], word, uniqueChars);
                fprintf(savef,"%s", word);
                aIndex++;
            }
        }
        aIndex++; 
        
        fprintf(savef,"\n---");
    }

    fclose(savef);

    return 0;
}

void Parselization(FILE *fp, int *embeddingLen, int *pairs, 
    char queWords[][maxWordLength], char ansWords[][maxWordLength], 
    char *uniqueChars,int *indexUnit, int *qIndex, int *aIndex){
    char str[maxCharCountInTheText];
    char *tempstr;
    char ch;
    int i;
    i=0;
    int current_line_len;
    current_line_len = 0;
    int max_line_len;
    max_line_len = 0;  

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            str[i++] = ch;
            if (current_line_len > max_line_len) {
                max_line_len = current_line_len;
            }
            current_line_len = 0;  
        } 
        else {  
            str[i++] = ch;
            current_line_len++;  
        }
    }
    if (current_line_len > max_line_len) {
        max_line_len = current_line_len;
    }
    str[i] = '\0'; 

    *embeddingLen = max_line_len;
    
    *indexUnit=isUnique(str,uniqueChars,0); /*The function that's recursive */
                    /*Current index is going to use to find embedding value*/

    *pairs=1;                         /*To calculate that how many pair has*/

    char token[maxWordsCount];
    tempstr=mystrtok(str, '\n',token);
    queWordsParser(token+3, queWords, qIndex, embeddingLen);
    while (tempstr != NULL) {
        tempstr=mystrtok(tempstr, '\n',token);
        if (strncmp(token, "Q: ", 3) == 0) {
            (*pairs)+=1;
            if(strlen(token) > *embeddingLen) {
                *embeddingLen = strlen(token);
            }
            queWordsParser(token+3, queWords, qIndex, embeddingLen);
        }

        else if (strncmp(token, "A: ", 3) == 0) {
            (*pairs)+=1;
            if(strlen(token) > *embeddingLen) {
                *embeddingLen = strlen(token);
            }
            ansWordsParser(token+3, ansWords, aIndex, embeddingLen);
        }
        else if (token[0] == '\0') {  
            tempstr = mystrtok(NULL, '\n', token);
            continue;
        }
                
    }
    
    if(*qIndex<maxWordsCount&&*aIndex<maxWordsCount){
        strcpy(queWords[*qIndex],"");
        strcpy(ansWords[*aIndex],"");
    }
}

void queWordsParser(char *token, char queWords[][maxWordLength], int *index, int *embeddingLen){
    int leftVectorCount;
    int i;
    leftVectorCount=(*embeddingLen)-strlen(token)-3;

    for(i=0;i<leftVectorCount;i++){
        strcpy(queWords[*index], "00000");
        (*index)+=1;    
    }

    char* word;
    word=strtok(token, " ");
    strcpy(queWords[*index], word);    
    (*index)+=1;

    while(word!=NULL && *index < maxWordsCount){
        word=strtok(NULL, " ");
        if(word != NULL) {
            strcpy(queWords[*index], word);
            (*index)+=1;
        }
    }
    
    if (*index < maxWordsCount) {
        strcpy(queWords[*index], "\n");
        (*index)+=1;
    }
}

void ansWordsParser(char *token, char ansWords[][maxWordLength], int *index, int *embeddingLen){
    int leftVectorCount;
    int i;
    leftVectorCount=(*embeddingLen)-strlen(token)-3;

    for(i=0;i<leftVectorCount;i++){
        strcpy(ansWords[*index], "00000");
        (*index)+=1;    
    }

    char* word;
    word=strtok(token, " ");
    strcpy(ansWords[*index], word);    
    (*index)+=1;

    while(word!=NULL && *index < maxWordsCount){
        word=strtok(NULL, " ");
        if(word != NULL) {
            strcpy(ansWords[*index], word);
            (*index)+=1;
        }
    }
    
    if (*index < maxWordsCount) {
        strcpy(ansWords[*index], "\n");
        (*index)+=1;
    }
}

char *wordToValue(char *inp, char *out, char *uniqueChars){
    int i,j,uniqueLen;
    uniqueLen=strlen(uniqueChars);

    if(inp==NULL||out==NULL){
        printf("There is no text.\n");
        return NULL;
    }

    for(i=0;/*inp[i]!='\0'&&*/i<maxWordLength;i++){

        for(j=0;j<uniqueLen;j++){
            if(inp[i]==uniqueChars[j]){
                out[j+(uniqueLen*i)]='1';
            }

            else{
                out[j+(uniqueLen*i)]='0';
            }
        }
    }

    out[uniqueLen*i]='\0';
    return out;
}

int isUnique(char *str, char *uniqueChars, int index) {
    for (; *str != '\0' && index < maxUniqueCharCount - 1; str++) {
        int found = 0;

        for (int j = 0; j < index; j++) {
            if (uniqueChars[j] == *str) {
                found = 1;
                break;
            }
        }

        if (!found) {
            uniqueChars[index++] = *str;
        }
    }

    uniqueChars[index] = '\0';
    return index;
}

char* mystrtok(char *inp, char bracket, char *out) {
    int i;
    char ch;

    if (inp == NULL||*inp == '\0') 
        return NULL;


    for (i = 0; inp[i] != '\0'; i++) {
        if (inp[i] == bracket) {
            out[i] = '\0';          
            return inp + i + 1;       
        } else {
            out[i] = inp[i];
        }
    }
    out[i] = '\0';                   
    return NULL;                      
}