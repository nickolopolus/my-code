/**
 * Copies a BMP piece by piece, just because.
 * Taken and adapted from cs50x's copy.c for pset4
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // if the user did not pass 2 arguments return 1
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./whodunit infile outfile\n");
        return 1;
    }
    
    // store filenames passed by user
    char *infile = argv[1];
    char *outfile = argv[2];

    // create pointer to FILE object from input filename, return 2 if error
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // create pointer to FILE object from output filename, return 3 if error
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }
    
    // read in BITMAPFILEHEADER from input file
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read in BITMAPINFOHEADER from input file
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // verify file is 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write BITMAPFILEHEADER to output file
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write BITMAPINFOHEADER to output file
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels within scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // change fully red pixels to white
            if (triple.rgbtGreen != 0xff) {
                triple.rgbtGreen = 0xff;
            }

            if(triple.rgbtBlue != 0xff){
                triple.rgbt.BLue = 0xff;
            }

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }
        
        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close input file
    fclose(inptr);

    // close output file
    fclose(outptr);

    return 0;

}
  