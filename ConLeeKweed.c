/*###################################################################
 *
 * ConLeeKweed - Liquid, A water ripple rendering program
 *               that produces BMP files
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


/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 *
 * TODO:
 *  1. parameter check
 *  2. print usage
 *
 *>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include <heightmap.h>
#include <tcimg.h>


const char MSGContent[] =
"ConLeeKweed is a small program simulating water ripples on a 2D surface. \n\
The source code can be downloaded from my website http://www.wordeazy.com \n\
ConLeeKweed produces a series of BMP files. There are some tools like \n\
convert from ImageMagick can convert them into an animated GIF file.\n\n\
ConLeeKweed was written by Xiaoyong Guo, a programming enthusiast.\n \
Feel free to send me email if you have any questions about this program.\n\
My email address is guo.xiaoyong@gmail.com.\n"; 



/*###################################################################
 *
 * Image Rendering Function:
 *
 * img[out]:   the result the of the rendering 
 * bgimg[in]:  backgroud image, i.e., the original image
 * hm[in]:     the height map of the wave
 * 
 * img, bgimg, hm all need to be the same size
 *
 * TODO: chekc the dimensions 
 *
 *##################################################################*/

int Water2D_Render( TrueColorImage* const img,
                    const TrueColorImage* const bgimg,
                    const HeightMap* const hm)
{
    const char MODE = 'R';
    register int i, j;

    int dx, dy, x, y;
    UINT8 R, G, B;
    int info;

    for (i = 0; i < img->ImgWidth; ++i)
    {
        for (j = 0; j < img->ImgHeight; ++j)
        {
            dx = GetHMDXValue(hm, i, j);
            dy = GetHMDYValue(hm, i, j);

            x = BoundaryHandler(i+dx, 0, img->ImgWidth-1, MODE);
            y = BoundaryHandler(j+dy, 0, img->ImgWidth-1, MODE);

            info = TCIMG_GetPixelClr(bgimg, x, y, &R, &G, &B);
            if (info == 0)
            {
                info = TCIMG_SetPixelClr(img, i, j, R, G, B);
            }
        }
    }

    return 0;
}




/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 *
 * Main Function
 *
 *>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

int main(int argc, char* argv[])
{
    extern TrueColorImage TCBGImage2;
    const TrueColorImage* BGImage = &TCBGImage2;
    char filename[256];
    int Drop_x, Drop_y;
    int  i, j, k;

    HeightMap       hm;
    TrueColorImage  img;

    int nFrames = 200;

    time_t StartTime = time(NULL);
    time_t EndTime;


    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/


    printf("%s\n", MSGContent);

    HeightMap_Init(&hm, BGImage->ImgWidth, BGImage->ImgHeight);
    TCIMG_Init(&img, BGImage->ImgWidth, BGImage->ImgHeight);

    srand(StartTime);
    for (k = 0; k < nFrames; ++k)
    {
        if (rand()%30 < 4)
        {
            Drop_x = rand()%BGImage->ImgWidth;
            Drop_y = rand()%BGImage->ImgHeight;

            for (i = -2; i < 2; ++i)
            {
                for (j = -2; j < 2; ++j)
                {
                    SetPrevHMValue(&hm, Drop_x+i,
                                   Drop_y+j, -200);
                }
            }
        }

        HeightMap_Next(&hm);
        HeightMap_PreRendering(&hm);

        Water2D_Render(&img, BGImage, &hm);

        sprintf(filename, "%06d.bmp", k);
        TCIMG_SaveToFile(&img, filename);
    }

    TCIMG_Release(&img);
    HeightMap_Release(&hm);

    EndTime = time(NULL);

    printf("Time Elapsed: %ld sec", EndTime-StartTime);


    return EXIT_SUCCESS;
}




