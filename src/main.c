#include "markov.h"
#include "matrix.h"
#include "list.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

List computeDictionary(char* filename) {
    FILE *file = fopen(filename, "r");
    
    List list = makeList();

    //Return empty dictionary if the file is not found
    if(!file) {
        return list;
    }

    char buff[1024];
    while(fgets(buff, 1024, file) != NULL) {

        int i = 0;
        while(buff[i] != '\n' && buff[i] != '\0') i++;
        buff[i] = '\0';

        //Add the word if it was not found
        if(listIndexOfStr(list, buff) < 0) {
            addToList(list, strdup(buff));
        }

    }

    fclose(file);

    return list;
}

List computeLinks(char* filename, List dict) {
    FILE *file = fopen(filename, "r");
    
    List list = makeList();

    if(!file)
        return list;

    char buff[64];
    while(fgets(buff, 64, file)) {
        int i = atoi(buff);
        
        fgets(buff, 64, file);
        int j = atoi(buff);

        fgets(buff, 64, file);
        int value = atoi(buff);
        
        addToList(list, makeNodeLink(
                (char*) getFromList(dict, i),
                (char*) getFromList(dict, j),
                value));

    }

    fclose(file);

    return list;
}

int genNextIdx(Matrix H, int i) {
    int result;
    Matrix col = getColVector(H, i);
    if(vecNorm(col) == 0) {
        result = -1;
    } else {
        double x = (rand() * 1.0) / RAND_MAX;
        double sum = 0;
        int idx = 0;
        while(sum < x && idx < H->ROWS) {
            sum += getMtrxVal(H, idx, i);
            if(sum > x) break;
            idx++;
        }

        result = idx == H->ROWS ? -1 : idx;
    }

    freeMatrix(col);
    return result;
}

void generateMarkovString(char* argv[]) {

    srand(time(NULL));
    
    int length = atoi(argv[2]);
    List dict = computeDictionary(argv[0]);
    List links = computeLinks(argv[1], dict);

    Matrix A = genGraphMatrix(dict, links);
    Matrix H = makeMarkovMatrix(A);
    
    int i = 0;
    int nextChar = rand() % listSize(dict);
    while(i < length && nextChar >= 0) {
        printf("%s ", (char*) getFromList(dict, nextChar));
        nextChar = genNextIdx(H, nextChar);
        i++;
    }
    printf("\n");
}

void updateLinks(List dict, List links, char* filename) {

    //Create the matrix to add to the file
    Matrix A = genGraphMatrix(dict, links);

    //Write the values to the file
    int size = listSize(dict);
    
    FILE* file = fopen(filename, "w");

    printf("Dictionary size is %i\n", listSize(dict));

    int i = 0;
    while(i < size) {
        int j = 0;
        while(j < size) {
            int x = (int) getMtrxVal(A, i, j);
            if(x) {
                fprintf(file, "%i\n%i\n%i\n", i, j, x);
            }
            j++;
        }
        i++;
    }

    fclose(file);

}

void addMarkovElement(char* argv[]) {

    printf("Adding link (%s) -> (%s)\n", argv[2], argv[3]);
    
    List dict = computeDictionary(argv[0]);
    List links = computeLinks(argv[1], dict);

    char* strFrom = argv[2];
    char* strTo = argv[3];
    
    FILE *file = fopen(argv[0], "a+");

    //Add each item to the dictionary if necessary
    if(listIndexOfStr(dict, strFrom) < 0) {
        addToList(dict, strdup(strFrom));
        printf("Adding %s to the dictionary\n", strFrom);
        fprintf(file, "%s\n", strFrom);
    }

    if(listIndexOfStr(dict, strTo) < 0) {
        addToList(dict, strdup(strTo));
        printf("Adding %s to the dictionary\n", strTo);
        fprintf(file, "%s\n", strTo);
    }

    fclose(file);

    //Add the link
    addToList(links, (void*) makeNodeLink(strFrom, strTo, 1));
    
    updateLinks(dict, links, argv[1]);
}

void addMarkovChain(char* argv[]) {
    
    List dict = computeDictionary(argv[0]);
    List links = computeLinks(argv[1], dict);

    FILE *file = fopen(argv[0], "a+");

    if(!argv[2] || !argv[3]) {
        printf("Insufficient nmber of arguments provided.\n");
    } else {
        int i = 3;
        while(argv[i] != NULL) {
            //Get two consecutive values
            char *strFrom = argv[i-1];
            char *strTo = argv[i];
            
            //Add the word(s) that are not known
            if(listIndexOfStr(dict, strFrom) < 0) {
                addToList(dict, strdup(strFrom));
                printf("Adding %s to the dictionary\n", strFrom);
                fprintf(file, "%s\n", strFrom);
            }

            if(listIndexOfStr(dict, strTo) < 0) {
                addToList(dict, strdup(strTo));
                printf("Adding %s to the dictionary\n", strTo);
                fprintf(file, "%s\n", strTo);
            }

            //Adds the particular link to the list
            addToList(links, (void*) makeNodeLink(strFrom, strTo, 1));

            i++;
        }
    }

    fclose(file);

    updateLinks(dict, links, argv[1]);

}

int main(int argc, char* argv[]) {
    
    if(argc == 1)
        printf("No arguments provided.\n");
    else if(!strcmp(argv[1], "gen")) {
        if(argc == 5)
            generateMarkovString(&argv[2]);
        else {
            printf("Invalid number of arguments\n");
            return 1;
        }
    } else if(!strcmp(argv[1], "link")) {
        if(argc == 6)
            addMarkovElement(&argv[2]);
        else {
            printf("Invalid number of arguments.\n");
            return 1;
        }
    } else if(!strcmp(argv[1], "chain")) {
        if(argc >= 6)
            addMarkovChain(&argv[2]);
        else {
            printf("Invalid number of arguments.\n");
            return 1;
        }
    } else {
        int result;
        if((result = strcmp(argv[1], "help")))
            printf("Invalid routine selected. The available routines are:\n");
        printf("gen [DICT] [LINKS] [LEN] - Generates a random markov chain.\n");
        printf("link [DICT] [LINKS] [SRC] [DST] - Adds a markov link.\n"); 
        printf("chain [DICT] [LINKS] [SRC] [DST] - Adds a markov chain.\n");

        return result ? 1 : 0;
    }

    return 0;
}
