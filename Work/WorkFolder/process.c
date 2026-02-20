#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A singular pixel within an image. */
typedef struct {
    char red[4];
    char green[4];
    char blue[4];
} Pixel; 

/* An image loaded from a file. */
typedef struct {
    char fileType[10];
    unsigned width;
    unsigned height;
    unsigned nvalues;
    Pixel* pixelArray;
} Image;

/* Free an image from memory. */
void free_image(Image *img)
{
    free(img);
    img = NULL;
    printf("Memory freed.\n");
}

/* Loads an image file to memory. Return NULL on failure.*/
Image* load_image(const char* filename)
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
        fscanf(f,"%s",img->pixelArray[i].blue);
        fscanf(f,"%s",img->pixelArray[i].green);
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
    };

    free_image(img);

};