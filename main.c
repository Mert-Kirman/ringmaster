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

// Add a new atomic sentence to an array of atomic sentences(big sentence)
void addAtomicSentence(int *sentencesCount, char ****atomicSentences, char **newAtomicSentence, int *sentencesMaxCount);

// Add a new big sentence to an array of big sentences
void addBigSentence(int *sentencesCount, char *****bigSentences, char ***newBigSentence, int *sentencesMaxCount);

// Add a new person to people array
void addPerson(int *peopleCount, struct Person ***people, struct Person *newPerson, int *peopleMaxCount);

// Add a new or existing item to a person's item list
void addItem(int *itemCount, char ***itemNames, char *newItem, int * itemMaxCount, int **itemNumbers);

// Add an integer to an int array
void addInteger(int *integerCount, int **integers, int integer, int *integersMaxSize);

// Check if a word is a keyword
int isKeyword(char *word);

// Check if a word is forbidden
int isForbiddenWord(char *word);

// Check if a word is an action word
int isActionWord(char *word);

// Check if a word is a condition word
int isConditionWord(char *word);

int main() {

    char line[1025];

    // Create an array of person structs with max capacity of 1
    int peopleCount = 0;
    int peopleMaxCount = 1;
    struct Person **people;
    people = (struct Person **)malloc(peopleMaxCount * sizeof(struct Person *));

    while (1) {
        printf(">> ");
        fflush(stdout);

        if (fgets(line, 1025, stdin) == NULL) {
            break;
        }

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

                if(subjectFound == false) {  // Subject does not exist yet
                    printf("NOTHING\n");
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
            for(int i = 0; i < subjectCount; i++) {
                for(int j = 0; j < peopleCount; j++) {
                    if(strcmp(people[j]->name, subjects[i]) == 0) {
                        for(int k = 0; k < people[j]->itemCount; k++) {
                            if(strcmp(people[j]->itemNames[k], words[totalIndex + 1]) == 0) {
                                totalItemCount += people[j]->itemNumbers[k];
                            }
                        }
                    }
                }
            }

            printf("%d\n", totalItemCount);
            continue;
        }

        // Break current sequential line into simpler if sentences
        int sentencesCount = 0;
        int sentencesMaxCount = 1;
        char ***sentences = (char ***)malloc(sentencesMaxCount * sizeof(char **));
        int *wordCountInEachSentence = (int *)malloc(sentencesMaxCount * sizeof(int));  // Integer array that keeps the word count in each sentence inside "sentences" array

        int startIndexOfCurrentSentence = 0;  // Starting point of the sentence we are currently working on
        int ifIndexCount = 0;
        int ifIndexMaxCount = 1;
        int *ifIndexArray = (int *)malloc(ifIndexMaxCount * sizeof(int));  // Keep track of the indices of each "if" word
        for(int i = 0; i < wordCount; i++) {  // Get the number of if sentences
            if(strcmp(words[i], "if") == 0) {
                addInteger(&ifIndexCount, &ifIndexArray, i, &ifIndexMaxCount);  // Add the index of this "if" word to the indices array
            }
        }

        for(int i = 0; i < ifIndexCount; i++) {  // There is at least 1 if sentence
            int indexOfIf = ifIndexArray[i];

            // Find the start of the next sentence after this if sentence
            int indexOfLastConditionWord;
            int indexOfNextSentenceAction = -1;  // Index of the action word in the beginning of the next sentence
            for(int j = indexOfIf + 1; j < wordCount; j++) {
                if(isConditionWord(words[j]) == true) {
                    indexOfLastConditionWord = j;
                }
                else if(isActionWord(words[j]) == true) {
                    indexOfNextSentenceAction = j;
                    break;
                }
            }

            // Separate if sentences according to 6 cases
            int startIndexOfNextSentence;
            if(strcmp(words[indexOfLastConditionWord], "at") == 0) {  // Case 1, 2, 3: If condition is "at", starting index of the next sentence is 'indexOfLastConditionWord + 3'
                startIndexOfNextSentence = indexOfLastConditionWord + 3;

                int sentenceWordCount = 0;  // Current count of words inside this sentence
                int sentenceWordMaxCount = 1;  // Max count of words this sentence is capable of holding
                char **newSentence = (char **)malloc(sentenceWordMaxCount * sizeof(char *));

                for(int j = startIndexOfCurrentSentence; j < startIndexOfNextSentence - 1; j++) {  // -1 because we don't want to take "and" words that combine "if sentences"
                    addElement(&sentenceWordCount, &newSentence, words[j], &sentenceWordMaxCount);  // Create the ith "if" sentence
                }
                addSentence(&sentencesCount, &sentences, newSentence, &sentencesMaxCount, &wordCountInEachSentence, sentenceWordCount);  // Add this newly formed if sentence to the sentences array

                startIndexOfCurrentSentence = startIndexOfNextSentence;
            }
            else if(strcmp(words[indexOfLastConditionWord], "has") == 0) {  // Case 4, 5, 6: If condition is "has", iterate until you come across a word without number preceding it
                if(indexOfNextSentenceAction == -1) {  // There is no "basic" or "if" sentence succeeding this "if" sentence, this method will not work; treat this solo "if" sentence as a "basic" sentence
                    break;
                }
                for(int j = indexOfNextSentenceAction - 2; j > indexOfLastConditionWord; j -= 2) {  // Iterate back until you don't come across "and"
                    if(strcmp(words[j], "and") == 0) {
                        startIndexOfNextSentence = j + 1;
                        continue;
                    }

                    break;
                }

                int sentenceWordCount = 0;  // Current count of words inside this sentence
                int sentenceWordMaxCount = 1;  // Max count of words this sentence is capable of holding
                char **newSentence = (char **)malloc(sentenceWordMaxCount * sizeof(char *));

                for(int j = startIndexOfCurrentSentence; j < startIndexOfNextSentence - 1; j++) {
                    addElement(&sentenceWordCount, &newSentence, words[j], &sentenceWordMaxCount);  // Create the ith "if" sentence
                }
                addSentence(&sentencesCount, &sentences, newSentence, &sentencesMaxCount, &wordCountInEachSentence, sentenceWordCount);  // Add this newly formed if sentence to the sentences array

                startIndexOfCurrentSentence = startIndexOfNextSentence;
            }
        }

        // Store the remaining basic sentence if it exists
        int sentenceWordCount = 0;  // Current count of words inside this sentence
        int sentenceWordMaxCount = 1;  // Max count of words this sentence is capable of holding
        char **newSentence = (char **)malloc(sentenceWordMaxCount * sizeof(char *));

        for(int i = startIndexOfCurrentSentence; i < wordCount; i++) {
            addElement(&sentenceWordCount, &newSentence, words[i], &sentenceWordMaxCount);  // Create the basic sentence which, if exists, is the last sentence of the line
        }
        addSentence(&sentencesCount, &sentences, newSentence, &sentencesMaxCount, &wordCountInEachSentence, sentenceWordCount);  // Add this newly formed basic sentence to the sentences array

        // Break the basic and if sentences into atomic(containing only 1 keyword) action and conditional sentences
        int bigSentencesActionPartCount = 0;
        int bigSentencesActionPartMaxCount = 1;
        char ****bigSentencesActionPart = (char ****)malloc(bigSentencesActionPartMaxCount * sizeof(char *));  // Array which contains basic sentences or parts of if sentences before the "if" word

        int bigSentencesConditionalPartCount = 0;
        int bigSentencesConditionalPartMaxCount = 1;
        char ****bigSentencesConditionalPart = (char ****)malloc(bigSentencesConditionalPartMaxCount * sizeof(char *));  // Array which contains parts of if sentences after the "if" word

        int validFormat = true;

        int atomicActionSentenceCount[sentencesCount];  // Integer array that keeps the count of atomic action sentences in each big sentence
        int atomicConditionalSentenceCount[sentencesCount];

        int atomicActionSentenceWordCount[sentencesCount][256];  // 2D array that keeps the number of words in each atomic sentence in its columns, each row is a different big sentence
        int atomicConditionalSentenceWordCount[sentencesCount][256];

        for(int i = 0; i < sentencesCount; i++) {  // Initialize above arrays
            atomicActionSentenceCount[i] = 0;
            atomicConditionalSentenceCount[i] = 0;
            for(int j = 0; j < 256; j++) {
                atomicActionSentenceWordCount[i][j] = 0;
                atomicConditionalSentenceWordCount[i][j] = 0;
            }
        }

        for(int i = 0; i < sentencesCount; i++) {
            int actionSentencesCount = 0;
            int actionSentencesMaxCount = 1;
            char ***actionSentences = (char ***)malloc(actionSentencesMaxCount * sizeof(char **));  // Contains atomic action sentences in each sentence

            int conditionalSentencesCount = 0;
            int conditionalSentencesMaxCount = 1;
            char ***conditionalSentences = (char ***)malloc(conditionalSentencesMaxCount * sizeof(char **));

            int atomicSentenceStartIndex = 0;
            int atomicSentenceEndIndex = 0;
            for(int j = 0; j < wordCountInEachSentence[i]; j++) {
                if(atomicSentenceStartIndex > j) {
                    continue;
                }

                // Action atomic sentence
                if(strcmp(sentences[i][j], "go") == 0) {  // If the basic sentence consists of "go" action
                    atomicSentenceEndIndex = j + 2;

                    // Create an atomic sentence
                    int atomicSentenceWordCount = 0;
                    int atomicSentenceWordMaxCount = 1;
                    char **newAtomicActionSentence = (char **)malloc(atomicSentenceWordMaxCount * sizeof(char *));

                    for(int k = atomicSentenceStartIndex; k <= atomicSentenceEndIndex; k++) {
                        addElement(&atomicSentenceWordCount, &newAtomicActionSentence, sentences[i][k], &atomicSentenceWordMaxCount);
                    }

                    // Add this newly formed atomic sentence
                    addAtomicSentence(&actionSentencesCount, &actionSentences, newAtomicActionSentence, &actionSentencesMaxCount);
                    atomicActionSentenceCount[i]++;
                    atomicActionSentenceWordCount[i][actionSentencesCount - 1] = atomicSentenceWordCount;

                    atomicSentenceStartIndex = atomicSentenceEndIndex + 2;

                    if(atomicSentenceEndIndex != wordCountInEachSentence[i] - 1) {
                        // Check if atomic sentences are connected with "and" keyword, in accordance with the format
                        if(strcmp(sentences[i][atomicSentenceEndIndex + 1], "and") != 0 && strcmp(sentences[i][atomicSentenceEndIndex + 1], "if") != 0) {
                            validFormat = false;
                        }
                    }
                }
                else if(strcmp(sentences[i][j], "buy") == 0) {
                    for(int k = j; k < wordCountInEachSentence[i]; k+=3) {
                        if(k + 2 == wordCountInEachSentence[i] - 1) {  // If "buy" is the last action in a sentence, k + 3 will always give false
                            atomicSentenceEndIndex = k + 2;
                            break;
                        }
                        else if((k + 3 < wordCountInEachSentence[i]) && (strcmp(sentences[i][k + 3], "if") == 0)) {
                            atomicSentenceEndIndex = k + 2;
                            break;
                        }
                        else if((k + 3 < wordCountInEachSentence[i]) && (strcmp(sentences[i][k + 3], "from") == 0)) {
                            atomicSentenceEndIndex = k + 4;
                            break;
                        }
                        else if((k + 3 < wordCountInEachSentence[i]) && (strcmp(sentences[i][k + 3], "and") == 0)) {
                            if(48 > sentences[i][k + 4][0] || sentences[i][k + 4][0] > 57) {  // If the word after "and" is not a number
                                atomicSentenceEndIndex = k + 2;
                                break;
                            }
                        }
                    }

                    // Create an atomic sentence
                    int atomicSentenceWordCount = 0;
                    int atomicSentenceWordMaxCount = 1;
                    char **newAtomicActionSentence = (char **)malloc(atomicSentenceWordMaxCount * sizeof(char *));

                    for(int k = atomicSentenceStartIndex; k <= atomicSentenceEndIndex; k++) {
                        addElement(&atomicSentenceWordCount, &newAtomicActionSentence, sentences[i][k], &atomicSentenceWordMaxCount);
                    }
                    if(atomicSentenceWordCount == 0) {  // Sentence is empty because atomicSentenceEndIndex is pointed to the previous atomic sentence's end index due to faulty format "Frodo buy and ..."
                        validFormat = false;
                        break;
                    }

                    addAtomicSentence(&actionSentencesCount, &actionSentences, newAtomicActionSentence, &actionSentencesMaxCount);
                    atomicActionSentenceCount[i]++;
                    atomicActionSentenceWordCount[i][actionSentencesCount - 1] = atomicSentenceWordCount;

                    atomicSentenceStartIndex = atomicSentenceEndIndex + 2;

                    if(atomicSentenceEndIndex != wordCountInEachSentence[i] - 1) {
                        // Check if atomic sentences are connected with "and" keyword, in accordance with the format
                        if(strcmp(sentences[i][atomicSentenceEndIndex + 1], "and") != 0 && strcmp(sentences[i][atomicSentenceEndIndex + 1], "if") != 0) {
                            validFormat = false;
                        }
                    }
                }
                else if(strcmp(sentences[i][j], "sell") == 0) {
                    for(int k = j; k < wordCountInEachSentence[i]; k+=3) {
                        if(k + 2 == wordCountInEachSentence[i] - 1) {  // If "sell" is the last action in a sentence, k + 3 will always give false
                            atomicSentenceEndIndex = k + 2;
                            break;
                        }
                        else if((k + 3 < wordCountInEachSentence[i]) && (strcmp(sentences[i][k + 3], "if") == 0)) {
                            atomicSentenceEndIndex = k + 2;
                            break;
                        }
                        else if((k + 3 < wordCountInEachSentence[i]) && (strcmp(sentences[i][k + 3], "to") == 0)) {
                            atomicSentenceEndIndex = k + 4;
                            break;
                        }
                        else if((k + 3 < wordCountInEachSentence[i]) && (strcmp(sentences[i][k + 3], "and") == 0)) {
                            if(48 > sentences[i][k + 4][0] || sentences[i][k + 4][0] > 57) {  // If the word after "and" is not a number
                                atomicSentenceEndIndex = k + 2;
                                break;
                            }
                        }
                    }

                    // Create an atomic sentence
                    int atomicSentenceWordCount = 0;
                    int atomicSentenceWordMaxCount = 1;
                    char **newAtomicActionSentence = (char **)malloc(atomicSentenceWordMaxCount * sizeof(char *));

                    for(int k = atomicSentenceStartIndex; k <= atomicSentenceEndIndex; k++) {
                        addElement(&atomicSentenceWordCount, &newAtomicActionSentence, sentences[i][k], &atomicSentenceWordMaxCount);
                    }
                    if(atomicSentenceWordCount == 0) {
                        validFormat = false;
                        break;
                    }

                    addAtomicSentence(&actionSentencesCount, &actionSentences, newAtomicActionSentence, &actionSentencesMaxCount);
                    atomicActionSentenceCount[i]++;
                    atomicActionSentenceWordCount[i][actionSentencesCount - 1] = atomicSentenceWordCount;

                    atomicSentenceStartIndex = atomicSentenceEndIndex + 2;

                    if(atomicSentenceEndIndex != wordCountInEachSentence[i] - 1) {
                        // Check if atomic sentences are connected with "and" keyword, in accordance with the format
                        if(strcmp(sentences[i][atomicSentenceEndIndex + 1], "and") != 0 && strcmp(sentences[i][atomicSentenceEndIndex + 1], "if") != 0) {
                            validFormat = false;
                        }
                    }
                }
                // Conditional Atomic Sentence
                else if(strcmp(sentences[i][j], "at") == 0) {
                    atomicSentenceEndIndex = j + 1;

                    // Create an atomic sentence
                    int atomicSentenceWordCount = 0;
                    int atomicSentenceWordMaxCount = 1;
                    char **newAtomicConditionalSentence = (char **)malloc(atomicSentenceWordMaxCount * sizeof(char *));

                    for(int k = atomicSentenceStartIndex; k <= atomicSentenceEndIndex; k++) {
                        addElement(&atomicSentenceWordCount, &newAtomicConditionalSentence, sentences[i][k], &atomicSentenceWordMaxCount);
                    }
                    addAtomicSentence(&conditionalSentencesCount, &conditionalSentences, newAtomicConditionalSentence, &conditionalSentencesMaxCount);
                    atomicConditionalSentenceCount[i]++;
                    atomicConditionalSentenceWordCount[i][conditionalSentencesCount - 1] = atomicSentenceWordCount;

                    atomicSentenceStartIndex = atomicSentenceEndIndex + 2;

                    if(atomicSentenceEndIndex != wordCountInEachSentence[i] - 1) {
                        // Check if atomic sentences are connected with "and" keyword, in accordance with the format
                        if(strcmp(sentences[i][atomicSentenceEndIndex + 1], "and") != 0) {
                            validFormat = false;
                        }
                    }
                }
                else if(strcmp(sentences[i][j], "has") == 0) {
                    if((j + 2 < wordCountInEachSentence[i]) && (strcmp(sentences[i][j+2], "than") == 0)) {
                        j += 2;
                    }
                    if(j + 2 == wordCountInEachSentence[i] - 1) {  // If "has" is the last condition in a sentence, k + 3 will always give false
                        atomicSentenceEndIndex = j + 2;
                    }
                    for(int k = j; k < wordCountInEachSentence[i]; k+=3) {
                        if((k + 3 < wordCountInEachSentence[i]) && (strcmp(sentences[i][k + 3], "and") == 0)) {
                            if((k + 4 < wordCountInEachSentence[i]) && (48 > sentences[i][k + 4][0] || sentences[i][k + 4][0] > 57)) {  // If the word after "and" is not a number
                                atomicSentenceEndIndex = k + 2;
                                break;
                            }
                        }
                    }

                    // Create an atomic sentence
                    int atomicSentenceWordCount = 0;
                    int atomicSentenceWordMaxCount = 1;
                    char **newAtomicConditionalSentence = (char **)malloc(atomicSentenceWordMaxCount * sizeof(char *));

                    for(int k = atomicSentenceStartIndex; k <= atomicSentenceEndIndex; k++) {
                        addElement(&atomicSentenceWordCount, &newAtomicConditionalSentence, sentences[i][k], &atomicSentenceWordMaxCount);
                    }
                    if(atomicSentenceWordCount == 0) {
                        validFormat = false;
                        break;
                    }

                    addAtomicSentence(&conditionalSentencesCount, &conditionalSentences, newAtomicConditionalSentence, &conditionalSentencesMaxCount);
                    atomicConditionalSentenceCount[i]++;
                    atomicConditionalSentenceWordCount[i][conditionalSentencesCount - 1] = atomicSentenceWordCount;

                    atomicSentenceStartIndex = atomicSentenceEndIndex + 2;

                    if(atomicSentenceEndIndex != wordCountInEachSentence[i] - 1) {
                        // Check if atomic sentences are connected with "and" keyword, in accordance with the format
                        if(strcmp(sentences[i][atomicSentenceEndIndex + 1], "and") != 0) {
                            validFormat = false;
                        }
                    }
                }

            }

            if(validFormat == false) {
                break;
            }

            // Store each big sentence
            addBigSentence(&bigSentencesActionPartCount, &bigSentencesActionPart, actionSentences, &bigSentencesActionPartMaxCount);
            addBigSentence(&bigSentencesConditionalPartCount, &bigSentencesConditionalPart, conditionalSentences, &bigSentencesConditionalPartMaxCount);
        }

        if(validFormat == false) {  // If atomic sentences are not connected via "and", print "INVALID"
            printf("INVALID\n");

            // Free allocated memory
            for(int i = 0; i < sentencesCount; i++) {
                free(sentences[i]);
            }
            free(sentences);

            free(wordCountInEachSentence);
            free(ifIndexArray);

            for(int i = 0; i < bigSentencesActionPartCount; i++) {
                for(int j = 0; j < atomicActionSentenceCount[i]; j++) {
                    free(bigSentencesActionPart[i][j]);
                }
                free(bigSentencesActionPart[i]);
            }
            free(bigSentencesActionPart);

            for(int i = 0; i < bigSentencesConditionalPartCount; i++) {
                for(int j = 0; j < atomicConditionalSentenceCount[i]; j++) {
                    free(bigSentencesConditionalPart[i][j]);
                }
                free(bigSentencesConditionalPart[i]);
            }
            free(bigSentencesConditionalPart);

            free(words);
            continue;
        }

        // Check formatting further
        int breakCompletely = false;
        for(int j = 0; j < sentencesCount; j++) {
            // Check atomic action sentence formatting
            for(int k = 0; k < atomicActionSentenceCount[j]; k++) {
                for(int l = 0; l < atomicActionSentenceWordCount[j][k]; l++) {
                    if(strcmp(bigSentencesActionPart[j][k][l], "go") == 0) {
                        // After the "go" action keyword
                        if(strcmp(bigSentencesActionPart[j][k][l + 1], "to") != 0) {
                            validFormat = false;
                            breakCompletely = true;
                            break;
                        }
                        if(isForbiddenWord(bigSentencesActionPart[j][k][l + 2]) == true || isKeyword(bigSentencesActionPart[j][k][l + 2]) == true) {
                            validFormat = false;
                            breakCompletely = true;
                            break;
                        }

                        // Before the "go" action keyword
                        for(int m = 0; m < l; m++) {  // Check if names are connected via "and"
                            if(m % 2 == 0) {
                                if(isForbiddenWord(bigSentencesActionPart[j][k][m]) == true || isKeyword(bigSentencesActionPart[j][k][m]) == true) {  // People names are invalid
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                            }
                            else {
                                if(strcmp(bigSentencesActionPart[j][k][m], "and") != 0) {
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    else if(strcmp(bigSentencesActionPart[j][k][l], "buy") == 0) {
                        // Before the "buy" action keyword
                        for(int m = 0; m < l; m++) {  // Check if names are connected via "and"
                            if(m % 2 == 0) {
                                if(isForbiddenWord(bigSentencesActionPart[j][k][m]) == true || isKeyword(bigSentencesActionPart[j][k][m]) == true) {  // People names are invalid
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                            }
                            else {
                                if(strcmp(bigSentencesActionPart[j][k][m], "and") != 0) {
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                            }
                        }
                        if(breakCompletely) {
                            break;
                        }

                        // After the "buy" action keyword
                        for(int m = l; m < atomicActionSentenceWordCount[j][k]; m+=3) {
                            if(m + 2 == atomicActionSentenceWordCount[j][k] - 1) {
                                if(bigSentencesActionPart[j][k][m + 1][0] < 48 || bigSentencesActionPart[j][k][m + 1][0] > 57) {  // Check if buy is succeeded by a numeric value
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                                if(isKeyword(bigSentencesActionPart[j][k][m + 2]) == true || isForbiddenWord(bigSentencesActionPart[j][k][m + 2]) == true) {
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                                break;
                            }
                            else if(strcmp(bigSentencesActionPart[j][k][m], "from") == 0) {
                                if(m + 1 != atomicActionSentenceWordCount[j][k] - 1) {  // There should be only 1 subject after the "from" keyword
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                                else if(isKeyword(bigSentencesActionPart[j][k][m + 1]) == true || isForbiddenWord(bigSentencesActionPart[j][k][m + 1]) == true) {  // Subject name should be valid
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                            }
                            else if(strcmp(bigSentencesActionPart[j][k][m + 3], "and") != 0 && strcmp(bigSentencesActionPart[j][k][m + 3], "from") != 0) {
                                validFormat = false;
                                breakCompletely = true;
                                break;
                            }
                            else {
                                if(bigSentencesActionPart[j][k][m + 1][0] < 48 || bigSentencesActionPart[j][k][m + 1][0] > 57) {  // Check if buy is succeeded by a numeric value
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                                if(isKeyword(bigSentencesActionPart[j][k][m + 2]) == true || isForbiddenWord(bigSentencesActionPart[j][k][m + 2]) == true) {
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    else if(strcmp(bigSentencesActionPart[j][k][l], "sell") == 0) {
                        // Before the "sell" action keyword
                        for(int m = 0; m < l; m++) {  // Check if names are connected via "and"
                            if(m % 2 == 0) {
                                if(isForbiddenWord(bigSentencesActionPart[j][k][m]) == true || isKeyword(bigSentencesActionPart[j][k][m]) == true) {  // People names are invalid
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                            }
                            else {
                                if(strcmp(bigSentencesActionPart[j][k][m], "and") != 0) {
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                            }
                        }
                        if(breakCompletely) {
                            break;
                        }

                        // After the "sell" action keyword
                        for(int m = l; m < atomicActionSentenceWordCount[j][k]; m+=3) {
                            if(m + 2 == atomicActionSentenceWordCount[j][k] - 1) {
                                if(bigSentencesActionPart[j][k][m + 1][0] < 48 || bigSentencesActionPart[j][k][m + 1][0] > 57) {  // Check if sell is succeeded by a numeric value
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                                if(isKeyword(bigSentencesActionPart[j][k][m + 2]) == true || isForbiddenWord(bigSentencesActionPart[j][k][m + 2]) == true) {
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                                break;
                            }
                            else if(strcmp(bigSentencesActionPart[j][k][m], "to") == 0) {
                                if(m + 1 != atomicActionSentenceWordCount[j][k] - 1) {  // There should be only 1 subject after the "to" keyword
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                                else if(isKeyword(bigSentencesActionPart[j][k][m + 1]) == true || isForbiddenWord(bigSentencesActionPart[j][k][m + 1]) == true) {  // Subject name should be valid
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                            }
                            else if(strcmp(bigSentencesActionPart[j][k][m + 3], "and") != 0 && strcmp(bigSentencesActionPart[j][k][m + 3], "to") != 0) {
                                validFormat = false;
                                breakCompletely = true;
                                break;
                            }
                            else {
                                if(bigSentencesActionPart[j][k][m + 1][0] < 48 || bigSentencesActionPart[j][k][m + 1][0] > 57) {  // Check if sell is succeeded by a numeric value
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                                if(isKeyword(bigSentencesActionPart[j][k][m + 2]) == true || isForbiddenWord(bigSentencesActionPart[j][k][m + 2]) == true) {
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
                if(breakCompletely) {
                    break;
                }
            }
            if(breakCompletely) {
                break;
            }

            // Check atomic conditional sentence formatting
            for(int k = 0; k < atomicConditionalSentenceCount[j]; k++) {
                for(int l = 0; l < atomicConditionalSentenceWordCount[j][k]; l++) {
                    if(strcmp(bigSentencesConditionalPart[j][k][l], "at") == 0) {
                        // After the "at" conditional keyword
                        if(isForbiddenWord(bigSentencesConditionalPart[j][k][l + 1]) == true || isKeyword(bigSentencesConditionalPart[j][k][l + 1]) == true) {
                            validFormat = false;
                            breakCompletely = true;
                            break;
                        }

                        // Before the "at" conditional keyword
                        for(int m = 0; m < l; m++) {  // Check if names are connected via "and"
                            if(m % 2 == 0) {
                                if(isForbiddenWord(bigSentencesConditionalPart[j][k][m]) == true || isKeyword(bigSentencesConditionalPart[j][k][m]) == true) {  // People names are invalid
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                            }
                            else {
                                if(strcmp(bigSentencesConditionalPart[j][k][m], "and") != 0) {
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    else if(strcmp(bigSentencesConditionalPart[j][k][l], "has") == 0) {
                        // Before the "has" conditional keyword
                        for(int m = 0; m < l; m++) {  // Check if names are connected via "and"
                            if(m % 2 == 0) {
                                if(isForbiddenWord(bigSentencesConditionalPart[j][k][m]) == true || isKeyword(bigSentencesConditionalPart[j][k][m]) == true) {  // People names are invalid
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                            }
                            else {
                                if(strcmp(bigSentencesConditionalPart[j][k][m], "and") != 0) {
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                            }
                        }
                        if(breakCompletely) {
                            break;
                        }

                        // After the "has" conditional keyword
                        // Adjust the index of 'l' so that if we have "has more than" l should point to 'than', otherwise l should point to 'has'
                        if(strcmp(bigSentencesConditionalPart[j][k][l + 2], "than") == 0) {
                            if(strcmp(bigSentencesConditionalPart[j][k][l + 1], "less") != 0 && strcmp(bigSentencesConditionalPart[j][k][l + 1], "more") != 0) {  // It must be either "has less than" or "has more than"
                                validFormat = false;
                                breakCompletely = true;
                                break;
                            }
                            l += 2;
                        }
                        for(int m = l; m < atomicConditionalSentenceWordCount[j][k]; m+=3) {
                            if(m + 2 == atomicConditionalSentenceWordCount[j][k] - 1) {
                                if(bigSentencesConditionalPart[j][k][m + 1][0] < 48 || bigSentencesConditionalPart[j][k][m + 1][0] > 57) {  // Check if "has" or "than" is succeeded by a numeric value
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                                if(isKeyword(bigSentencesConditionalPart[j][k][m + 2]) == true || isForbiddenWord(bigSentencesConditionalPart[j][k][m + 2]) == true) {
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                                break;
                            }
                            else if(strcmp(bigSentencesConditionalPart[j][k][m + 3], "and") != 0) {
                                validFormat = false;
                                breakCompletely = true;
                                break;
                            }
                            else {
                                if(bigSentencesConditionalPart[j][k][m + 1][0] < 48 || bigSentencesConditionalPart[j][k][m + 1][0] > 57) {  // Check if "has" or "than" is succeeded by a numeric value
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                                if(isKeyword(bigSentencesConditionalPart[j][k][m + 2]) == true || isForbiddenWord(bigSentencesConditionalPart[j][k][m + 2]) == true) {
                                    validFormat = false;
                                    breakCompletely = true;
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
                if(breakCompletely) {
                    break;
                }
            }
            if(breakCompletely) {
                break;
            }
        }

        if(validFormat == false) {
            printf("INVALID\n");
        }
        else {
            printf("OK\n");
            for(int i = 0; i < sentencesCount; i++) {
                int breakCompletely2 = false;
                int allConditionsTrue = true;
                if(atomicConditionalSentenceCount[i] != 0) {  // There are conditions that need to be checked before processing actions
                    for(int j = 0; j < atomicConditionalSentenceCount[i]; j++) {
                        for(int k = 0; k < atomicConditionalSentenceWordCount[i][j]; k++) {
                            if(strcmp(bigSentencesConditionalPart[i][j][k], "at") == 0) {
                                char *location = bigSentencesConditionalPart[i][j][k + 1];
                                for(int l = 0; l < k; l+=2) {
                                    char *name = bigSentencesConditionalPart[i][j][l];
                                    int personFound = false;
                                    for(int m = 0; m < peopleCount; m++) {
                                        if(strcmp(people[m]->name, name) == 0) {  // Person found
                                            personFound = true;
                                            if(strcmp(people[m]->location, location) != 0) {
                                                allConditionsTrue = false;
                                                break;
                                            }
                                        }
                                    }
                                    if(personFound == false || allConditionsTrue == false) {
                                        allConditionsTrue = false;
                                        breakCompletely2 = true;
                                        break;
                                    }
                                }
                                if(breakCompletely2) {
                                    break;
                                }
                            }
                            else if(strcmp(bigSentencesConditionalPart[i][j][k], "has") == 0) {
                                if(strcmp(bigSentencesConditionalPart[i][j][k + 1], "less") == 0) {
                                    for(int l = 0; l < k; l+=2) {  // Traverse subjects in the atomic condition sentence
                                        char *name = bigSentencesConditionalPart[i][j][l];
                                        for(int m = k + 3; m < atomicConditionalSentenceWordCount[i][j]; m+=3) {  // Traverse items in the atomic condition sentence
                                            char *itemName = bigSentencesConditionalPart[i][j][m + 1];
                                            char *itemCountChar = bigSentencesConditionalPart[i][j][m];
                                            int itemCount = atoi(itemCountChar);

                                            for(int n = 0; n < peopleCount; n++) {  // Traverse people array
                                                if(strcmp(people[n]->name, name) == 0) {
                                                    for(int p = 0; p < people[n]->itemCount; p++) {  // Traverse a person structure's items
                                                        if(strcmp(people[n]->itemNames[p], itemName) == 0) {
                                                            if(people[n]->itemNumbers[p] >= itemCount) {  // Item number does not match to condition
                                                                allConditionsTrue = false;
                                                                breakCompletely2 = true;
                                                                break;
                                                            }
                                                        }
                                                    }
                                                    if(breakCompletely2) {
                                                        break;
                                                    }
                                                }
                                            }
                                            if(breakCompletely2) {
                                                break;
                                            }
                                        }
                                        if(breakCompletely2) {
                                            break;
                                        }
                                    }
                                }
                                else if(strcmp(bigSentencesConditionalPart[i][j][k], "more") == 0) {
                                    for(int l = 0; l < k; l+=2) {  // Traverse subjects in the atomic condition sentence
                                        char *name = bigSentencesConditionalPart[i][j][l];
                                        int personFound = false;
                                        for(int m = k + 3; m < atomicConditionalSentenceWordCount[i][j]; m+=3) {  // Traverse items in the atomic condition sentence
                                            char *itemName = bigSentencesConditionalPart[i][j][m + 1];
                                            char *itemCountChar = bigSentencesConditionalPart[i][j][m];
                                            int itemCount = atoi(itemCountChar);
                                            int itemFound = false;

                                            for(int n = 0; n < peopleCount; n++) {  // Traverse people array
                                                if(strcmp(people[n]->name, name) == 0) {
                                                    personFound = true;
                                                    for(int p = 0; p < people[n]->itemCount; p++) {  // Traverse a person structure's items
                                                        if(strcmp(people[n]->itemNames[p], itemName) == 0) {
                                                            itemFound = true;
                                                            if(people[n]->itemNumbers[p] <= itemCount) {  // Item number does not match to condition
                                                                allConditionsTrue = false;
                                                                breakCompletely2 = true;
                                                                break;
                                                            }
                                                        }
                                                    }
                                                    if(itemFound == false) {
                                                        allConditionsTrue = false;
                                                        breakCompletely2 = true;
                                                    }
                                                    if(breakCompletely2) {
                                                        break;
                                                    }
                                                }
                                            }
                                            if(personFound == false) {
                                                allConditionsTrue = false;
                                                breakCompletely2 = true;
                                            }
                                            if(breakCompletely2) {
                                                break;
                                            }
                                        }
                                        if(breakCompletely2) {
                                            break;
                                        }
                                    }
                                }
                                else {
                                    for(int l = 0; l < k; l+=2) {  // Traverse subjects in the atomic condition sentence
                                        char *name = bigSentencesConditionalPart[i][j][l];
                                        int personFound = false;
                                        for(int m = k + 3; m < atomicConditionalSentenceWordCount[i][j]; m+=3) {  // Traverse items in the atomic condition sentence
                                            char *itemName = bigSentencesConditionalPart[i][j][m + 1];
                                            char *itemCountChar = bigSentencesConditionalPart[i][j][m];
                                            int itemCount = atoi(itemCountChar);
                                            int itemFound = false;

                                            for(int n = 0; n < peopleCount; n++) {  // Traverse people array
                                                if(strcmp(people[n]->name, name) == 0) {
                                                    personFound = true;
                                                    for(int p = 0; p < people[n]->itemCount; p++) {  // Traverse a person structure's items
                                                        if(strcmp(people[n]->itemNames[p], itemName) == 0) {
                                                            itemFound = true;
                                                            if(people[n]->itemNumbers[p] != itemCount) {  // Item number does not match to condition
                                                                allConditionsTrue = false;
                                                                breakCompletely2 = true;
                                                                break;
                                                            }
                                                        }
                                                    }
                                                    if(itemFound == false) {
                                                        allConditionsTrue = false;
                                                        breakCompletely2 = true;
                                                    }
                                                    if(breakCompletely2) {
                                                        break;
                                                    }
                                                }
                                            }
                                            if(personFound == false) {
                                                allConditionsTrue = false;
                                                breakCompletely2 = true;
                                            }
                                            if(breakCompletely2) {
                                                break;
                                            }
                                        }
                                        if(breakCompletely2) {
                                            break;
                                        }
                                    }
                                }
                            }
                            if(breakCompletely2) {
                                break;
                            }
                        }
                        if(breakCompletely2) {
                            break;
                        }
                    }
                }

                if(allConditionsTrue) {  // All conditions are met, proceed with actions
                    for(int j = 0; j < atomicActionSentenceCount[i]; j++) {
                        int breakCompletely3 = false;
                        for(int k = 0; k < atomicActionSentenceWordCount[i][j]; k++) {
                            if(strcmp(bigSentencesActionPart[i][j][k], "go") == 0) {
                                char *location = (char *)malloc(strlen(bigSentencesActionPart[i][j][k + 2]) + 1);
                                strcpy(location, bigSentencesActionPart[i][j][k + 2]);
                                for(int l = 0; l < k; l+=2) {  // Traverse subjects in the atomic action sentence
                                    char *name = (char *)malloc(strlen(bigSentencesActionPart[i][j][l]) + 1);
                                    strcpy(name, bigSentencesActionPart[i][j][l]);
                                    int personFound = false;
                                    for(int m = 0; m < peopleCount; m++) {  // Traverse people array
                                        if(strcmp(people[m]->name, name) == 0) {
                                            personFound = true;
                                            people[m]->location = location;
                                            break;
                                        }
                                    }

                                    if(personFound == false) {  // This person does not exist, create it
                                        struct Person *person = (struct Person *)malloc(sizeof(struct Person));
                                        person->itemCount = 0;
                                        person->itemMaxCount = 1;
                                        person->itemNames = (char **)malloc((person->itemMaxCount) * sizeof(char *));
                                        person->itemNumbers = (int *)malloc((person->itemMaxCount) * sizeof(int));
                                        person->name = name;
                                        person->location = location;

                                        // Add person instance to the people array
                                        addPerson(&peopleCount, &people, person, &peopleMaxCount);
                                    }
                                }
                            }
                            else if(strcmp(bigSentencesActionPart[i][j][k], "buy") == 0) {
                                if(strcmp(bigSentencesActionPart[i][j][atomicActionSentenceWordCount[i][j] - 2], "from") == 0) {  // buy ... from
                                    int buyerCount = (k + 1) / 2;
                                    char *seller = (char *)malloc(strlen(bigSentencesActionPart[i][j][atomicActionSentenceWordCount[i][j] - 1]) + 1);
                                    strcpy(seller, bigSentencesActionPart[i][j][atomicActionSentenceWordCount[i][j] - 1]);

                                    int indexOfSeller = -1;  // Index of the seller in our people array

                                    // Check if the seller exists and has enough items for all buyers
                                    for(int l = k + 1; l < atomicActionSentenceWordCount[i][j] - 2; l+=3) {  // For each item to be bought from the seller
                                        int itemCount = atoi(bigSentencesActionPart[i][j][l]);
                                        char *itemName = (char *)malloc(strlen(bigSentencesActionPart[i][j][l + 1]) + 1);
                                        strcpy(itemName, bigSentencesActionPart[i][j][l + 1]);

                                        int sellerFound = false;
                                        for(int m = 0; m < peopleCount; m++) {  // Traverse people array to find the seller
                                            if(strcmp(people[m]->name, seller) == 0) {  // Seller exists
                                                sellerFound = true;
                                                indexOfSeller = m;
                                                int itemFound = false;
                                                for(int n = 0; n < people[m]->itemCount; n++) {  // Traverse items of the seller
                                                    if(strcmp(people[m]->itemNames[n], itemName) == 0) {
                                                        itemFound = true;
                                                        if(itemCount * buyerCount > people[m]->itemNumbers[n]) {  // If seller does not have enough items
                                                            breakCompletely3 = true;
                                                            break;
                                                        }
                                                    }

                                                    if(itemFound) {
                                                        break;
                                                    }
                                                }

                                                if(itemFound == false || breakCompletely3) {
                                                    breakCompletely3 = true;
                                                    break;
                                                }
                                            }

                                            if(sellerFound) {
                                                break;
                                            }
                                        }

                                        if(sellerFound == false || breakCompletely3) {
                                            breakCompletely3 = true;
                                            break;
                                        }
                                    }

                                    if(breakCompletely3) {
                                        break;
                                    }

                                    // Seller exists and has enough items for all of buyers
                                    for(int l = k + 1; l < atomicActionSentenceWordCount[i][j] - 2; l+=3) {  // For each item to be bought from the seller
                                        int itemCount = atoi(bigSentencesActionPart[i][j][l]);
                                        char *itemName = (char *)malloc(strlen(bigSentencesActionPart[i][j][l + 1]) + 1);
                                        strcpy(itemName, bigSentencesActionPart[i][j][l + 1]);

                                        // Decrement from seller
                                        for(int m = 0; m < people[indexOfSeller]->itemCount; m++) {
                                            if(strcmp(people[indexOfSeller]->itemNames[m], itemName) == 0) {
                                                people[indexOfSeller]->itemNumbers[m] -= itemCount * buyerCount;
                                                break;
                                            }
                                        }

                                        // Increment items of buyers
                                        for(int m = 0; m < k; m+=2) {  // For each buyer
                                            char *buyerName = (char *)malloc(strlen(bigSentencesActionPart[i][j][m]) + 1);
                                            strcpy(buyerName, bigSentencesActionPart[i][j][m]);

                                            // Iterate people array to find buyer structs
                                            int buyerFound = false;
                                            int indexOfBuyer = -1;
                                            for(int n = 0; n < peopleCount; n++) {  // Find the index of buyer if he exists
                                                if(strcmp(people[n]->name, buyerName) == 0) {
                                                    buyerFound = true;
                                                    indexOfBuyer = n;
                                                    break;
                                                }
                                            }

                                            if(buyerFound == false) {  // If buyer person does not exist, create it
                                                struct Person *person = (struct Person *)malloc(sizeof(struct Person));
                                                person->itemCount = 0;
                                                person->itemMaxCount = 1;
                                                person->itemNames = (char **)malloc((person->itemMaxCount) * sizeof(char *));
                                                person->itemNumbers = (int *)malloc((person->itemMaxCount) * sizeof(int));
                                                person->name = buyerName;
                                                person->location = "NOWHERE";

                                                // Add person instance to the people array
                                                addPerson(&peopleCount, &people, person, &peopleMaxCount);

                                                indexOfBuyer = peopleCount - 1;
                                            }

                                            // Add this item "itemCount" many times
                                            for(int p = 0; p < itemCount; p++) {
                                                addItem(&(people[indexOfBuyer]->itemCount), &(people[indexOfBuyer]->itemNames), itemName, &(people[indexOfBuyer]->itemMaxCount), &(people[indexOfBuyer]->itemNumbers));
                                            }
                                        }
                                    }
                                }
                                else {  // buy
                                    for(int l = k + 1; l < atomicActionSentenceWordCount[i][j]; l+=3) {  // For each item to be bought
                                        int itemCount = atoi(bigSentencesActionPart[i][j][l]);
                                        char *itemName = (char *)malloc(strlen(bigSentencesActionPart[i][j][l + 1]) + 1);
                                        strcpy(itemName, bigSentencesActionPart[i][j][l + 1]);

                                        // Increment items of buyers
                                        for(int m = 0; m < k; m+=2) {  // For each buyer
                                            char *buyerName = (char *)malloc(strlen(bigSentencesActionPart[i][j][m]) + 1);
                                            strcpy(buyerName, bigSentencesActionPart[i][j][m]);

                                            // Iterate people array to find buyer structs
                                            int buyerFound = false;
                                            int indexOfBuyer = -1;
                                            for(int n = 0; n < peopleCount; n++) {  // Find the index of buyer if he exists
                                                if(strcmp(people[n]->name, buyerName) == 0) {
                                                    buyerFound = true;
                                                    indexOfBuyer = n;
                                                    break;
                                                }
                                            }

                                            if(buyerFound == false) {  // If buyer person does not exist, create it
                                                struct Person *person = (struct Person *)malloc(sizeof(struct Person));
                                                person->itemCount = 0;
                                                person->itemMaxCount = 1;
                                                person->itemNames = (char **)malloc((person->itemMaxCount) * sizeof(char *));
                                                person->itemNumbers = (int *)malloc((person->itemMaxCount) * sizeof(int));
                                                person->name = buyerName;
                                                person->location = "NOWHERE";

                                                // Add person instance to the people array
                                                addPerson(&peopleCount, &people, person, &peopleMaxCount);

                                                indexOfBuyer = peopleCount - 1;
                                            }

                                            // Add this item "itemCount" many times
                                            for(int p = 0; p < itemCount; p++) {
                                                addItem(&(people[indexOfBuyer]->itemCount), &(people[indexOfBuyer]->itemNames), itemName, &(people[indexOfBuyer]->itemMaxCount), &(people[indexOfBuyer]->itemNumbers));
                                            }
                                        }
                                    }
                                }
                            }
                            else if(strcmp(bigSentencesActionPart[i][j][k], "sell") == 0) {
                                if(strcmp(bigSentencesActionPart[i][j][atomicActionSentenceWordCount[i][j] - 2], "to") == 0) {  // sell ... to
                                    int sellerCount = (k + 1) / 2;
                                    char *buyerName = (char *)malloc(strlen(bigSentencesActionPart[i][j][atomicActionSentenceWordCount[i][j] - 1]) + 1);
                                    strcpy(buyerName, bigSentencesActionPart[i][j][atomicActionSentenceWordCount[i][j] - 1]);

                                    // Check if the sellers exist and have enough items for the buyer
                                    for(int x = 0; x < k; x+=2) {  // Iterate sellers in the atomic action sentence
                                        for(int l = k + 1; l < atomicActionSentenceWordCount[i][j] - 2; l+=3) {  // For each item to be sold to the buyer
                                            int itemCount = atoi(bigSentencesActionPart[i][j][l]);
                                            char *itemName = (char *)malloc(strlen(bigSentencesActionPart[i][j][l + 1]) + 1);
                                            strcpy(itemName, bigSentencesActionPart[i][j][l + 1]);

                                            // Traverse people array to find the seller
                                            int sellerFound = false;
                                            for(int m = 0; m < peopleCount; m++) {
                                                if(strcmp(people[m]->name, bigSentencesActionPart[i][j][x]) == 0) {  // Seller exists
                                                    sellerFound = true;

                                                    // Traverse items of the seller to find the item we are looking for
                                                    int itemFound = false;
                                                    for(int n = 0; n < people[m]->itemCount; n++) {
                                                        if(strcmp(people[m]->itemNames[n], itemName) == 0) {
                                                            itemFound = true;

                                                            if(itemCount > people[m]->itemNumbers[n]) {  // If seller does not have enough items
                                                                breakCompletely3 = true;
                                                                break;
                                                            }
                                                        }

                                                        if(itemFound) {
                                                            break;
                                                        }
                                                    }

                                                    if(itemFound == false || breakCompletely3) {
                                                        breakCompletely3 = true;
                                                        break;
                                                    }
                                                }

                                                if(sellerFound) {
                                                    break;
                                                }
                                            }

                                            if(sellerFound == false || breakCompletely3) {
                                                breakCompletely3 = true;
                                                break;
                                            }
                                        }

                                        if(breakCompletely3) {
                                            break;
                                        }
                                    }

                                    if(breakCompletely3) {
                                        break;
                                    }

                                    // Sellers exist and have enough items for the buyer
                                    for(int l = k + 1; l < atomicActionSentenceWordCount[i][j] - 2; l+=3) {  // For each item to be sold to the buyer
                                        int itemCount = atoi(bigSentencesActionPart[i][j][l]);
                                        char *itemName = (char *)malloc(strlen(bigSentencesActionPart[i][j][l + 1]) + 1);
                                        strcpy(itemName, bigSentencesActionPart[i][j][l + 1]);

                                        // Decrement from sellers
                                        for(int x = 0; x < k; x+=2) {
                                            char *sellerName = (char *)malloc(strlen(bigSentencesActionPart[i][j][x]) + 1);
                                            strcpy(sellerName, bigSentencesActionPart[i][j][x]);

                                            for(int m = 0; m < peopleCount; m++) {
                                                if(strcmp(people[m]->name, sellerName) == 0) {
                                                    for(int n = 0; n < people[m]->itemCount; n++) {
                                                        if(strcmp(people[m]->itemNames[n], itemName) == 0) {
                                                            people[m]->itemNumbers[n] -= itemCount;
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                        }

                                        // Increment items of the buyer
                                        int buyerFound = false;
                                        int indexOfBuyer = -1;
                                        for(int n = 0; n < peopleCount; n++) {  // Find the index of buyer if he exists
                                            if(strcmp(people[n]->name, buyerName) == 0) {
                                                buyerFound = true;
                                                indexOfBuyer = n;
                                                break;
                                            }
                                        }

                                        if(buyerFound == false) {  // If buyer person does not exist, create it
                                            struct Person *person = (struct Person *)malloc(sizeof(struct Person));
                                            person->itemCount = 0;
                                            person->itemMaxCount = 1;
                                            person->itemNames = (char **)malloc((person->itemMaxCount) * sizeof(char *));
                                            person->itemNumbers = (int *)malloc((person->itemMaxCount) * sizeof(int));
                                            person->name = buyerName;
                                            person->location = "NOWHERE";

                                            // Add person instance to the people array
                                            addPerson(&peopleCount, &people, person, &peopleMaxCount);

                                            indexOfBuyer = peopleCount - 1;
                                        }

                                        // Add this item "itemCount" many times
                                        for(int p = 0; p < itemCount * sellerCount; p++) {
                                            addItem(&(people[indexOfBuyer]->itemCount), &(people[indexOfBuyer]->itemNames), itemName, &(people[indexOfBuyer]->itemMaxCount), &(people[indexOfBuyer]->itemNumbers));
                                        }
                                    }
                                }
                                else {  // sell
                                    // Check if the sellers exist and have enough items for the buyer
                                    for(int x = 0; x < k; x+=2) {  // Iterate sellers in the atomic action sentence
                                        for(int l = k + 1; l < atomicActionSentenceWordCount[i][j]; l+=3) {  // For each item to be sold
                                            int itemCount = atoi(bigSentencesActionPart[i][j][l]);
                                            char *itemName = (char *)malloc(strlen(bigSentencesActionPart[i][j][l + 1]) + 1);
                                            strcpy(itemName, bigSentencesActionPart[i][j][l + 1]);

                                            // Traverse people array to find the seller
                                            int sellerFound = false;
                                            for(int m = 0; m < peopleCount; m++) {
                                                if(strcmp(people[m]->name, bigSentencesActionPart[i][j][x]) == 0) {  // Seller exists
                                                    sellerFound = true;

                                                    // Traverse items of the seller to find the item we are looking for
                                                    int itemFound = false;
                                                    for(int n = 0; n < people[m]->itemCount; n++) {
                                                        if(strcmp(people[m]->itemNames[n], itemName) == 0) {
                                                            itemFound = true;

                                                            if(itemCount > people[m]->itemNumbers[n]) {  // If seller does not have enough items
                                                                breakCompletely3 = true;
                                                                break;
                                                            }
                                                        }

                                                        if(itemFound) {
                                                            break;
                                                        }
                                                    }

                                                    if(itemFound == false || breakCompletely3) {
                                                        breakCompletely3 = true;
                                                        break;
                                                    }
                                                }

                                                if(sellerFound) {
                                                    break;
                                                }
                                            }

                                            if(sellerFound == false || breakCompletely3) {
                                                breakCompletely3 = true;
                                                break;
                                            }
                                        }

                                        if(breakCompletely3) {
                                            break;
                                        }
                                    }

                                    if(breakCompletely3) {
                                        break;
                                    }

                                    // Sellers exist and have enough items to sell
                                    for(int l = k + 1; l < atomicActionSentenceWordCount[i][j]; l+=3) {  // For each item to be sold
                                        int itemCount = atoi(bigSentencesActionPart[i][j][l]);
                                        char *itemName = (char *)malloc(strlen(bigSentencesActionPart[i][j][l + 1]) + 1);
                                        strcpy(itemName, bigSentencesActionPart[i][j][l + 1]);

                                        // Decrement from sellers
                                        for(int x = 0; x < k; x+=2) {
                                            char *sellerName = (char *)malloc(strlen(bigSentencesActionPart[i][j][x]) + 1);
                                            strcpy(sellerName, bigSentencesActionPart[i][j][x]);

                                            for(int m = 0; m < peopleCount; m++) {
                                                if(strcmp(people[m]->name, sellerName) == 0) {
                                                    for(int n = 0; n < people[m]->itemCount; n++) {
                                                        if(strcmp(people[m]->itemNames[n], itemName) == 0) {
                                                            people[m]->itemNumbers[n] -= itemCount;
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Free allocated memory
        for(int i = 0; i < sentencesCount; i++) {
            free(sentences[i]);
        }
        free(sentences);

        free(wordCountInEachSentence);
        free(ifIndexArray);

        for(int i = 0; i < bigSentencesActionPartCount; i++) {
            for(int j = 0; j < atomicActionSentenceCount[i]; j++) {
                free(bigSentencesActionPart[i][j]);
            }
            free(bigSentencesActionPart[i]);
        }
        free(bigSentencesActionPart);

        for(int i = 0; i < bigSentencesConditionalPartCount; i++) {
            for(int j = 0; j < atomicConditionalSentenceCount[i]; j++) {
                free(bigSentencesConditionalPart[i][j]);
            }
            free(bigSentencesConditionalPart[i]);
        }
        free(bigSentencesConditionalPart);

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

void addAtomicSentence(int *sentencesCount, char ****atomicSentences, char **newAtomicSentence, int *sentencesMaxCount) {
    (*atomicSentences)[*sentencesCount] = newAtomicSentence;
    (*sentencesCount)++;

    if(*sentencesCount == *sentencesMaxCount) {  // Array is full, double its size
        *sentencesMaxCount *= 2;
        *atomicSentences = (char ***)realloc(*atomicSentences, (*sentencesMaxCount) * sizeof(char **));
    }
}

void addBigSentence(int *sentencesCount, char *****bigSentences, char ***newBigSentence, int *sentencesMaxCount) {
    (*bigSentences)[*sentencesCount] = newBigSentence;
    (*sentencesCount)++;

    if(*sentencesCount == *sentencesMaxCount) {  // Array is full, double its size
        *sentencesMaxCount *= 2;
        *bigSentences = (char ****)realloc(*bigSentences, (*sentencesMaxCount) * sizeof(char **));
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

void addInteger(int *integerCount, int **integers, int integer, int *integersMaxSize) {
    (*integers)[*integerCount] = integer;
    (*integerCount)++;

    if(*integerCount == *integersMaxSize) {  // Array is full, double its size
        *integersMaxSize *= 2;
        *integers = (int *)realloc(*integers, (*integersMaxSize) * sizeof(int));
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

int isActionWord(char *word) {
    for(int i = 0; i < 3; i++) {
        if(strcmp(word, actions[i]) == 0) {
            return true;
        }
    }

    return false;
}

int isConditionWord(char *word) {
    for(int i = 0; i < 2; i++) {
        if(strcmp(word, conditions[i]) == 0) {
            return true;
        }
    }

    return false;
}