/*###################################################################
 *
 * bmp2c - A small program convert a 24bit true color Bitmap 
 *         file (Microsoft V3.0 format) into a C file.
 *         This tool is created for LeeKweed and ConLeeKweed
 *         because I want to hardcode the image into the program.
 *         Although for windows excutable, the data can be compile
 *         into the program as a resource. Using BMP2C is a more 
 *         portable way to hard-code image data into the program.
 *
 *
 *
 * Copyright (c) 2008-2009, Xiaoyong Guo
 *
 *
 * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 *
 *
 * mailto: guo.xiaoyong@gmail.com
 *         http://www.wordeazy.com
 *
 *##################################################################*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#include <tcimg.h>


/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

const char USAGE[] = "bmp2c <24bit BMP file> [Image Object Name] [updown or downup]";


const char RFError[]     =  "Read image file ERROR!!";
const char ARGError[]    =  "Please specify a 24bit true-color BMP (MSV3) file name";
const char NAMETOOLONG[] =  "argv[2], maximal length of the name \
			    for the object exceeded!, \
			    use default name instead!!";

const char ARGV3ERROR[]  = "argv[3], unrecogonizable switch!";


/* maximal length for the input file name */
#define MXNL 256
const int MaxNameLen = MXNL;

/* the name of the image object, default is "Image" */
char IMGObjName[MXNL] = "Image";





/*###################################################################
 *
 * Main Function
 *
###################################################################*/

int main(int argc, char* argv[])
{
    int i, j, offset, info;
    TrueColorImage img;


    /* Bitmap is desinged to scan from down to up,
     * but in SDL, the data is store up to down,
     * need a switch to indicate the scan order,
     * default is downup*/
    enum ScanOrder {updown, downup};
    enum ScanOrder scanorder = downup;



    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     *
     * check the number of input arguments 
     *
     *>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    if (argc < 2)
    {
        printf("%s\n\n", ARGError);
        printf("%s\n", USAGE);
        return 1;
    }
   
    if (argc >= 3)
    {
        if ((int)strlen(argv[2]) < MaxNameLen)
        {
            strcpy(IMGObjName, argv[2]);
        }
        else
        {
            fprintf(stderr, "%s\n", NAMETOOLONG);
        }
    }
    
    if (argc == 4)
    {
        if ( !strcmp(argv[3], "downup") )
        {
            scanorder = downup;
        }
        else if ( !strcmp(argv[3], "updown") )
        {
            scanorder = updown;
        }
        else
        {
            fprintf(stderr, "%s\n", ARGV3ERROR);
        }
    }


    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     *
     * Read image data from file
     *
     >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    info = TCIMG_ReadFromFile(&img, argv[1]);
    if (info != 0)
    {
        printf("%s\n", RFError);
        return 2;
    }



    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     *
     * Print out the C code, usually need to be redirected
     * to a disk file
     *
     *>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    printf("#include <tcimg.h> \n\n\n");
    printf("UINT8 %s_ImageData[] = { \n", IMGObjName);

    /* output image data */
    for (j = 0; j < img.ImgHeight; ++j)
    {
        for (i = 0; i < img.ImgByteWidth; i+=3)
        {
            if (scanorder == downup)
            {
                offset = j * img.ImgByteWidth + i;
            }
            else
            {
                offset = (img.ImgHeight-1-j) * img.ImgByteWidth + i;
            }

            printf("%u, %u, %u, \n",
                   (UINT32)img.ImgData[offset+0],
                   (UINT32)img.ImgData[offset+1],
                   (UINT32)img.ImgData[offset+2]);
        }
    }

    printf("};\n\n\n");

    printf("TrueColorImage %s = {  \n", IMGObjName);

    printf( "%s_ImageData, \n", IMGObjName);

    printf( "%d, \n", img.ImgWidth);
    printf( "%d, \n", img.ImgHeight);
    printf( "%d, \n", img.ImgByteWidth);
    printf( "%d, \n", img.ImgTotalPixels);
    printf( "%d }; \n", img.ImgTotalBytes);



    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    printf("\n\n\n");
    printf("/* EOF */ \n");
    printf("\n\n\n");

    return EXIT_SUCCESS;
}



/* EOF */




