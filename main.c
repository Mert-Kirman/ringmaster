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
// Array of actions that will be used for parsing a line
char actions[3][5] = {"sell", "buy", "go"};
// Array of conditions that will be used for parsing a line
char conditions[2][4] = {"at", "has"};

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

// Add a new sentence to an array of sentences
void addSentence(int *sentencesCount, char ****sentences, char **newSentence, int *sentencesMaxCount, int **wordCountInEachSentence, int sentenceWordCount);

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
    person1->name = "mert";
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
    person2->name = "arda";
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

        // NOT DECIDED: WHAT TO DO IN CASE OF BLANK LINE ?

        if(strcmp(line, "exit\n") == 0) {
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

        // Question Sentences

        // Question 1: who at ... ?
        if(strcmp(words[0], "who") == 0) {
            // Check if the question format is correct
            if(wordCount != 4){
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
            for(int i = 0; i < atLocationCount - 1; i++) {
                printf("%s and ", atLocation[i]);
            }
            printf("%s\n", atLocation[atLocationCount - 1]);


            free(atLocation);
            free(words);
            continue;
        }

        // Question 2-3: total ? / where ?
        if(wordCount == 3) {
            // Check if the first word is a forbidden word
            if(isKeyword(words[0]) == true || isForbiddenWord(words[0]) == true) {
                printf("INVALID\n");
                continue;
            }

            if(strcmp(words[1], "total") == 0) {
                if(strcmp(words[2], "?") != 0) {
                    printf("INVALID\n");
                    continue;
                }

                char *subjectName = words[0];
                int subjectFound = false;
                for(int i = 0; i < peopleCount; i++) {
                    if(strcmp(people[i]->name, subjectName) == 0) {  // Subject found
                        subjectFound = true;
                        int hasItem = false;
                        int itemCount = people[i]->itemCount;
                        // Print the items of the subject with their counts
                        for(int j = 0; j < itemCount - 1; j++) {
                            if(people[i]->itemNumbers[j] > 0) {
                                hasItem = true;
                                printf("%d %s and ", people[i]->itemNumbers[j], people[i]->itemNames[j]);
                            }
                        }

                        if(people[i]->itemNumbers[itemCount - 1] > 0) {
                            hasItem = true;
                            printf("%d %s\n", people[i]->itemNumbers[itemCount - 1], people[i]->itemNames[itemCount - 1]);
                        }

                        if(hasItem == false) {  // Subject does not have any items
                            printf("NOTHING\n");
                        }
                        break;
                    }
                }

                if(subjectFound == false) {  // NOT DECIDED: Subject does not exist yet
                    printf("INVALID\n");
                }
                continue;
            }

            if(strcmp(words[1], "where") == 0) {
                if(strcmp(words[2], "?") != 0) {
                    printf("INVALID\n");
                    continue;
                }

                char *subjectName = words[0];
                int subjectFound = false;
                for(int i = 0; i < peopleCount; i++) {
                    if(strcmp(people[i]->name, subjectName) == 0) {
                        subjectFound = true;
                        printf("%s\n", people[i]->location);
                        break;
                    }
                }

                if(subjectFound == false) {
                    printf("INVALID\n");
                }
                continue;
            }
        }

        // Question 4: total ... ?
        int totalQuestion = false;
        int totalIndex;
        for(int i = 0; i < wordCount; i++) {
            if(strcmp(words[i], "total") == 0) {
                totalQuestion = true;
                totalIndex = i;
                break;
            }
        }

        if(totalQuestion == true) {
            // Check formatting of the question
            if(totalIndex + 3 != wordCount) {
                printf("INVALID\n");
                continue;
            }
            if(isKeyword(words[totalIndex + 1]) == true || isForbiddenWord(words[totalIndex + 1]) == true) {  // Check if item name is forbidden
                printf("INVALID\n");
                continue;
            }
            if(strcmp(words[totalIndex + 2], "?") != 0) {
                printf("INVALID\n");
                continue;
            }
            if(strcmp(words[totalIndex - 1], "and") == 0) {
                printf("INVALID\n");
                continue;
            }

            int subjectCount = 0;  // Amount of current subjects we want to look for
            char *subjects[200];
            int invalidFormat = false;
            for(int i = 0; i < totalIndex; i++) {
                if(i % 2 == 1) {  // Words with odd indices should be "and"
                    if(strcmp(words[i], "and") != 0) {
                        printf("INVALID\n");
                        invalidFormat = true;
                        break;
                    }
                    continue;  // Do not add "and" to subjects array
                }

                // Check if subject names are forbidden words
                if(isKeyword(words[i]) == true || isForbiddenWord(words[i]) == true) {
                    printf("INVALID\n");
                    invalidFormat = true;
                    break;
                }

                subjects[i] = words[i];
                subjectCount++;
            }

            if(invalidFormat == true) {
                continue;
            }

            // Print total amount of items of the subjects
            int totalItemCount = 0;
            int validPeople = 0;  // Keep track of the subjects that exist in our people array
            for(int i = 0; i < subjectCount; i++) {
                for(int j = 0; j < peopleCount; j++) {
                    if(strcmp(people[j]->name, subjects[i]) == 0) {
                        validPeople++;
                        for(int k = 0; k < people[j]->itemCount; k++) {
                            if(strcmp(people[j]->itemNames[k], words[totalIndex + 1]) == 0) {
                                totalItemCount += people[j]->itemNumbers[k];
                            }
                        }
                    }
                }
            }

            if(validPeople != subjectCount) {  // There are subjects that do not exist in people array
                printf("INVALID\n");
                continue;
            }

            printf("%d\n", totalItemCount);
            continue;
        }

        // Break current line into action sentences
        int sentencesCount = 0;
        int sentencesMaxCount = 1;
        char ***sentences = (char ***)malloc(sentencesMaxCount * sizeof(char **));
        int *wordCountInEachSentence = (int *)malloc(sentencesMaxCount * sizeof(int));  // Integer array that keeps the word count in each sentence inside "sentences" array

        int startIndexOfCurrentSentence = 0;  // Starting point of the sentence we are currently working on
        for(int i = 0; i < wordCount; i++) {
            for(int j = 0; j < 3; j++) {  // Check if this word is an action word
                if(strcmp(words[i], actions[j]) == 0) {  // Find out what kind of action word this is
                    if(strcmp(actions[j], "go") == 0) {
                        int sentenceWordCount = 0;  // Current count of words inside this sentence
                        int sentenceWordMaxCount = 1;  // Max count of words this sentence is capable of holding
                        char **newSentence = (char **)malloc(sentenceWordCount * sizeof(char *));

                        for(int k = startIndexOfCurrentSentence; k < i + 3; k++) {
                            addElement(&sentenceWordCount, &newSentence, words[k], &sentenceWordMaxCount);  // Create a sentence which has action "go"
                        }

                        addSentence(&sentencesCount, &sentences, newSentence, &sentencesMaxCount, &wordCountInEachSentence, sentenceWordCount);  // Add this newly formed sentence to the sentences array
                        i += 2;  // Bypass the following 2 words: "to [location]"
                        startIndexOfCurrentSentence = i + 1;
                        break;
                    }

                    if(strcmp(actions[j], "sell") == 0) {
                        ;
                    }

                    if(strcmp(actions[j], "buy") == 0) {
                        ;
                    }
                }
            }
        }

        for(int i = 0; i < sentencesCount; i++) {
            for(int j = 0; j < wordCountInEachSentence[i]; j++) {
                printf("%s ", sentences[i][j]);
            }
            printf("\n");
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

void addSentence(int *sentencesCount, char ****sentences, char **newSentence, int *sentencesMaxCount, int **wordCountInEachSentence, int sentenceWordCount) {
    (*sentences)[*sentencesCount] = newSentence;
    (*wordCountInEachSentence)[*sentencesCount] = sentenceWordCount;
    (*sentencesCount)++;

    if(*sentencesCount == *sentencesMaxCount) {  // Array is full, double its size
        *sentencesMaxCount *= 2;
        *sentences = (char ***)realloc(*sentences, (*sentencesMaxCount) * sizeof(char **));
        *wordCountInEachSentence = (int *)realloc(*wordCountInEachSentence, (*sentencesMaxCount) * sizeof(int));
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
