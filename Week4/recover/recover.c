#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>   // Needed for defining uint8_t as BYTE and others.

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)  // If arguments not given correctly in command line.
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    char *raw_image = argv[1];

    FILE *inptr = fopen(raw_image, "r");
    if (inptr == NULL)  // If memory card can't be opened. 
    {
        fprintf(stderr, "Could not open %s\n", raw_image);
        return 1;
    }

    int file_count = 0, c;
    FILE *newfile = NULL;

    BYTE *image_buffer = malloc(512 * sizeof(BYTE));  // takeing 512 bytes at a time because FAT works like that.
    
    for (int block = 0; file_count < 50; block++)
    {
        
        fread(image_buffer, sizeof(BYTE), 512, inptr); 
        // So that it's more efficient to read start of each 512 byte block than reading byte after byte.
        
        
        /* 
        JPEG file starts with a signature 0xff d8 ff in the first 3 bytes 
        and 0xe or 1110 in the first 4 bit of the 4th byte.
        Also they end with the signature 0xff d9.
        */
        if (image_buffer[0] == 0xff && image_buffer[1] == 0xd8 && image_buffer[2] == 0xff &&
            (image_buffer[3] == 0xe0 || image_buffer[3] == 0xe1 || image_buffer[3] == 0xe2 || image_buffer[3] == 0xe3 ||
             image_buffer[3] == 0xe4 || image_buffer[3] == 0xe5 || image_buffer[3] == 0xe6 || image_buffer[3] == 0xe7 ||
             image_buffer[3] == 0xe8 || image_buffer[3] == 0xe9 || image_buffer[3] == 0xea || image_buffer[3] == 0xeb ||
             image_buffer[3] == 0xec || image_buffer[3] == 0xed || image_buffer[3] == 0xee || image_buffer[3] == 0xef))
        {
            if (newfile != NULL)  // Closing an old file which is already open
            {
                fclose(newfile);
                file_count++;
            }

            char file_name[8];
            sprintf(file_name, "%03d.jpg", file_count);  // Used so that the file name can be increamented each time.

            newfile = fopen(file_name, "w");                  // New image file is opened
            fwrite(image_buffer, sizeof(BYTE), 512, newfile);
        }

        else if (file_count != 49 && newfile != NULL)
        {
            fwrite(image_buffer, sizeof(BYTE), 512, newfile);  // writing BLOCKS into newfile if signature is not encountered. 
        }
        
        // For last image
        if (file_count == 49)
        {
            while ((c = getc(inptr)) != EOF) // Reading byte after byte till EOF 
            {
                putc(c, newfile);  
            }
            if (feof(inptr))  // feof() works only if we cross the end of file, i.e., getc() returns EOF. 
            {
                fclose(newfile); // Closing last file.
                file_count++;
            }
        }
    }

    free(image_buffer); // Freeing memory ( use valgring ./recover for checking for leaks)
    fclose(inptr);   // Closing memory card.
}

