// recover
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // check if user entered only one command line argument
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
    }
    
    // check if pointer to input file points to null
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        printf("File not found\n");
        return 1;
    }
    
    // initialize a buffer
    BYTE image[512];
    
    // check for the first jpeg header and read into the buffer
    while (fread(image, (512 * sizeof(BYTE)), 1, inptr))
    {
        if (image[0] == 0xff && image[1] == 0xd8 && image[2] == 0xff && ((image[3] & 0xf0) == 0xe0))
        {
            break;
        }
    }
    
    // write the contents of the buffer into an output file
    char filename[8];
    sprintf(filename, "%03i.jpg", 0);
    FILE *outptr = fopen(filename, "w");
    if (outptr == NULL)
    {
        printf("Error\n");
        return 1;
    }
    
    fwrite(image, (512 * sizeof(BYTE)), 1, outptr);
    int i = 0;
    
    while (fread(image, (512 * sizeof(BYTE)), 1, inptr))
    {
        // open a new file when a jpeg header is detected
        if (image[0] == 0xff && image[1] == 0xd8 && image[2] == 0xff && ((image[3] & 0xf0) == 0xe0))
        {
            fclose(outptr);
            i++;
            
            sprintf(filename, "%03i.jpg", i);
            outptr = fopen(filename, "w");
            if (outptr == NULL)
            {
                printf("Error\n");
                return 1;
            }
            
            fwrite(image, (512 * sizeof(BYTE)), 1, outptr);
        }
        // keep writing to the old file until jpeg header is found againn
        else
        {
            fwrite(image, (512 * sizeof(BYTE)), 1, outptr);
        }
    }
    
    fclose(inptr);
    fclose(outptr);
    return 0;
    
}
