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

    // Add dummies for test purposes
    struct Person *person1 = (struct Person *)malloc(sizeof(struct Person));
    person1->name = "Mert";
    person1->location = "boun";
    addPerson(&peopleCount, &people, person1, &peopleMaxCount);

    struct Person *person2 = (struct Person *)malloc(sizeof(struct Person));
    person2->name = "Arda";
    person2->location = "boun";
    addPerson(&peopleCount, &people, person2, &peopleMaxCount);

    struct Person *person3 = (struct Person *)malloc(sizeof(struct Person));
    person3->name = "Umut";
    person3->location = "boun";
    addPerson(&peopleCount, &people, person3, &peopleMaxCount);

    struct Person *person4 = (struct Person *)malloc(sizeof(struct Person));
    person4->name = "Ahmet";
    person4->location = "boun";
    addPerson(&peopleCount, &people, person4, &peopleMaxCount);

    struct Person *person5 = (struct Person *)malloc(sizeof(struct Person));
    person5->name = "Tolga";
    person5->location = "boun";
    addPerson(&peopleCount, &people, person5, &peopleMaxCount);

    while (1) {
        printf(">> ");
        fflush(stdout);

        if (fgets(line, 1025, stdin) == NULL) {
            break;
        }
        if (strcmp(line, "exit\n") == 0) {
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

        // Check whether first word is "who"
        if(strcmp(words[0], "who") == 0) {
            // Check if the question format is correct
            if (wordCount != 4){
                printf("INVALID\n");
                continue;
            }

            if(strcmp(words[1], "at") != 0) {
                printf("INVALID\n");
                continue;
            }

            if(strcmp(words[3], "?") != 0) {
                printf("INVALID\n");
                continue;
            }

            // Check if location name is valid
            char *locationName = words[2];
            if(isKeyword(locationName) == true || isForbiddenWord(locationName) == true) {
                printf("INVALID\n");
                free(words);
                continue;
            }

            int locationFound = false;

            // Keep track of the people at this location
            int atLocationCount = 0;
            int atLocationMaxSize = 1;
            char **atLocation = (char **)malloc(atLocationMaxSize * sizeof(char *));

            // Check if anyone is at this location
            for(int i = 0; i < peopleCount; i++) {
                if(strcmp(people[i]->location, locationName) == 0) {  // If someone is at this location, store them
                    locationFound = true;
                    addElement(&atLocationCount,&atLocation,people[i]->name,&atLocationMaxSize);
                }
            }

            if(locationFound == false) {
                printf("NOBODY\n");
                free(atLocation);
                free(words);
                continue;
            }

            // Print people at this location
            for (int i = 0; i < atLocationCount - 1; i++) {
                printf("%s and ", atLocation[i]);
            }
            printf("%s\n", atLocation[atLocationCount - 1]);


            free(atLocation);
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
