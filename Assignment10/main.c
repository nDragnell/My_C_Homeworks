#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_EMBEDDING_LENGTH 30000
#define MAX_PAIR_COUNT 300
#define MAX_SENTENCE_LENGTH 300
#define MAX_WORD_LENGTH 30
#define MAX_WORD_COUNT 100
#define MAX_CHARSET_LENGTH 100

char charset[MAX_CHARSET_LENGTH];
int CHARSET_LENGTH = 0;

bool charset_contains(char ch) {
    for (int i = 0; i < CHARSET_LENGTH; i++) {
        if (charset[i] == ch)
            return true;
    }
    return false;
}

void build_charset_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open %s\n", filename);
        exit(1);
    }

    char line[MAX_SENTENCE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        int start = 0;
        if (strncmp(line, "Question:", 9) == 0) {
            start = 9;
        } else if (strncmp(line, "Answer:", 7) == 0) {
            start = 7;
        } else {
            continue; 
        }

        for (int i = start; line[i] != '\0' && line[i] != '\n'; i++) {
            char ch = line[i];
            if (!charset_contains(ch) && CHARSET_LENGTH < MAX_CHARSET_LENGTH) {
                charset[CHARSET_LENGTH++] = ch;
            }
        }
    }
    fclose(file);
    printf("\n");
}

char *embed_sentence(char sentence[][MAX_WORD_LENGTH], int word_count, int max_word_length, int max_words_per_sentence) {
    int embedding_length = max_words_per_sentence * max_word_length * CHARSET_LENGTH;
    if (embedding_length > MAX_EMBEDDING_LENGTH) {
        printf("Embedding length (%d) exceeds MAX_EMBEDDING_LENGTH (%d)\n", embedding_length, MAX_EMBEDDING_LENGTH);
        return NULL;
    }

    char *embedding = (char *)malloc(embedding_length + 1);
    if (!embedding) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    int pos = 0;
    for (int word_index = 0; word_index < max_words_per_sentence; word_index++) {
        if (word_index < word_count) {
            int word_len = strlen(sentence[word_index]);
            for (int char_index = 0; char_index < max_word_length; char_index++) {
                if (char_index < word_len) {
                    char ch = sentence[word_index][char_index];
                    for (int j = 0; j < CHARSET_LENGTH; j++) {
                        embedding[pos++] = (charset[j] == ch) ? '1' : '0';
                    }
                } else {
                    for (int j = 0; j < CHARSET_LENGTH; j++) {
                        embedding[pos++] = '0';
                    }
                }
            }
        } else {
            for (int j = 0; j < max_word_length * CHARSET_LENGTH; j++) {
                embedding[pos++] = '0';
            }
        }
    }

    embedding[pos] = '\0';

    return embedding;
}


double compute_similarity(const char *e1, const char *e2) {
    int len = strlen(e1);
    if (len != strlen(e2)) {
        printf("Embedding length mismatch: %d vs %d\n", len, (int)strlen(e2));
        return 0.0;
    }
    int match = 0;
    for (int i = 0; i < len; i++) {
        if (e1[i] == e2[i]) {
            match++;
        }
    }
    return (double)match / len;
}


void tokenize_sentence(const char *input, char output[][MAX_WORD_LENGTH], int *word_count) {
    char buffer[MAX_SENTENCE_LENGTH];
    strncpy(buffer, input, MAX_SENTENCE_LENGTH - 1);
    buffer[MAX_SENTENCE_LENGTH - 1] = '\0';

    *word_count = 0;
    char *token = strtok(buffer, " \n");
    while (token != NULL && *word_count < MAX_WORD_COUNT) {
        strncpy(output[*word_count], token, MAX_WORD_LENGTH - 1);
        output[*word_count][MAX_WORD_LENGTH - 1] = '\0';
        (*word_count)++;
        token = strtok(NULL, " \n");
    }
}

void binary_to_string(const char *binary, char *output, int output_size) {
    int bit_len = 0;
    int out_pos = 0;
    int total_bits = strlen(binary);

    while (bit_len + CHARSET_LENGTH <= total_bits && out_pos < output_size - 1) {
        int index = -1;
        for (int i = 0; i < CHARSET_LENGTH; i++) {
            if (binary[bit_len + i] == '1') {
                index = i;
                break;
            }
        }
        if (index >= 0 && index < CHARSET_LENGTH) {
            output[out_pos++] = charset[index];
        } 
        else {
            output[out_pos++] = '_';
        }
        bit_len += CHARSET_LENGTH;
    }
    output[out_pos] = '\0';
}

int main() {
   
    build_charset_from_file("embeddings.txt");
    if (CHARSET_LENGTH == 0) {
        printf("Error: Charset is empty\n");
        return 1;
    }

    FILE *file = fopen("embeddings.txt", "r");
    if (!file) {
        printf("Cannot open embeddings.txt\n");
        return 1;
    }

    char *questions[MAX_PAIR_COUNT];     
    char *question_embeddings[MAX_PAIR_COUNT]; 
    char *answers[MAX_PAIR_COUNT];       
    int pair_count = 0;

    char line[MAX_SENTENCE_LENGTH];
    char *current_question = NULL;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; 
        if (strncmp(line, "Question:", 9) == 0) {
            if (current_question) {
                free(current_question); 
            }
            current_question = strdup(line + 9);
            if (!current_question) {
                printf("Memory allocation failed for question\n");
                fclose(file);
                return 1;
            }

        } else if (strncmp(line, "Answer:", 7) == 0 && current_question) {
            if (pair_count >= MAX_PAIR_COUNT) {
                printf("Maximum pair count reached\n");
                free(current_question);
                break;
            }
            questions[pair_count] = current_question;
            answers[pair_count] = strdup(line + 7);
            if (!answers[pair_count]) {
                printf("Memory allocation failed for answer\n");
                free(current_question);
                fclose(file);
                return 1;
            }
        
            char tokenized[MAX_WORD_COUNT][MAX_WORD_LENGTH] = {{0}};
            int word_count = 0;
            tokenize_sentence(questions[pair_count], tokenized, &word_count);
            question_embeddings[pair_count] = embed_sentence(tokenized, word_count, MAX_WORD_LENGTH, MAX_WORD_COUNT);
            if (!question_embeddings[pair_count]) {
                printf("Failed to embed question %d\n", pair_count);
                free(current_question);
                free(answers[pair_count]);
                fclose(file);
                return 1;
            }

            pair_count++;
            current_question = NULL;
        }
    }
    if (current_question) {
        free(current_question); 
    }
    fclose(file);

    if (pair_count == 0) {
        printf("No valid question-answer pairs found in embeddings.txt\n");
        return 1;
    }

    printf("Enter your question:\n");
    char user_input[MAX_SENTENCE_LENGTH];
    if (!fgets(user_input, sizeof(user_input), stdin)) {
        printf("Error reading input.\n");
        for (int i = 0; i < pair_count; i++) {
            free(questions[i]);
            free(question_embeddings[i]);
            free(answers[i]);
        }
        return 1;
    }
    user_input[strcspn(user_input, "\n")] = '\0';
    
    printf("User input: %s\n", user_input);
    if (strlen(user_input) == 0) {
        printf("Error: Empty input\n");
        for (int i = 0; i < pair_count; i++) {
            free(questions[i]);
            free(question_embeddings[i]);
            free(answers[i]);
        }
        return 1;
    }

    char tokenized[MAX_WORD_COUNT][MAX_WORD_LENGTH] = {{0}};
    int word_count = 0;
    tokenize_sentence(user_input, tokenized, &word_count);
    char *user_embedding = embed_sentence(tokenized, word_count, MAX_WORD_LENGTH, MAX_WORD_COUNT);
    if (!user_embedding) {
        for (int i = 0; i < pair_count; i++) {
            free(questions[i]);
            free(question_embeddings[i]);
            free(answers[i]);
        }
        return 1;
    }

    int best_index = -1;
    double best_score = -1.0;
    for (int i = 0; i < pair_count; i++) {
        double score = compute_similarity(user_embedding, question_embeddings[i]);
        if (score > best_score) {
            best_score = score;
            best_index = i;
        }
    }

    free(user_embedding);

    if (best_index == -1) {
        printf("No matching question found.\n");
    } else {
        printf(">> Best match found! Score: %.4f\n", best_score);
        printf("Matched question: %s\n", questions[best_index]);
        printf("RAW binary answer:\n%s\n", answers[best_index]);
        char decoded_answer[MAX_SENTENCE_LENGTH * 4] = {0};
        binary_to_string(answers[best_index], decoded_answer, sizeof(decoded_answer));
        printf("Answer: %s\n", decoded_answer);
    }

    for (int i = 0; i < pair_count; i++) {
        free(questions[i]);
        free(question_embeddings[i]);
        free(answers[i]);
    }

    return 0;
}