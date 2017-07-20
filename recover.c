/**
 * recover.c
 *
 * Recovers .jpg images from a raw file
 *
 * Usage: ./recover <filename.raw>
 *
 * Will save .jpg's to current directory
 *
 * Not sure why check50 returns 'failed to recover middle and last image'.
 * All 50 images in card.raw are written to the current directory. They
 * all open and look fine. All 50 .jpg's are also evenly divisible by
 * 512 (size of file buffer for FAT)
 */

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;

int main(int argc, char* argv[])
{
    // verify usage
    if(argc != 2){
        fprintf(stderr, "Usage: ./recover <recovery filename>\n");
        return 1;
    }
    
    // open file
    FILE* file = fopen(argv[1], "r");
    if(file == NULL){
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    // get number of bytes in file
    fseek(file, 0, SEEK_END);
    int numBytes = ftell(file);
    fseek(file, 0, SEEK_SET);
    //fprintf(stdout, "Size of %s: %d bytes\n", argv[1], numBytes);
    
    // read file contents into buffer
    BYTE* buffer = (BYTE*) malloc((numBytes) * sizeof(char) + sizeof(char));
    fread(buffer, numBytes, 1, file);
    
    // buffer and counter for filename
    char filename[10];
    int fileNum = 0;
    
    //iterate through file buffer, byte by byte
    for(int i = 0; i < numBytes - 4; i++)
    {
        //detect jpg signature
        if((buffer[i] == 0xff) && (buffer[i + 1] == 0xd8) && buffer[i + 2] == 0xff && (buffer[i + 3] >= 0xe0 && buffer[i + 3] <= 0xef)){
            
            // once signature detected, detect next signature or EOF
            for(int j = i + 3; j < numBytes - 4; j++)
            {
                if(((buffer[j] == 0xff) && (buffer[j + 1] == 0xd8) && (buffer[j + 2] == 0xff) && (buffer[j + 3] >= 0xe0 && buffer[j + 3] <= 0xef)) || (j == (numBytes - 5))){
                    
                    // if last jpg, set j to last byte
                    if(j == (numBytes - 5)){
                        j = numBytes;
                    }
                    
                    // once 2nd signature detected, write to file
                    sprintf(filename, "%03d.jpg", fileNum++);
                    FILE* jpgfile = fopen(filename, "wb");
                    fwrite(&buffer[i], j - i, 1, jpgfile);
                    fclose(jpgfile);
                    i = (j - 1);
                    break;
                }
            }
        }
    }
    
    // free buffer memory previously allocated
    free(buffer);
    
    // close file
    fclose(file);
    
    return 0;
}