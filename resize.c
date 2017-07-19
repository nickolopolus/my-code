/**
 * Copies a BMP piece by piece, just because.
 * Adapted from copy.c 
 * 
 * TODO = Refactor this large vat of spaghetti. 
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // verify usage
    if(argc != 4){
        fprintf(stderr, "Usage: ./resize <factor> <infile> <outfile>\n");
        return 1;
    }
    
    // remember factor
    int f = atoi(argv[1]);
    
    // validate range of factor
    if(f < 1 || f > 100){
        fprintf(stderr, "Factor must be between 1 and 100.\n");
        return 2;
    }
    
    // remember filenames
	char* infile = argv[2];
	char* outfile = argv[3];
    
    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }
    
    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // new bitmap headers for resized file
    BITMAPFILEHEADER bf_rs = bf;
	BITMAPINFOHEADER bi_rs = bi;
	
	// resize width and height in new info header using factor
	bi_rs.biWidth = bi.biWidth * f;
	bi_rs.biHeight = bi.biHeight * f;
	
	// calculate old and new padding 
	int oldPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
	int newPadding = (4 - (bi_rs.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
	
	// calculate new biSizeImage and bfSize
	bi_rs.biSizeImage = (bi_rs.biWidth * sizeof(RGBTRIPLE) + newPadding) * abs(bi_rs.biHeight);
	bf_rs.bfSize = bi_rs.biSizeImage + 54;
	
	// write outfile's BITMAPFILEHEADER
	fwrite(&bf_rs, sizeof(BITMAPFILEHEADER), 1, outptr);

	// write outfile's BITMAPINFOHEADER
	fwrite(&bi_rs, sizeof(BITMAPINFOHEADER), 1, outptr);
	
	// for each line in old image
	for (int i = 0, oldHeight = abs(bi.biHeight); i < oldHeight; i++)
	{
	    // repeat writing of each line f (factor) times 
		for(int j = 0; j < f; j++)
		{
		    // iterate over pixels in scanline
			for (int k = 0, oldWidth = bi.biWidth; k < oldWidth; k++)
			{
			    // temporary storage
				RGBTRIPLE triple;

				// read RGB triple from infile
				fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

				// write RGB triple to outfile f (factor) times
				for(int l = 0; l < f; l++)
				{
				    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
				}
			}
			
			// skip over padding in infile
			fseek(inptr, oldPadding, SEEK_CUR);

			// add padding to outfile
			for (int m = 0; m < newPadding; m++)
			{
			    fputc(0x00, outptr);    
			}
            
            // return file position to beginning of scanline
			fseek(inptr, -1 * (bi.biWidth * 3 + oldPadding ), SEEK_CUR);
		}
		
		//move file position back to where it naturally would have been without fseek above
		fseek(inptr, bi.biWidth * 3 + oldPadding, SEEK_CUR);
	}
    
    // close infile
	fclose(inptr);

	// close outfile
	fclose(outptr);
    
    // success
    return 0;
}
