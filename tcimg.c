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


/* standard C library headers */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <setjmp.h>


#include <tcimg.h>

static jmp_buf FileWriteExceptionEnv;
static jmp_buf FileReadExceptionEnv;


#ifdef __cplusplus
extern "C" {
#endif


    /*##############################################################
     *
     * IsLittleEndian: test whether the ENDIANNESS of the
     *                 machine currently working on.
     *
     * ############################################################*/

    int IsLittleEndian()
    {
        unsigned int   number = 0x12345678;
        unsigned char *bytes  = (unsigned char*)&number;

        if ( bytes[0] == (unsigned char)0x78 )
        {
            return 1;
        }
        else if (bytes[0] == (unsigned char)0x12)
        {
            return 0;
        }
        else
        {
            fprintf(stderr, "IsLittleEndian: error occurred!!\n");
            return -1;
        }
    }




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
                             const void* const data)
    {
        const char* bytes = (const char*)data;
        const int ErrorID = (int)('w');
        const INT32 SIZ  = (INT32)sizeof(UINT16);
        register INT32 i;

        if ( endianness == 's' || endianness == 'S' )
        {
            for (i = 0; i < SIZ; ++i)
            {
                fputc(bytes[i], fp);
                if ( ferror(fp) )
                {
                    longjmp(FileWriteExceptionEnv, ErrorID);
                }
            }
        }
        else if ( endianness == 'b' || endianness == 'B' )
        {
            for (i = SIZ-1; i >= 0; --i)
            {
                fputc(bytes[i], fp);
                if ( ferror(fp) )
                {
                    longjmp(FileWriteExceptionEnv, ErrorID);
                }
            }
        }
        else
        {
            /* should never happend */
            fprintf(stderr, "SEWriteINT16ToFile Error!!\n");
        }
    }


    void SEWriteINT32ToFile( FILE* const fp,
                             const char endianness,
                             const void* const data)
    {
        const char* bytes = (const char*)data;
        const int ErrorID = (int)('w');
        const INT32 SIZ  = (INT32)sizeof(INT32);
        register int i;

        if ( endianness == 's' || endianness == 'S' )
        {
            for (i = 0; i < SIZ; ++i)
            {
                fputc(bytes[i], fp);
                if ( ferror(fp) )
                {
                    longjmp(FileWriteExceptionEnv, ErrorID);
                }
            }
        }
        else if ( endianness == 'b' || endianness == 'B' )
        {
            for (i = SIZ-1; i >= 0; --i)
            {
                fputc(bytes[i], fp);
                if ( ferror(fp) )
                {
                    longjmp(FileWriteExceptionEnv, ErrorID);
                }
            }
        }
        else
        {
            /* should never happend */
            fprintf(stderr, "SEWriteINT32ToFile Error!!\n");
        }
    }




    /*##############################################################
     *
     * SEReadINT16FromFile and SEReadINT32FromFile:
     *             Read 2-byte integer and 4-byte integer stored
     *             in little endian order.
     *
     * ############################################################*/


    void SEReadINT16FromFile( FILE* const fp,
                              const char endianness,
                              void* const data)
    {
        char* const bytes = (char* const)data;
        const int ErrorID = (int)('r');
        const INT32 SIZ  = (INT32)sizeof(UINT16);
        register int i;

        if ( endianness == 's' || endianness == 'S' )
        {
            for (i = 0; i < SIZ; ++i)
            {
                bytes[i] = (char)fgetc(fp);
                if ( ferror(fp) )
                {
                    longjmp(FileReadExceptionEnv, ErrorID);
                }
            }
        }
        else if ( endianness == 'b' || endianness == 'B' )
        {
            for (i = SIZ-1; i >= 0; --i)
            {
                bytes[i] = (char)fgetc(fp);
                if ( ferror(fp) )
                {
                    longjmp(FileReadExceptionEnv, ErrorID);
                }
            }
        }
        else
        {
            /* should never happend */
            fprintf(stderr, "SEWriteINT16ToFile Error!!\n");
        }
    }


    void SEReadINT32FromFile( FILE* const fp,
                              const char endianness,
                              void* const data)
    {
        char* const bytes = (char* const)data;
        const int ErrorID = (int)('r');
        const INT32 SIZ  = (INT32)sizeof(INT32);
        register int i;

        if ( endianness == 's' || endianness == 'S' )
        {
            for (i = 0; i < SIZ; ++i)
            {
                bytes[i] = (char)fgetc(fp);
                if ( ferror(fp) )
                {
                    longjmp(FileReadExceptionEnv, ErrorID);
                }
            }
        }
        else if ( endianness == 'b' || endianness == 'B' )
        {
            for (i = SIZ-1; i >= 0; --i)
            {
                bytes[i] = (char)fgetc(fp);
                if ( ferror(fp) )
                {
                    longjmp(FileReadExceptionEnv, ErrorID);
                }
            }
        }
        else
        {
            /* should never happend */
            fprintf(stderr, "SEWriteINT16ToFile Error!!\n");
        }
    }



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
                    const INT32 Height)
    {
        /* Bytes Per Pixel */
        const INT32 BPP = 3;

        assert(Width  >= 0);
        assert(Height >= 0);

        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
        img->ImgWidth  = Width;
        img->ImgHeight = Height;
        img->ImgByteWidth = img->ImgWidth * BPP;

        /* follow the rule which states the
         * Each new .BMP line must start on an even 32 bit boundary!
        * i.e., img->ImgByteWidth*8 % 32 == 0 */

        if ( img->ImgByteWidth % 4 != 0 )
        {
            /* img->ImgByteWidth = (img->ImgByteWidth / 4 + 1) * 4 ;*/
            img->ImgByteWidth = ( (img->ImgByteWidth >> 2) + 1) << 2;
        }


        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
        img->ImgTotalPixels = img->ImgWidth * img->ImgHeight;
        /* in case there's an overflow */
        assert (img->ImgTotalPixels > img->ImgWidth );

        img->ImgTotalBytes = img->ImgHeight * img->ImgByteWidth;
        /* in case there's an overflow */
        assert (img->ImgTotalBytes > img->ImgHeight);

        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/


        img->ImgData = (UINT8*)malloc((size_t)img->ImgTotalBytes);
        if (img->ImgData == NULL)
        {
            return 1;
        }

        /* successful */
        return 0;
    }

    void TCIMG_Release(TrueColorImage* const img)
    {
        free(img->ImgData);
        img->ImgData = NULL;

        /* dimension */
        img->ImgWidth  = 0;
        img->ImgHeight = 0;

        /* these data can be computed from all the above
         * values, stored here to be ready to use */
        img->ImgByteWidth   = 0;
        img->ImgTotalPixels = 0;
        img->ImgTotalBytes  = 0;
    }


    /*#############################################################
     *
     * TCIMG_IsInit: check whether a TrueColorImage structure has
     *               has been initialized.
     *
     * return value: 0, not initialized
     *               1, the structure has been initialized
     *
     * Note: this function check whether all the fields
     *       in the structure is consistent and reasonable.
     *       If the return value is 0, then this data structure
     *       is definited not initialized. If the return value
     *       is 1, then this structure is almost surely
     *       initialized. Although theoretically it is
     *       possible that all the fields in the structure
     *       are not been initialized but appears consistent
     *       and reasonable, but the chance for this to happen
     *       is very very small.
     *
     * ##########################################################*/


    int TCIMG_IsInit(TrueColorImage* const img)
    {
        const INT32 BPP = 3;

        if (img == NULL)
        {
            return 0;
        }

        if (img->ImgData == NULL)
        {
            return 0;
        }

        if (img->ImgWidth <=0)
        {
            return 0;
        }

        if (img->ImgHeight <= 0)
        {
            return 0;
        }

        /* follow the rule which states the
         * Each new .BMP line must start on an even 32 bit boundary! */
        if ( img->ImgByteWidth % 4 == 0 )
        {
            if (img->ImgByteWidth != img->ImgWidth * BPP)
            {
                return 0;
            }
        }
        else
        {
            /* img->ImgByteWidth =(ImgWidth*3 / 4 + 1) * 4 ;*/
            if ( img->ImgByteWidth
                    !=
                    ( (((img->ImgWidth * BPP) >> 2) + 1) << 2 ) )
            {
                return 0;
            }
        }


        if ( img->ImgTotalPixels
                != img->ImgWidth * img->ImgHeight)
        {
            return 0;
        }

        if ( img->ImgTotalBytes
                != img->ImgHeight * img->ImgByteWidth)
        {
            return 0;
        }

        return 1;
    }



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
                          const char filename[])
    {
        FILE* fp;
        int   info;
        MSDIB_FileHeader    bmFH;
        V3MSDIB_InfoHeader  bmIH;

        const UINT32 FHsiz = 14;
        const UINT32 IHsiz = 40;
        const UINT32 HeaderSiz = FHsiz+IHsiz;

        int ExceptionID;

        char endianness;

        if ( IsLittleEndian() )
        {
            endianness = 'S';
        }
        else
        {
            endianness = 'B';
        }


        /* open a file to write the image data */
        fp = fopen(filename, "wb");

        if ( fp == NULL )
        {
            /* openfile error */
            return 1;
        }

        ExceptionID = setjmp(FileWriteExceptionEnv);

        if (ExceptionID != 0)
        {
            info = fclose (fp);
            assert(info == 0);
            /* file writting error */
            return 2;
        }




        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
        /*++ BEGINNING OF SETTING THE FILE HEADER PARAMETERS */

        /* fill the field of the FileHeader and InfoHeader
         * with right numbers */
        bmFH.bfType = (UINT16)(('M' << 8) + 'B');

        bmFH.bfSize
        = HeaderSiz +
          (UINT32)(img->ImgHeight) * (UINT32)(img->ImgByteWidth);

        bmFH.bfReserved = 0x00000000;

        /* if there's palette, what is the OffBits ? */
        bmFH.bfOffBits  = HeaderSiz;

        /*++ END of SETTING THE FILE HEADER PARAMETERS */





        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
        /*++ BEGINNING OF SETTING THE INFO HEADER PARAMETERS */

        bmIH.biSize = IHsiz;

        bmIH.biWidth  = img->ImgWidth;
        bmIH.biHeight = img->ImgHeight;

        /* always 1 */
        bmIH.biPlanes = 1;

        /* should be 24 for true color image */
        bmIH.biBitCount = (UINT16)24;

        /* no compression */
        bmIH.biCompression = 0;

        bmIH.biSizeImage
        = (UINT32)(img->ImgHeight) * (UINT32)(img->ImgByteWidth);

        /* not useful for true color image */
        bmIH.biXPelsPerMeter  = 0;
        bmIH.biYPelsPerMeter  = 0;
        bmIH.biClrUsed        = 0;
        bmIH.biClrImportant   = 0;

        /*++ END OF SETTING THE INFO HEADER PARAMETERS */





        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
        /* Write File Header */

        SEWriteINT16ToFile(fp, endianness, &bmFH.bfType    );
        SEWriteINT32ToFile(fp, endianness, &bmFH.bfSize    );
        SEWriteINT32ToFile(fp, endianness, &bmFH.bfReserved);
        SEWriteINT32ToFile(fp, endianness, &bmFH.bfOffBits );

        /*+++++++++++++++++++++++++++++++++++++++++++++++++*/
        /* Write Info Header */
        SEWriteINT32ToFile(fp, endianness, &bmIH.biSize  );
        SEWriteINT32ToFile(fp, endianness, &bmIH.biWidth );
        SEWriteINT32ToFile(fp, endianness, &bmIH.biHeight);

        SEWriteINT16ToFile(fp, endianness, &bmIH.biPlanes  );
        SEWriteINT16ToFile(fp, endianness, &bmIH.biBitCount);

        SEWriteINT32ToFile(fp, endianness, &bmIH.biCompression  );
        SEWriteINT32ToFile(fp, endianness, &bmIH.biSizeImage    );
        SEWriteINT32ToFile(fp, endianness, &bmIH.biXPelsPerMeter);
        SEWriteINT32ToFile(fp, endianness, &bmIH.biYPelsPerMeter);
        SEWriteINT32ToFile(fp, endianness, &bmIH.biClrUsed      );
        SEWriteINT32ToFile(fp, endianness, &bmIH.biClrImportant );



        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
        /* Write bitmap data, endianness is not a concern here */

        if ( fwrite(img->ImgData, bmIH.biSizeImage, 1, fp)
                != (size_t)bmIH.biSizeImage )
        {
            info = fclose(fp);
            assert(info == 0);
            return 3;
        }

        info = fclose(fp);
        assert(info == 0);
        return 0;
    }



    int TCIMG_ReadFromFile( TrueColorImage* const img,
                            const char filename[])
    {
        FILE*  fp = NULL;
        char   head = 0;
        int    info = 1;

        const UINT32 FHsiz = 14;
        const UINT32 IHsiz = 40;
        const UINT32 HeaderSiz = FHsiz+IHsiz;
        const UINT32 BPP   = 3; /* Bytes Per Pixel */

        /* important field and their position in the file */
        UINT32 bfOffBits = 0;
        INT32  bfOffBits_offset = 10;

        INT32 biWidth = 0;
        INT32 biWidth_offset = 18;

        INT32 biHeight = 0;
        INT32 biHeight_offset = 22;

        UINT16 biBitCount = 0;
        INT32  biBitCount_offset = 28;

        UINT32 biCompression = 0;
        INT32  biCompression_offset = 30;

        int ExceptionID = 9;

        char endianness;

        if ( IsLittleEndian() )
        {
            endianness = 'S';
        }
        else
        {
            endianness = 'B';
        }



        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
        /* open a BMP file */
        fp = fopen(filename, "rb");
        if (fp == NULL)
        {
            info = fclose(fp);
            assert(info == 0);
            /* file open error */
            return 1;
        }



        /* Exception handling */
        ExceptionID = setjmp(FileWriteExceptionEnv);

        if (ExceptionID != 0)
        {
            info = fclose (fp);
            assert(info == 0);
            /* file writting error */
            return 2;
        }

        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
        /* check the magic number */
        /* read the first byte */
        head = (char)fgetc(fp);
        if (ferror(fp))
        {
            info = fclose(fp);
            assert(info == 0);
            /* file reading error */
            return 3;
        }

        if ( head != 'B' )
        {
            info = fclose(fp);
            assert(info == 0);
            /* not-a-BMP file error */
            return 4;
        }

        /* read the second byte */
        head = (char)fgetc(fp);
        if (ferror(fp))
        {
            info = fclose(fp);
            assert(info == 0);
            /* file reading error */
            return 5;
        }
        if (head != 'M')
        {
            info = fclose(fp);
            assert(info == 0);
            /* not-a-BMP file error */
            return 6;
        }


        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
        /* only several field are important */

        info = fseek(fp, (long int)bfOffBits_offset, SEEK_SET);
        if (info == 0)
        {
            SEReadINT32FromFile(fp, endianness, &bfOffBits);
        }
        else
        {
            info = fclose(fp);
            assert(info == 0);
            return 7;
        }

        info = fseek(fp, (long int)biBitCount_offset, SEEK_SET);
        if (info == 0)
        {
            SEReadINT16FromFile(fp, endianness, &biBitCount);
        }
        else
        {
            info = fclose(fp);
            assert(info == 0);
            return 8;
        }

        info = fseek(fp, (long int)biCompression_offset, SEEK_SET);
        if (info == 0)
        {
            SEReadINT32FromFile(fp, endianness, &biCompression);
        }
        else
        {
            info = fclose(fp);
            assert(info == 0);
            return 9;
        }



        if (bfOffBits != HeaderSiz)
        {
            info = fclose(fp);
            assert(info == 0);
            return 10;
        }

        if ((UINT32)biBitCount != BPP*8)
        {
            info = fclose(fp);
            assert(info == 0);
            return 11;
        }

        if (biCompression != 0)
        {
            info = fclose(fp);
            assert(info == 0);
            return 12;
        }


        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
        /* get the width and height of the image */
        info = fseek(fp, (long int)biWidth_offset, SEEK_SET);
        if (info == 0)
        {
            SEReadINT32FromFile(fp, endianness, &biWidth);
        }
        else
        {
            info = fclose(fp);
            assert(info == 0);
            return 13;
        }

        info = fseek(fp, (long int)biHeight_offset, SEEK_SET);
        if (info == 0)
        {
            SEReadINT32FromFile(fp, endianness, &biHeight);
        }
        else
        {
            info = fclose(fp);
            assert(info == 0);
            return 14;
        }

        assert(biWidth  >= 0);
        assert(biHeight >= 0);


        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
        img->ImgWidth  = biWidth;
        img->ImgHeight = biHeight;

        /* follow the rule which states the
         * Each new .BMP line must start on an even 32 bit boundary! */
        if ( biWidth % 4 == 0 )
        {
            img->ImgByteWidth = BPP * biWidth;
        }
        else
        {
            /* img->ImgRealWidth =(biWidth * 3 / 4 + 1) * 4 ;*/
            img->ImgByteWidth = ( ((biWidth * 3) >> 2) + 1) << 2;
        }

        img->ImgTotalPixels = img->ImgWidth * img->ImgHeight;
        /* in case there's an overflow */
        assert (img->ImgTotalPixels > img->ImgWidth );

        img->ImgTotalBytes = img->ImgHeight * img->ImgByteWidth;
        /* in case there's an overflow */
        assert (img->ImgTotalBytes > img->ImgHeight);

        img->ImgData = (UINT8*)malloc((size_t)img->ImgTotalBytes);

        if (img->ImgData == NULL)
        {
            info = fclose(fp);
            assert(info == 0);
            return 15;
        }


        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
        /* goto the image data block */
        info = fseek(fp, (long int)HeaderSiz, SEEK_SET);
        if (info == 0)
        {
            fread(img->ImgData, 1, (size_t)img->ImgTotalBytes, fp);

            if (ferror(fp))
            {
                info = fclose(fp);
                assert(info == 0);
                return 16;
            }
        }
        else
        {
            info = fclose(fp);
            assert(info == 0);
            return 17;
        }


        info = fclose(fp);
        assert(info == 0);
        /* successful */
        return 0;
    }


    /*###############################################################
     *
     * TCIMG_SetPixelClr: set the point (x, y) to be the
     *                    RGB color (R, G, B)
     *
    ###############################################################*/

    int TCIMG_SetPixelClr( TrueColorImage* const img,
                           INT32 x, INT32 y,
                           UINT8 R, UINT8 G, UINT8 B )
    {
        /* Byte Per Pixel */
        const INT32 BPP = 3;

        UINT8* ptr = NULL;

        assert(img != NULL);

        /* check range */
        if ( x < 0 || x >= img->ImgWidth )
        {
            return 1;
        }

        if ( y < 0 || y >= img->ImgHeight)
        {
            return 2;
        }

        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

        ptr = img->ImgData + (y * img->ImgByteWidth) + (x * BPP);

        assert(ptr != NULL);

        ptr[0] = B;
        ptr[1] = G;
        ptr[2] = R;


        /* successful */
        return 0;
    }

    /*###############################################################
     *
     * TCIMG_SetPixelClr: set the point (x, y) to be the
     *                    RGB color (R, G, B)
     *
    ###############################################################*/

    int TCIMG_GetPixelClr( const TrueColorImage* const img,
                           INT32 x, INT32 y,
                           UINT8 *R, UINT8 *G, UINT8 *B )
    {
        /* Byte Per Pixel */
        const INT32 BPP = 3;

        UINT8* ptr = NULL;

        assert(img != NULL);

        /* check range */
        if ( x < 0 || x >= img->ImgWidth )
        {
            return 1;
        }

        if ( y < 0 || y >= img->ImgHeight)
        {
            return 2;
        }

        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

        ptr = img->ImgData + (y * img->ImgByteWidth) + (x * BPP);

        assert(ptr != NULL);

        *B = ptr[0];
        *G = ptr[1];
        *R = ptr[2];

        /* successful */
        return 0;
    }






    int TCIMG_FillClr( TrueColorImage* const img,
                       UINT8 R, UINT8 G, UINT8 B )
    {
        register INT32 i, j;

        if ( !TCIMG_IsInit(img) )
        {
            return 1;
        }


        for (i = 0; i < img->ImgWidth; ++i)
        {
            for (j = 0; j < img->ImgHeight; ++j)
            {
                TCIMG_SetPixelClr(img, i, j, R, G, B);
            }
        }

        return 0;
    }



#ifdef __cplusplus
}
#endif



/* EOF */


