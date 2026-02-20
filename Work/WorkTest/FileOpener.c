#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fileOpen(const char *filename) {
    /* Open the file for reading */
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "File %s could not be opened.\n", filename);
    }
    else {
        fprintf(stderr, "File %s has been opened.\n", filename);
    };

    char fileType[10];
    fscanf(f,"%s", &fileType);

    printf("File Type: %s. \n",fileType);

    fclose(f);
    fprintf(stderr, "File %s has been closed.\n", filename);
};

int main() {

    fileOpen("coffee.hqhex");

};