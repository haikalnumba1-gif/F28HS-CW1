#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* [ Structure ] 
* A singular pixel within an image. */
typedef struct {
    //Stores hexadecimal code.
    unsigned red;
    unsigned green;
    unsigned blue;
} Pixel; 

/* [ Structure ]
* An image loaded from a file. */
typedef struct {
    //Max file name of 30 characters.
    char fileType[30];
    unsigned width;
    unsigned height;
    unsigned nvalues;
    Pixel* pixelArray;
} Image;

/*[ Helper Function ]
* Adds 2 hexadecimal numbers. 
* Caps max output: 0xffff.*/
static unsigned sumHexa(unsigned h1, unsigned h2){
    unsigned h3 = h1 + h2;
    if(h3 > 0xffff)
        h3 = 0xffff;
    return h3;
}

/*[ Helper Function ]
* Adds the RGB values of 2 pixels.*/
static Pixel sumPixel(Pixel p1, Pixel p2){
    Pixel p3;
    p3.red = sumHexa(p1.red,p2.red);
    p3.green = sumHexa(p1.green,p2.green);
    p3.blue = sumHexa(p1.blue,p2.blue);
    return p3;
}

/* [ Helper Function ]
* Finds the index in a contiguous 2D array.
* row: 0 to height - 1 
* col: 0 to width -1 
* totCol = width */
static unsigned seekIndex(unsigned row, unsigned col, unsigned totCol){
    return (row*totCol) + col;
}

/* [ Base Function ]
* Free an image from memory.
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

/* [ Base Function ] 
* Loads an image file to memory. 
* Success: Returns an image.
* Failure: Returns NULL.*/
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
    if(fscanf(f,"%s", img->fileType) == 0){
        printf("Failed header read: filetype.\n");
        return NULL;
    }
    if(fscanf(f,"%u", &img->width) == 0){
        printf("Failed header read: width.\n");
        return NULL;
    }
    if(fscanf(f,"%u", &img->height) == 0){
        printf("Failed header read: height.\n");
        return NULL;
    }
    if(fscanf(f,"%u", &img->nvalues) == 0){
        printf("Failed header read: nvalues.\n");
        return NULL;
    }

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
        if(fscanf(f,"%x",&img->pixelArray[i].red) == 0){
            printf("Failed red pixel read: %u.\n",i);
            return NULL;
        } 
        if(fscanf(f,"%x",&img->pixelArray[i].green) == 0){
            printf("Failed green pixel read: %u.\n",i);
            return NULL;
        }
        if(fscanf(f,"%x",&img->pixelArray[i].blue) == 0){
            printf("Failed blue pixel read: %u.\n",i);
            return NULL;
        }
        
        //Progress indicator.
        if(i%(((img->width)*(img->height))/10) == 0 || i==(img->width)*(img->height)){
            printf("*");
            fflush(stdout);
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

/* [ Base Function ]
* Write img to file: filename. 
* Success: Returns true.
* Failure: Returns false. */
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
            fflush(stdout);
        };
    };
    printf("\nSaving Complete.\n");
    printf("Saved to: %s.\n",filename);

    fclose(f);
    return true;
}

/* [ Base Function ] 
* Deep copy of an image. 
* Success: Returns copy of an image.
* Failure: Returns NULL. */
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

/* [ Feature Function ]
* Vertically reflects an image onto itself.
* Success: Returns new edited image.
* Failure: Returns NULL. */
Image *apply_reflect(Image *source){

    if(source == NULL){//Err Msg...
        printf("Image is null.\n");
        return NULL;
    };
    
    Image* newImg = copy_image(source);

    unsigned buffIndex;//Current index.
    unsigned buffVerIndex;//Vertically opposite index.
    
    Pixel buffPix;
    
    unsigned h = source->height;
    unsigned w = source->width;

    unsigned progCounter = 0;

    printf("Reflecting Progress: ");
    for(unsigned i=0 ; i < (h-1)/2 ; i++){//Row searcher
        for(unsigned j=0 ; j < (w-1) ; j++){//Column searcher

            //Progress indicator.
            progCounter++;
            if(progCounter%(((w*h))/20) == 0 || i==(w*h)){
            printf("*");
            fflush(stdout);
            };

            //Find indexes.
            buffIndex = seekIndex(i,j,w);
            buffVerIndex = seekIndex(h-1-i,j,w);

            //Add pixels.
            buffPix = sumPixel(source->pixelArray[buffIndex],source->pixelArray[buffVerIndex]);
            
            //Override pixels.
            newImg->pixelArray[buffIndex] = buffPix;
            newImg->pixelArray[buffVerIndex] = buffPix;
        }
    }
    printf("\nReflecting Complete.\n");

    return newImg;

}

/* [ Feature Function ]
* Prints out all used RGB values.
* Success: Returns true.
* Failure: Returns false. */
bool apply_hist(Image *source){

    if(source == NULL){//Err Msg...
        printf("Image is null.\n");
        return false;    
    };

    //64 possible combinations of 4 hexadecimal digits.
    unsigned arrRed[0xffff] = {0};
    unsigned arrGreen[0xffff] = {0};
    unsigned arrBlue[0xffff] = {0};

    Pixel buffPix;
    
    unsigned h = source->height;
    unsigned w = source->width;

    //Searching...
    printf("Searching Progress:");
    for(unsigned i=0 ; i < (w*h)-1 ; i++){//Pixel searcher

        //Progress indicator.
        if(i%((w*h)/10) == 0 || i==(w*h)){
            printf("*");
            fflush(stdout);
        };

        buffPix = source->pixelArray[i];

        for(unsigned a=0x0 ; a <= 0xffff ; a++){//Red checker.
            if(a == buffPix.red){
                arrRed[a]++;
                break;
            }
        }

        for(unsigned a=0x0 ; a <= 0xffff ; a++){//Green checker.
            if(a == buffPix.green){
                arrGreen[a]++;
                break;
            }
        }

        for(unsigned a=0x0 ; a <= 0xffff ; a++){//Blue checker.
            if(a == buffPix.blue){
                arrBlue[a]++;
                break;
            }
        }
    }
    printf("\nSearch Complete.\n");

    //Printing...
    for(unsigned i=0x0 ; i < 0xffff ; i++){//Red printer.
        if(arrRed[i] != 0){
            printf("Red Value %x: %u pixels\n",i,arrRed[i]);
        }
    }

    for(unsigned i=0x0 ; i < 0xffff ; i++){//Green printer.
        if(arrGreen[i] != 0){
            printf("Green Value %x: %u pixels\n",i,arrGreen[i]);
        }
    }

    for(unsigned i=0x0 ; i < 0xffff ; i++){//Blue printer.
        if(arrBlue[i] != 0){
            printf("Blue Value %x: %u pixels\n",i,arrBlue[i]);
        }
    }

    return true;
}

/* [ Tester Function ] 
* Prints image headers and first pixel.*/
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

/*Execution.*/
int main(int argc, char *argv[]) {
        
    // Check command-line arguments
    if (argc != 3) {//Err Msg...
        fprintf(stderr, "Usage: process INPUTFILE OUTPUTFILE\n");
        return 1;
    }

    // Load the input image
    Image *in_img = load_image(argv[1]);
    if (in_img == NULL) {//Err Msg...
        return 1;
    }

    // Apply the first process to the input image 
    Image *out_img = apply_reflect(in_img);
    if (out_img == NULL) {//Err Msg...
        fprintf(stderr, "First process failed.\n");
        free_image(&in_img);
        return 1;
    }

    // Apply the second process to the output of the first process 
    if (!apply_hist(out_img)) {//Err Msg...
        fprintf(stderr, "Second process failed.\n");
        free_image(&in_img);
        free_image(&out_img);
        return 1;
    }

    // Save the output image.
    if (!save_image(out_img, argv[2])) {//Err Msg...
        fprintf(stderr, "Saving image to %s failed.\n", argv[2]);
        free_image(&in_img);
        free_image(&out_img);
        return 1;
    }

    free_image(&in_img);
    free_image(&out_img);
    return 0;
}