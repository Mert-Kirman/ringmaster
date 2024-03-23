#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define true 1
#define false 0

// These words cannot be used inside a sentence
char forbiddenWords[3][8] = { "NOBODY", "NOTHING", "NOWHERE"};
// Used for parsing
char keywords[16][6] = {"sell", "buy", "go", "to", "from", "and", "at", "has", "if",
                        "less", "more", "than", "exit", "where", "total", "who"};

struct Person {
    char *name;
    char *location;
    char **itemNames;  // 2D Array to keep item names
    int *itemNumbers;  // 1D Array to keep item counts
    int itemCount;
    int itemMaxCount;
};

// Add a word to an array
void addElement(int *wordCount, char ***words, char *word, int *wordsMaxSize);

// Add a new person to people array
void addPerson(int *peopleCount, struct Person ***people, struct Person *newPerson, int *peopleMaxCount);

// Add a new or existing item to a person's item list
void addItem(int *itemCount, char ***itemNames, char *newItem, int * itemMaxCount, int **itemNumbers);

// Check if a word is a keyword
int isKeyword(char *word);

// Check if a word is forbidden
int isForbiddenWord(char *word);

int main() {

    char line[1025];

    // Create an array of person structs with max capacity of 1
    int peopleCount = 0;
    int peopleMaxCount = 1;
    struct Person **people;
    people = (struct Person **)malloc(peopleMaxCount * sizeof(struct Person *));

    /* Add dummies for test purposes */
    // Create a person instance
    struct Person *person1 = (struct Person *)malloc(sizeof(struct Person));
    // Default values for every person instance
    person1->itemCount = 0;
    person1->itemMaxCount = 1;
    person1->itemNames = (char **)malloc((person1->itemMaxCount) * sizeof(char *));
    person1->itemNumbers = (int *)malloc((person1->itemMaxCount) * sizeof(int));
    // Custom values
    person1->name = "Mert";
    person1->location = "boun";
    // Add person instance to the people array
    addPerson(&peopleCount, &people, person1, &peopleMaxCount);

    printf("%s number is: %d\n", (person1->itemNames)[0], (person1->itemNumbers)[0]);

    // Add a dummy item for person1
    char *newItem = "elma";
    addItem(&(person1->itemCount), &(person1->itemNames), newItem, &(person1->itemMaxCount), &(person1->itemNumbers));

    printf("%s number is: %d\n", (person1->itemNames)[0], (person1->itemNumbers)[0]);

    // Add the same dummy item for person1 again
    newItem = "elma";
    addItem(&(person1->itemCount), &(person1->itemNames), newItem, &(person1->itemMaxCount), &(person1->itemNumbers));

    printf("%s number is: %d\n", (person1->itemNames)[0], (person1->itemNumbers)[0]);

    // Add a different dummy item for person1
    newItem = "armut";
    addItem(&(person1->itemCount), &(person1->itemNames), newItem, &(person1->itemMaxCount), &(person1->itemNumbers));

    printf("%s number is: %d\n", (person1->itemNames)[0], (person1->itemNumbers)[0]);
    printf("%s number is: %d\n", (person1->itemNames)[1], (person1->itemNumbers)[1]);

    printf("\nMOVING ON TO SECOND DUMMY PERSON\n\n");

    // Create a second person instance
    struct Person *person2 = (struct Person *)malloc(sizeof(struct Person));
    // Default values for every person instance
    person2->itemCount = 0;
    person2->itemMaxCount = 1;
    person2->itemNames = (char **)malloc((person2->itemMaxCount) * sizeof(char *));
    person2->itemNumbers = (int *)malloc((person2->itemMaxCount) * sizeof(int));
    // Custom values
    person2->name = "Arda";
    person2->location = "boun";
    // Add person instance to the people array
    addPerson(&peopleCount, &people, person2, &peopleMaxCount);

    printf("%s number is: %d\n", (person2->itemNames)[0], (person2->itemNumbers)[0]);

    // Add a dummy item for person1
    newItem = "muz";
    addItem(&(person2->itemCount), &(person2->itemNames), newItem, &(person2->itemMaxCount), &(person2->itemNumbers));

    printf("%s number is: %d\n", (person2->itemNames)[0], (person2->itemNumbers)[0]);

    // Add the same dummy item for person2 again
    newItem = "muz";
    addItem(&(person2->itemCount), &(person2->itemNames), newItem, &(person2->itemMaxCount), &(person2->itemNumbers));

    printf("%s number is: %d\n", (person2->itemNames)[0], (person2->itemNumbers)[0]);

    // Add a different dummy item for person1
    newItem = "cilek";
    addItem(&(person2->itemCount), &(person2->itemNames), newItem, &(person2->itemMaxCount), &(person2->itemNumbers));

    printf("%s number is: %d\n", (person2->itemNames)[0], (person2->itemNumbers)[0]);
    printf("%s number is: %d\n", (person2->itemNames)[1], (person2->itemNumbers)[1]);
    printf("\n\n");
    /* END OF DUMMY TEST */

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

void addItem(int *itemCount, char ***itemNames, char *newItem, int *itemMaxCount, int **itemNumbers) {
    // Check if the item to be added already exists
    for(int i = 0; i < (*itemCount); i++) {
        if(strcmp((*itemNames)[i], newItem) == 0) {  // Item already exists, increment its number
            (*itemNumbers)[i]++;
            return;
        }
    }

    // This is a new item
    (*itemNames)[*itemCount] = (char *)malloc(sizeof(*newItem));
    strcpy((*itemNames)[*itemCount], newItem);
    (*itemNumbers)[*itemCount] = 1;
    (*itemCount)++;

    // If array is full, double its size
    if(*itemCount == *itemMaxCount) {
        *itemMaxCount *= 2;
        *itemNames = (char **)realloc(*itemNames, (*itemMaxCount) * sizeof(char *));
        *itemNumbers = (int *)realloc(*itemNumbers, (*itemMaxCount) * sizeof(int));
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
