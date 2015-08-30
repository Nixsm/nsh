#include "makeargs.h"
#include <stdlib.h>
#include <string.h>

int makeArgs(IN char* inputString, IN char* delimiter, OUT char*** strings) {
    int error;
    int i;
    char* t;
    const char* snew;
    int numOfTokens;

    
    if((!inputString) || (!delimiter) || (!strings)) {
        return -1;
    }

    *strings = NULL;

    snew = inputString + strspn(inputString, delimiter);
    if (!(t = malloc(strlen(snew) + 1))) {
        return -1;
    }

    strcpy(t, snew);

    numOfTokens = 0;

    if (strtok(t, delimiter) != NULL) {
        for (numOfTokens = 1; strtok(NULL, delimiter) != NULL; ++numOfTokens);
    }

    if (!(*strings = malloc((numOfTokens + 1) * sizeof(char *)))) {
        free(t);
        return -1;
    }

    if (numOfTokens == 0) {
        free(t);
    } else {
        strcpy(t, snew);
        **strings = strtok(t, delimiter);
        for (i = 1; i < numOfTokens; ++i) {
            *((*strings) + i) = strtok(NULL, delimiter);
        }
    }

    *((*strings) + numOfTokens) = NULL;

    return numOfTokens;

}
