#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define true 1
#define false 0

struct Person {
    char *name;
    char *location;
//    char **itemNames;  // 2D Array to keep item names
//    int *itemNumbers;
};

char forbiddenWords[3][8] = { "NOBODY", "NOTHING", "NOWHERE"};  // These words cannot be used inside a sentence
char keywords[16][6] = {"sell", "buy", "go", "to", "from", "and", "at", "has", "if",
                        "less", "more", "than", "exit", "where", "total", "who"};  // Used for parsing

// Add a word to an array
void addElement(int *wordCount, char ***words, char *word, int *wordsMaxSize);

// Add a new person to people array
void addPerson(int *peopleCount, struct Person ***people, struct Person *newPerson, int *peopleMaxCount);

// Check if a word is a keyword
int isKeyword(char *word);

// Check if a word is forbidden
int isForbiddenWord(char *word);

int main() {

    char line[1025];

    int peopleCount = 0;
    int peopleMaxCount = 1;
    struct Person **people;
    people = (struct Person **)malloc(peopleMaxCount * sizeof(struct Person *));  // At the beginning, create an array of person structs with only 1 person struct

    // Add a person for test purposes
    struct Person person;
    person.name = "Mert";
    person.location = "boun";
    struct Person *tmp = &person;
    addPerson(&peopleCount, &people, tmp, &peopleMaxCount);

    while (1) {
        printf(">> ");
        fflush(stdout);

        if (fgets(line, 1025, stdin) == NULL) {
            break;
        }

        char *t;
        int wordCount = 0;
        int wordsMaxSize = 1;  // Initial max size of the words array where words of the current line are kept
        char **words;  // Array of char pointers to keep words of a line
        words = (char **)malloc(wordsMaxSize * sizeof(char *));
        for(t = strtok(line, " \n"); t != NULL; t = strtok(NULL, " \n")) {
            addElement(&wordCount, &words, t, &wordsMaxSize);
        }

        if (strcmp(line, "exit\n") == 0) {
            break;
        }

        // Check whether first word is "who"
        if(strcmp(words[0], "who") == 0) {
            if(strcmp(words[1], "at") != 0) {
                printf("INVALID\n");
                continue;
            }

            char *locationName = words[2];
            if(isKeyword(locationName) == true || isForbiddenWord(locationName) == true) {
                printf("INVALID\n");
                free(words);
                continue;
            }

            // Location is valid
            int locationFound = false;

            // Check if anyone is at this location
            for(int i = 0; i < peopleCount; i++) {
                if(strcmp(((*people)[i]).location, locationName) == 0) {  // If someone is at this location, print the name of the person
                    locationFound = true;
                    printf("%s\n", ((*people)[i]).name);
                    break;
                }
            }

            if(locationFound == false) {
                printf("NOBODY\n");
            }

            free(words);
            continue;
        }

        free(words);
    }

    return 0;
}

void addElement(int *wordCount, char ***words, char *word, int *wordsMaxSize) {
    (*words)[*wordCount] = word;
    (*wordCount)++;

    if(*wordCount == *wordsMaxSize) {  // Array is full, double its size
        *wordsMaxSize *= 2;
        *words = (char **)realloc(*words, (*wordsMaxSize) * sizeof(char *));
    }
}

void addPerson(int *peopleCount, struct Person ***people, struct Person *newPerson, int *peopleMaxCount) {
    (*people)[*peopleCount] = newPerson;
    (*peopleCount)++;

    if(*peopleCount == *peopleMaxCount) {
        *peopleMaxCount *= 2;
        *people = (struct Person **)realloc(*people, (*peopleMaxCount) * sizeof(struct Person *));
    }
}

int isKeyword(char *word) {
    for(int i = 0; i < sizeof(keywords) / sizeof(char *); i++) {
        if(strcmp(word, keywords[i]) == 0) {
            return true;
        }
    }

    return false;
}

int isForbiddenWord(char *word) {
    for(int i = 0; i < 3; i++) {
        if(strcmp(word, forbiddenWords[i]) == 0) {
            return true;
        }
    }

    return false;
}
