/*###################################################################
 *
 * tcimg.h tcimg.c - tcimg stands for True Color Image. This library 
 *                   provides a set of routines to manipulating 24bit 
 *                   Bitmap files (widely used Microsoft V3.0). This 
 *                   library should work on both small endian machine
 *                   and big endian machine.
 *
 * Copyright (c) 2002-2009, Xiaoyong Guo
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
 *
 *##################################################################*/


/*#################################################################
 *
 * Created : Aug, 12, 2002
 *
 * Modified: Mar, 01, 2006
 * Modified: Apr, 15, 2009
 * 
 * Modified: Jan, 12, 2009
 *        1. add support for big endianness
 *        2. fix a bug of setting proper 
 *           bytewidth (also called pitch)
 *
 *
 * TODO: add more basic drawing functions
 *
 *#################################################################*/


#ifndef   __MSDIBV3_H__
#define   __MSDIBV3_H__


#include<stdio.h>
#include<math.h>


/* basic data type definition */
typedef int              INT32;
typedef unsigned int     UINT32;
typedef unsigned short   UINT16;
typedef unsigned char    UINT8 ;


/*###################################################################
 *
 * BMP File Header:
 *       Stores general information about the BMP file.
 *
 * Bitmap Information (DIB header):
 *         Stores detailed information about the bitmap image.
 *
 * Color Palette:
 *        Stores the definition of the colors being
 *	 used for indexed color bitmaps.
 *
 * Bitmap Data:
 *         Stores the actual image, pixel by pixel.
 *
 *##################################################################*/




/*####################################################################
 *
 *  MSDIB_FileHeader: the size is 14 bytes. the struct in memory
 *                    may take up 16 bytes if it is not packed
 *                    To write this struct into file, it is better
 *                    to write one field by one field. The order
 *                    of the fields are important.
 *
 *####################################################################*/

typedef
struct stru_MSDIB_FileHeader
{
    /* bfType = {'B' = 0x42 = 66, 'M' = 0x4D = 77} = 0x4D42 = 19778
     * bfType = [bfType1, bfType2] */
    UINT16  bfType;

    /* the size of the file */
    UINT32  bfSize;

    /* actual value depend on the appplication created them,
     * usually set to zero bfReserved = [bfReserved1, bfReserved2] */
    UINT32   bfReserved;

    /* the starting address of the bitmap data */
    UINT32  bfOffBits;
}
MSDIB_FileHeader;


/*####################################################################
 *
 * MSDIB_InfoHeader_V3: the size is 40 bytes. Like BITMAPFILEHEADER,
 *                      to write this struct to file, it is better to
 *                      write one field by one field, not to write
 *                      them as a whole. The order of the fields are
 *                      Important.
 *
 *
 * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 * biCompress Field:
 *
 * Value | Identified by  |  Compression method
 * 0       BI_RGB            none
 * 1       BI_RLE8RLE        8-bit/pixel
 * 2       BI_RLE4 RLE       4-bit/pixel
 * 3       BI_BITFIELDS      Bit field
 * 4       BI_JPEGJPEG 	     The bitmap contains a JPEG image
 * 5       BI_PNGPNG 	     The bitmap contains a PNG image
 *
 * Value | Comment
 * 0       Most common
 * 1       Can be used only with 8-bit/pixel bitmaps
 * 2       Can be used only with 4-bit/pixel bitmaps
 * 3       Can be used only with 16 and 32-bit/pixel bitmaps.
 * 4       The bitmap contains a JPEG image
 * 5       The bitmap contains a PNG image
 *
 * Also BI_JPG and BI_PNG are for printer drivers
 * and are not supported when rendering to the screen.
 *
 *####################################################################*/

typedef
struct stru_V3MSDIB_InfoHeader
{
    /* size of the BITMAPINFOHEADER, always 40 */
    UINT32 biSize;

    /* image width/height */
    INT32  biWidth;
    INT32  biHeight;

    /* the number of color planes being used. Must be set to 1 */
    UINT16  biPlanes;

    /* bits per pixel, typical values are 1, 4, 8, 16, 24, 32 */
    UINT16  biBitCount;

    /* compression method */
    UINT32 biCompression;

    /* the size of the raw bitmap data,
     * for BI_RGB image, it can be set to 0 */
    UINT32 biSizeImage;

    /* pixel per meter in herizontal vertical direction */
    INT32 biXPelsPerMeter;
    INT32 biYPelsPerMeter;

    /* the number of colors in the color palette,
     * or 0 to default to 2^n */
    UINT32 biClrUsed;

    /* the number of important colors that is used,
     * if all colors are important,
     * it should be set to 0. This field is usually
     * ignored */
    UINT32 biClrImportant;
}
V3MSDIB_InfoHeader;




/*###################################################################
 *
 * A DIB always uses the RGB color model. In this model,
 * a color is terms of different intensities (from 0 to 255)
 * of the additive primary colors red (R), green (G),
 * and blue (B). A color is thus defined using the 3 values
 * for R, G and B (though stored in backwards order in
 * each palette entry).
 *
 * The number of entries in the palette is either 2^n or
 * a smaller number specified in the header.
 * Each entry contains four bytes.
 *
 * The color palette is not used when the bitmap is
 * 16-bit or higher; there are no palette bytes in
 * those BMP files.
 *
 *#################################################################*/




/*#################################################################
 *
 * Define 24bit TrueColor Image, the storage is compatible with
 * the disk image of BMP file
 *
 * ################################################################*/

typedef
struct struct_TrueColorImage
{
    UINT8   *ImgData;

    /* dimension */
    INT32    ImgWidth;
    INT32    ImgHeight;

    /* these data can be computed from all the above
     * values, stored here to be ready to use */
    INT32    ImgByteWidth;
    INT32    ImgTotalPixels;
    INT32    ImgTotalBytes;
}
TrueColorImage;



#ifdef __cplusplus
extern "C" {
#endif


    /*##############################################################
     *
     * IsLittleEndian: test whether the ENDIANNESS of the
     *                 machine currently working on.
     *
     * ############################################################*/

    int IsLittleEndian();

    /*##############################################################
     *
     * SEWriteINT16ToFile and SEWriteINT32ToFile:
     *             Write 2-byte integer and 4-byte integer to
     *             the disk file according to their endianness.
     *             Both types of integers are written in small
     *             endianness (that where the prefix SE came from)
     *
     * ############################################################*/

    void SEWriteINT16ToFile( FILE* const fp,
                             const char endianness,
                             const void* const data);

    void SEWriteINT32ToFile( FILE* const fp,
                             const char endianness,
                             const void* const data);


    /*##############################################################
     *
     * SEReadINT16FromFile and SEReadINT32FromFile:
     *             Read 2-byte integer and 4-byte integer stored
     *             in little endian order.
     *
     * ############################################################*/


    void SEReadINT16FromFile( FILE* const fp,
                              const char endianness,
                              void* const data);

    void SEReadINT32FromFile( FILE* const fp,
                              const char endianness,
                              void* const data);

    /*#############################################################
     *
     * TCIMG_Init
     *
     * return value:  0: successful
     *                1: file open error
     *                2: file writting error with fputc
     *                3: file writting error with fwrite
     *
     * ############################################################*/

    int TCIMG_Init( TrueColorImage* const img,
                    const INT32 Width,
                    const INT32 Height);

    void TCIMG_Release(TrueColorImage* const img);

    int TCIMG_IsInit(TrueColorImage* const img);

    /*#############################################################
     *
     * TCIMG_SaveToFile
     *
     * return value:  0: successful
     *                1: file open error
     *                2: file writting error with fputc
     *                3: file writting error with fwrite
     *
     * ############################################################*/

    /* save and read from disk files */
    int TCIMG_SaveToFile( const TrueColorImage* const img,
                          const char filename[]);

    int TCIMG_ReadFromFile( TrueColorImage* const img,
                          const char filename[]);

    int TCIMG_SetPixelClr( TrueColorImage* const img,
                           INT32 x, INT32 y,
                           UINT8 R, UINT8 G, UINT8 B );

    int TCIMG_GetPixelClr( const TrueColorImage* const img,
                           INT32 x, INT32 y,
                           UINT8 *R, UINT8 *G, UINT8 *B );

    int TCIMG_FillClr( TrueColorImage* const img,
                       UINT8 R, UINT8 G, UINT8 B );




#ifdef __cplusplus
}
#endif

#endif


/* EOF */


