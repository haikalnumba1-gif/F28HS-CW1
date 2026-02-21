#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A singular pixel within an image. */
typedef struct {
    //String of 4 hexadecimal characters + EOS of "/0".
    char red[6];
    char green[6];
    char blue[6];
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

/* Free an image from memory. */
void free_image(Image *img)
{
    //Free pixelArray.
    free(img->pixelArray);
    img->pixelArray = NULL;

    //Free img.
    free(img);
    img = NULL;
    printf("Memory freed.\n");
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
        fscanf(f,"%s",img->pixelArray[i].red);
        fscanf(f,"%s",img->pixelArray[i].green);
        fscanf(f,"%s",img->pixelArray[i].blue);
        
        //Progress indicator.
        if(i%(((img->width)*(img->height))/10) == 0 || i==(img->width)*(img->height)){
            printf("*");
        };
    };
    printf("\n");

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
        return NULL;
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
        fprintf(f,"%s ",img->pixelArray[i].red);
        fprintf(f,"%s ",img->pixelArray[i].green);
        fprintf(f,"%s ",img->pixelArray[i].blue);

        //Progress indicator.
        if(i%(((img->width)*(img->height))/10) == 0 || i==(img->width)*(img->height)){
            printf("*");
        };
    };
    printf("\n");

    fclose(f);
    return true;
}

/*For testing.*/
int main() {

    Image* img;

    img = load_image("bars.hqhex");
    if(img == NULL){
        printf("Image empty.\n");
    }
    else {
    printf("File Type: %s. \n",img->fileType);
    printf("File Width: %u. \n",img->width);
    printf("File Height: %u. \n",img->height);
    printf("File nValue: %u. \n",img->nvalues);
    printf("R: %s\n", img->pixelArray[0].red);
    printf("G: %s\n", img->pixelArray[0].green);
    printf("B: %s\n", img->pixelArray[0].blue);

    if(save_image(img, "dummy.hqhex")){
        printf("Image saved!\n");
    }
    else{
        printf("Image failed to save!\n");
    };

    };

    free_image(img);

};