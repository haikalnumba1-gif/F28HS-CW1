#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A singular pixel within an image. */
typedef struct {
    //Stores hexadecimal code.
    unsigned red;
    unsigned green;
    unsigned blue;
} Pixel; 

/* An image loaded from a file. */
typedef struct {
    //Max file name of 30 characters.
    char fileType[30];
    unsigned width;
    unsigned height;
    unsigned nvalues;
    Pixel* pixelArray;
} Image;

/* Free an image from memory. *
* Pass the image as an address. E.g: &img */
void free_image(Image **img)
{
    if(*img == NULL)
        printf("No memory to be freed.\n");
    else{
        //Free img.
        free(*img);
        *img = NULL;
        printf("Memory freed.\n");
    }
}

/* Loads an image file to memory. Return NULL on failure.*/
Image* load_image(char* filename)
{
    //Open image file. Read only.
    FILE *f = fopen(filename, "r");
    if (f == NULL) { //Err Msg.
        fprintf(stderr, "File %s could not be opened.\n", filename);
        perror("Error");
        return NULL;
    };

    //Create image object.
    Image* img = malloc(sizeof(Image));

    //Reading file headers.
    fscanf(f,"%s", img->fileType);
    fscanf(f,"%u", &img->width);
    fscanf(f,"%u", &img->height);
    fscanf(f,"%u", &img->nvalues);

    //Checking file header validity.
    if (strcmp(img->fileType,"HQHEX")!=0){//Err Msg.
        fprintf(stderr,"File type is not HQHEX.");
        fclose(f);
        return NULL;
    };
    if (img->nvalues!=3) {//Err Msg.
        fprintf(stderr,"Image nvalues not equal to 3.");
        fclose(f);
        return NULL;
    };

    //Allocating memory for array of pixels.
    img->pixelArray = malloc((img->width)*(img->height)*sizeof(Pixel));

    //Read pixel values from file.
    printf("Reading Progress: ");
    for(unsigned i=0; i<((img->width)*(img->height)) ; i++)
    {
        fscanf(f,"%x",&img->pixelArray[i].red);
        fscanf(f,"%x",&img->pixelArray[i].green);
        fscanf(f,"%x",&img->pixelArray[i].blue);
        
        //Progress indicator.
        if(i%(((img->width)*(img->height))/10) == 0 || i==(img->width)*(img->height)){
            printf("*");
        };
    };
    printf("\nReading Complete.\n");

    //Close the file.
    fclose(f);

    //Return.
    if (img == NULL) {//Err Msg.
        fprintf(stderr, "File %s could not be read.\n", filename);
        perror("Error");
        return NULL;
    }
    return img;
}

/* Write img to file filename. Return true on success, false on error. */
bool save_image(Image *img, char *filename)
{
    //Open image file. Write only, will override files.
    FILE *f = fopen(filename, "w");
    if (f == NULL) { //Err Msg.
        fprintf(stderr, "File %s could not be opened.\n", filename);
        perror("Error");
        return false;
    };

    //Write file headers.
    fprintf(f,"%s ",img->fileType);
    fprintf(f,"%u ", img->width);
    fprintf(f,"%u ", img->height);
    fprintf(f,"%u ", img->nvalues);

    //Write pixel values.
    printf("Saving Progress: ");
    for(unsigned i=0; i<((img->width)*(img->height)) ; i++)
    {
        fprintf(f,"%x ",img->pixelArray[i].red);
        fprintf(f,"%x ",img->pixelArray[i].green);
        fprintf(f,"%x ",img->pixelArray[i].blue);

        //Progress indicator.
        if(i%(((img->width)*(img->height))/10) == 0 || i==(img->width)*(img->height)){
            printf("*");
        };
    };
    printf("\nSaving Complete.\n");
    printf("Saved to: %s.\n",filename);

    fclose(f);
    return true;
}

/* Deep copy of an image. On error, returns NULL. */
Image *copy_image(Image *source)
{
    if (source == NULL) {//Err Msg.
        fprintf(stderr, "Source is empty/not loaded. \n");
        return NULL;
    }

    //Allocate memory for copy.
    Image* imgCopy = malloc(sizeof(Image));
    *imgCopy = *source;
    return imgCopy;
}




/*Adds 2 hexadecimal numbers. Caps max output: 0xffff.*/
unsigned sumHexa(unsigned h1, unsigned h2){
    unsigned h3 = h1 + h2;
    if(h3 > 0xffff)
        h3 = 0xffff;
    return h3;
}

/*Finds the 1D array index 2D indexes.
* row: 0 to height - 1 
* col: 0 to width -1 
* totCol = width */
unsigned seekIndex(unsigned row, unsigned col, unsigned totCol){
    return (row*totCol) + col;
}

/*Tester function*/
void printImgDetails(Image* img)
{
    printf("File Type: %s. \n",img->fileType);
    printf("File Width: %u. \n",img->width);
    printf("File Height: %u. \n",img->height);
    printf("File nValue: %u. \n",img->nvalues);
    printf("R: %x\n", img->pixelArray[0].red);
    printf("G: %x\n", img->pixelArray[0].green);
    printf("B: %x\n", img->pixelArray[0].blue);
}

/*For testing.*/
int main() {

    Image* img = NULL;
    Image* img2 = NULL;
    unsigned buff1;
    unsigned buff2;

    //Test loadimg.
    img = load_image("bars.hqhex");
    if(img == NULL)
        printf("Image empty.\n");
    else {
        img2 = copy_image(img);
        printImgDetails(img2);
    };

    free_image(&img);
    free_image(&img2);

};