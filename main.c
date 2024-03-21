#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Add a word to an array
void addElement(int *wordCount, char ***words, char *word, int *wordsMaxSize);

struct Person {
    char *name;
    char *location;
    char *itemNames[2];
    int *itemNumbers;
};

int main() {

    char line[1025];
    struct Person *people[2];

    while (1) {
        printf(">> ");
        fflush(stdout);

        if (fgets(line, 1025, stdin) == NULL) {
            break;
        }

        char *t;
        int wordCount = 0;
        int wordsMaxSize = 4;  // Initial max size of the words array where words of the current line are kept
        char **words;  // Array of char pointers to keep words of a line
        words = (char **)malloc(wordsMaxSize * sizeof(char *));
        for(t = strtok(line, " \n"); t != NULL; t = strtok(NULL, " \n")) {
            addElement(&wordCount, &words, t, &wordsMaxSize);
        }

        if (strcmp(line, "exit\n") == 0) {
            break;
        }

        for(int i = 0; i < wordsMaxSize; i++) {
            printf("%s\n", words[i]);
        }
        free(words);
    }

    return 0;
}

void addElement(int *wordCount, char ***words, char *word, int *wordsMaxSize) {
    (*words)[*wordCount] = word;
    (*wordCount)++;

    printf("wordCount: %d, wordMaxSize: %d\n", *wordCount, *wordsMaxSize);
    if(*wordCount == *wordsMaxSize) {  // Array is full
        printf("HEYO\n");
        *wordsMaxSize *= 2;
        *words = (char **)realloc(*words, (*wordsMaxSize) * sizeof(char *));
    }
}
