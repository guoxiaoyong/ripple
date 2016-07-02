/*####################################################################
 *
 * CopyRight(c)  2009
 *
 * File: heightmap.c
 *
 * Description: maintaining a 2D height map that describes
 *              2D wave.
 *
 * Author: Xiaoyong Guo
 *
 *##################################################################*/



/* C standard library headers */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>


#include <heightmap.h>


#ifdef __cplusplus
extern "C" {
#endif


    /*##################################################################
     *
     * BoundaryHandler:
     *
     * mod:
     *      'n' or 'N', doing nothing
     *      'c' or 'C', circular
     *      'r' or 'R', reflect
     *
     * #################################################################*/

    int BoundaryHandler(int x, int LB, int UB, char mod)
    {
        /* x is within the defined range */
        if ( (x >= LB) && (x <= UB) )
        {
            return x;
        }

        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
        /* ELSE if x is not in the range */
        if (mod == 'n' || mod == 'N' )
        {
            if (x < LB)
            {
                return LB;
            }

            if (x > UB)
            {
                return UB;
            }
        }

        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
        if (mod == 'c' || mod == 'C' )
        {
            if (x < LB)
            {
                return UB-(LB-x);
            }

            if (x > UB)
            {
                return LB+(x-UB);
            }
        }


        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
        if (mod == 'r' || mod == 'R' )
        {
            if (x < LB)
            {
                return LB+(LB-x);
            }

            if (x > UB)
            {
                return UB-(x-UB);
            }
        }

        return 0;
    }


    /*################################################################
     *
     * HeightMap_Init: initialize the HeightMap structure
     *
     * return value: 0, successful.
     *               1, the size of the data block is too big.
     *               2, 3, 4, 5, memory allocation failed.
     *
     *################################################################*/

    int HeightMap_Init( HeightMap* const hm,
                        const int width,
                        const int height )
    {
        int i;
        hm->Width  =  width;
        hm->Height =  height;

        hm->InnerWidth  = width  - 2;
        hm->InnerHeight = height - 2;

        hm->Size = width*height;

        /* overflow handling */
        if (hm->Size < width || hm->Size < height)
        {
            return 1;
        }


        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
         *
         *  Allocate memory to hm->CurrHeightMap and hm->PrevHeightMap
         *  The data block are initialize to zeros
         *
         * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

        hm->CurrHeightMap = (int*)calloc(hm->Size, sizeof(int));
        if (!hm->CurrHeightMap)
        {
            /* memory allocation failed */
            return 2;
        }

        hm->PrevHeightMap = (int*)calloc(hm->Size, sizeof(int));
        if (!hm->PrevHeightMap)
        {
            /* memory allocation failed */
            return 3;
        }

        hm->DX = (int*)calloc(hm->Size, sizeof(int));
        if (!hm->DX)
        {
            /* memory allocation failed */
            return 4;
        }

        hm->DY = (int*)calloc(hm->Size, sizeof(int));
        if (!hm->DY)
        {
            /* memory allocation failed */
            return 5;
        }


        /* point arrays which point to the beginning and end of a line */
        hm->iCurrLineBegin = (int**)calloc(hm->Height, sizeof(int*));
        if ( !hm->iCurrLineBegin )
        {
            return 6;
        }

        hm->iCurrLineEnd = (int**)calloc(hm->Height, sizeof(int*));
        if ( !hm->iCurrLineEnd)
        {
            return 7;
        }

        hm->iPrevLineBegin = (int**)calloc(hm->Height, sizeof(int*));
        if ( !hm->iPrevLineBegin )
        {
            return 6;
        }

        hm->iPrevLineEnd = (int**)calloc(hm->Height, sizeof(int*));
        if ( !hm->iPrevLineEnd)
        {
            return 7;
        }

        /* points to DX and DY data block*/
        hm->DXLineBegin = (int**)calloc(hm->Height, sizeof(int*));
        if (!hm->DXLineBegin)
        {
            return 8;
        }

        hm->DXLineEnd = (int**)calloc(hm->Height, sizeof(int*));
        if (!hm->DXLineEnd)
        {
            return 9;
        }

	hm->DYLineBegin = (int**)calloc(hm->Height, sizeof(int*));
        if (!hm->DYLineBegin)
        {
            return 10;
        }

        hm->DYLineEnd = (int**)calloc(hm->Height, sizeof(int*));
        if (!hm->DYLineEnd)
        {
            return 11;
        }

        /* iXXXXLineBegin and iXXXXLineEnd is only for internal use */
        hm->CurrLineBegin = hm->iCurrLineBegin;
        hm->CurrLineEnd = hm->iCurrLineEnd;

        hm->PrevLineBegin = hm->iPrevLineBegin;
        hm->PrevLineEnd = hm->iPrevLineEnd;


        /* initialize these points */
        hm->CurrLineBegin[0] = hm->CurrHeightMap;
        hm->CurrLineEnd[0]   = hm->CurrLineBegin[0] + hm->Width - 1;

        hm->PrevLineBegin[0] = hm->PrevHeightMap;
        hm->PrevLineEnd[0]   = hm->PrevLineBegin[0] + hm->Width - 1;

        hm->DXLineBegin[0] = hm->DX;
        hm->DXLineEnd[0]   = hm->DX + hm->Width - 1;

        hm->DYLineBegin[0] = hm->DY;
        hm->DYLineEnd[0] = hm->DY + hm->Width -1;

        for (i = 1; i < hm->Height; ++i)
        {
            hm->CurrLineBegin[i]  =  hm->CurrLineBegin[i-1] + hm->Width;
            hm->CurrLineEnd[i]    =  hm->CurrLineEnd[i-1] + hm->Width;

            hm->PrevLineBegin[i]  =  hm->PrevLineBegin[i-1] + hm->Width;
            hm->PrevLineEnd[i]    =  hm->PrevLineEnd[i-1] + hm->Width;

            hm->DXLineBegin[i] = hm->DXLineBegin[i-1] + hm->Width;
            hm->DXLineEnd[i] = hm->DXLineEnd[i-1] + hm->Width;

            hm->DYLineBegin[i] = hm->DYLineBegin[i-1] + hm->Width;
            hm->DYLineEnd[i] = hm->DYLineEnd[i-1] + hm->Width;
        }




        /* successful */
        return 0;
    }




    /*#############################################################
     *
     * HeightMap_Release: release the dynamically allocated
     *                    memory blocks.
     *
     *#############################################################*/

    void HeightMap_Release(HeightMap* const hm)
    {
        free(hm->CurrHeightMap);
        free(hm->PrevHeightMap);

        free(hm->iCurrLineBegin);
        free(hm->iCurrLineEnd);

        free(hm->iPrevLineBegin);
        free(hm->iPrevLineEnd);

        hm->CurrHeightMap = NULL;
        hm->PrevHeightMap = NULL;

        hm->iCurrLineBegin = NULL;
        hm->iPrevLineBegin = NULL;
        hm->iCurrLineEnd = NULL;
        hm->iPrevLineEnd = NULL;
    }




    /*#############################################################
     *
     * Four get-value and set-value functions are defined to
     * access the data.
     *
     *        GetCurrHMValue
     *        GetPrevHMValue
     *        SetCurrHMValue
     *        SetPrevHMValue
     *
     *  GetCurrHMValue and GetPrevHMValue use the glocal
     *  variable errno defined in errno.h to indicate
     *  whether an out-of-range has occured.
     *
     *  SetCurrHMValue and SetPrevHMValue also use errno
     *  as their get-value counterparts. They also return 1
     *  to indicate success and 0 to indicate failure.
     *
     *#############################################################*/

    int GetCurrHMValue( const HeightMap* const hm,
                        const int x,
                        const int y )
    {
        if (x < 0 || x >= hm->Width
                || y < 0 || y >= hm->Height)
        {
            errno = ERANGE; /* out of range error code */

            /* You don't need to check the value of errno
             * if the return value is equal to 0 */
            return 0;
        }
        else
        {
            return hm->CurrHeightMap [y*hm->Width+x];
        }
    }



    int GetPrevHMValue( const HeightMap* const hm,
                        const int x,
                        const int y )
    {
        if (x < 0 || x >= hm->Width
                || y < 0 || y >= hm->Height)
        {
            errno = ERANGE; /* out of range errnor code */

            /* You don't need to check the value of errno
             * if the return value is equal to 0 */
            return 0;
        }
        else
        {
            return hm->PrevHeightMap [y*hm->Width+x];
        }
    }



    int SetCurrHMValue( HeightMap* const hm,
                        const int x,
                        const int y,
                        const int value )
    {
        if (x < 0 || x >= hm->Width
                || y < 0 || y >= hm->Height)
        {
            errno = ERANGE; /* out of range error code */
            return 0;
        }
        else
        {
            hm->CurrHeightMap[y*hm->Width+x] = value;
            return 1;
        }
    }



    int SetPrevHMValue( HeightMap* const hm,
                        const int x,
                        const int y,
                        const int value )
    {
        if (x < 0 || x >= hm->Width
                || y < 0 || y >= hm->Height)
        {
            errno = ERANGE; /* out of range error code */
            return 0;
        }
        else
        {
            hm->PrevHeightMap [y*hm->Width+x] = value;
            return 1;
        }
    }



    int GetHMDXValue( const HeightMap* const hm,
                      const int x,
                      const int y )
    {
        if (x < 0 || x >= hm->Width
                || y < 0 || y >= hm->Height)
        {
            errno = ERANGE; /* out of range error code */

            /* You don't need to check the value of errno
             * if the return value is equal to 0 */
            return 0;
        }
        else
        {
            return hm->DX [y*hm->Width+x];
        }
    }



    int GetHMDYValue( const HeightMap* const hm,
                      const int x,
                      const int y )
    {
        if (x < 0 || x >= hm->Width
                || y < 0 || y >= hm->Height)
        {
            errno = ERANGE; /* out of range errnor code */

            /* You don't need to check the value of errno
             * if the return value is equal to 0 */
            return 0;
        }
        else
        {
            return hm->DY [y*hm->Width+x];
        }
    }



    int SetHMDXValue( HeightMap* const hm,
                      const int x,
                      const int y,
                      const int value )
    {
        if (x < 0 || x >= hm->Width
                || y < 0 || y >= hm->Height)
        {
            errno = ERANGE; /* out of range error code */
            return 0;
        }
        else
        {
            hm->DX [y*hm->Width+x] = value;
            return 1;
        }
    }



    int SetHMDYValue( HeightMap* const hm,
                      const int x,
                      const int y,
                      const int value )
    {
        if (x < 0 || x >= hm->Width
                || y < 0 || y >= hm->Height)
        {
            errno = ERANGE; /* out of range error code */
            return 0;
        }
        else
        {
            hm->DY [y*hm->Width+x] = value;
            return 1;
        }
    }



    /*#############################################################
     *
     * HeightMap_next: compute the Height Map of the wave for
     *                 the next moment. The algorithm is from
     *                 the article 2D Water by Hugo Elias.
     *
     *  http//freespace.virgin.net/hugo.elias/graphics/x_water.htm
     *
     *#############################################################*/


    void HeightMap_Next(HeightMap* const hm)
    {
        register int *PrevPtr, *CurrPtr, *Ptr;
        register int **iPtr;

        int i;

#define __SLOW_HM_NEXT__

#undef __SLOW_HM_NEXT__

#ifdef __SLOW_HM_NEXT__

        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        *
         * Slow but simple way to compute the wave propogation
        *
         *>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

        int j, hmpix;
        int x[4], y[4];
        const char MODE = 'R';

        for (i = 0; i < hm->Width; ++i)
        {
            for (j = 0; j < hm->Height; ++j)
            {
                x[0] = BoundaryHandler(i-1, 0, hm->Width-1, MODE);
                y[0] = BoundaryHandler(j, 0, hm->Height-1, MODE);

                x[1] = BoundaryHandler(i+1, 0, hm->Width-1, MODE);
                y[1] = BoundaryHandler(j, 0, hm->Height-1, MODE);

                x[2] = BoundaryHandler(i, 0, hm->Width-1, MODE);
                y[2] = BoundaryHandler(j-1, 0, hm->Height-1, MODE);

                x[3] = BoundaryHandler(i, 0, hm->Width-1, MODE);
                y[3] = BoundaryHandler(j+1, 0, hm->Height-1, MODE);

                hmpix = GetCurrHMValue(hm, x[0], y[0])+
                        GetCurrHMValue(hm, x[1], y[1])+
                        GetCurrHMValue(hm, x[2], y[2])+
                        GetCurrHMValue(hm, x[3], y[3]);
                hmpix /= 2;
                hmpix -= GetPrevHMValue(hm, i, j);
                hmpix -= (hmpix >> 5);

                SetPrevHMValue(hm, i, j, hmpix);
            }
        }
#endif


#define __FAST_HM_NEXT__

#ifdef __FAST_HM_NEXT__

        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        *
         * Fast but complicated way to compute the wave propogation
        *
         *>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

        /* process (0,0) and (Width-1, 0) */
        PrevPtr  =  hm->PrevLineBegin[0];
        CurrPtr  =  hm->CurrLineBegin[0];
        *PrevPtr =  CurrPtr[1] + CurrPtr[hm->Width] - *PrevPtr;
        *PrevPtr -= ( (*PrevPtr) >> 5 );

        PrevPtr  =  hm->PrevLineEnd[0];
        CurrPtr  =  hm->CurrLineEnd[0];
        *PrevPtr =  CurrPtr[-1] + CurrPtr[hm->Width] - *PrevPtr;
        *PrevPtr -= ( (*PrevPtr) >> 5 );

        /* process the points between (0,0) and (width-1, 0) */
        Ptr = hm->PrevLineEnd[0];
        PrevPtr = hm->PrevLineBegin[0]+1;
        CurrPtr = hm->CurrLineBegin[0]+1;

        while (PrevPtr < Ptr)
        {
            *PrevPtr
            = (( CurrPtr[-1] + CurrPtr[+1]
                 + 2*CurrPtr[+hm->Width]) >> 1 )
              - *PrevPtr;

            (*PrevPtr) -= ( (*PrevPtr) >> 5 );

            PrevPtr++;
            CurrPtr++;
        }

        /* process (0, height-1) and (width-1, height-1)*/
        PrevPtr  =  hm->PrevLineBegin[hm->Height-1];
        CurrPtr  =  hm->CurrLineBegin[hm->Height-1];
        *PrevPtr =  CurrPtr[1] + CurrPtr[-hm->Width] - *PrevPtr;
        *PrevPtr -= ( (*PrevPtr) >> 5 );

        PrevPtr  =  hm->PrevLineEnd[hm->Height-1];
        CurrPtr  =  hm->CurrLineEnd[hm->Height-1];
        *PrevPtr =  CurrPtr[-1] + CurrPtr[-hm->Width] - *PrevPtr;
        *PrevPtr -= ( (*PrevPtr) >> 5 );

        /* process the points between (0,height-1) and (width-1, height-1) */
        Ptr = hm->PrevLineEnd[hm->Height-1];
        PrevPtr = hm->PrevLineBegin[hm->Height-1]+1;
        CurrPtr = hm->CurrLineBegin[hm->Height-1]+1;

        while (PrevPtr < Ptr)
        {
            *PrevPtr
            = (( CurrPtr[-1] + CurrPtr[+1]
                 + 2*CurrPtr[-hm->Width]) >> 1 )
              - *PrevPtr;

            (*PrevPtr) -= ( (*PrevPtr) >> 5 );

            PrevPtr++;
            CurrPtr++;
        }

        for (i = 1; i <= hm->InnerHeight; ++i)
        {
            /* process the points betwwen (0, 0) and (0, heigth-1)*/
            PrevPtr = hm->PrevLineBegin[i];
            CurrPtr = hm->CurrLineBegin[i];

            *PrevPtr
            = (( 2*CurrPtr[+1]
                 + CurrPtr[-hm->Width]
                 + CurrPtr[+hm->Width]) >> 1 )
              - *PrevPtr;

            (*PrevPtr) -= ( (*PrevPtr) >> 5 );

            /* process the points betwwen (widht-1, 0) and (width-1, heigth-1)*/
            PrevPtr = hm->PrevLineEnd[i];
            CurrPtr = hm->CurrLineEnd[i];

            *PrevPtr
            = (( 2*CurrPtr[-1]
                 + CurrPtr[-hm->Width]
                 + CurrPtr[+hm->Width]) >> 1 )
              - *PrevPtr;

            (*PrevPtr) -= ( (*PrevPtr) >> 5 );
        }


        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        *
        * Process the inner area of the HeightMap
        *
         *>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

        for (i = 1; i <= hm->InnerHeight; ++i)
        {
            Ptr = hm->PrevLineEnd[i];

            PrevPtr = hm->PrevLineBegin[i]+1;
            CurrPtr = hm->CurrLineBegin[i]+1;

            while (PrevPtr < Ptr)
            {
                *PrevPtr
                = (( CurrPtr[-1] + CurrPtr[+1]
                     + CurrPtr[+hm->Width]
                     + CurrPtr[-hm->Width] ) >> 1 )
                  - *PrevPtr;

                (*PrevPtr) -= ( (*PrevPtr) >> 5 );

                PrevPtr++;
                CurrPtr++;
            }
        }

#endif


        /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
        /* Swap hm->CurrHeightMap and hm->PreHeightMap */
        Ptr = hm->CurrHeightMap;
        hm->CurrHeightMap = hm->PrevHeightMap;
        hm->PrevHeightMap = Ptr;

        iPtr = hm->CurrLineBegin;
        hm->CurrLineBegin = hm->PrevLineBegin;
        hm->PrevLineBegin = iPtr;

        iPtr = hm->CurrLineEnd;
        hm->CurrLineEnd = hm->PrevLineEnd;
        hm->PrevLineEnd = iPtr;
    }



    void HeightMap_PreRendering(HeightMap* const hm)
    {
        register int i;

#define __SLOW_PRERENDER__

#undef  __SLOW_PRERENDER__

#ifdef  __SLOW_PRERENDER__

        register int j, dx, dy;

        for (i = 1; i <= hm->InnerWidth; ++i)
        {
            for (j = 1; j <= hm->InnerHeight; ++j)
            {
                dx = GetCurrHMValue(hm, i-1, j) - GetCurrHMValue(hm, i+1, j);
                dy = GetCurrHMValue(hm, i, j-1) - GetCurrHMValue(hm, i, j+1);
                SetHMDXValue(hm, i, j, dx);
                SetHMDYValue(hm, i, j, dy);
            }
        }
#endif


#define __FAST_PRERENDER__

//#undef __FAST_PRERENDER__

#ifdef __FAST_PRERENDER__
        register int *CurrHMPtr, *DXPtr, *DYPtr;

        /* corners are always zeros */
        *(hm->DX) = 0;
        *(hm->DXLineEnd[0]) = 0;
        *(hm->DXLineBegin[hm->Height-1]) = 0;
        *(hm->DXLineEnd[hm->Height-1]) = 0;

        *(hm->DY) = 0;
        *(hm->DYLineEnd[0]) = 0;
        *(hm->DYLineBegin[hm->Height-1]) = 0;
        *(hm->DYLineEnd[hm->Height-1]) = 0;

        /* process inner area */
        for (i = 1; i <= hm->InnerHeight; ++i)
        {
            CurrHMPtr = hm->CurrLineBegin[i]+1;
            DXPtr = hm->DXLineBegin[i]+1;
            DYPtr = hm->DYLineBegin[i]+1;

            while (CurrHMPtr < hm->CurrLineEnd[i])
            {
                *DXPtr = CurrHMPtr[-1] - CurrHMPtr[+1];
                *DYPtr = CurrHMPtr[-hm->Width] - CurrHMPtr[+hm->Width];

                CurrHMPtr++;
                DXPtr++;
                DYPtr++;
            }
        }

        for (i = 1; i <= hm->InnerHeight; ++i)
        {
            CurrHMPtr = hm->CurrLineBegin[i];
            DXPtr = hm->DXLineBegin[i];
            DYPtr = hm->DYLineBegin[i];

            *DXPtr = 0;
            *DYPtr = CurrHMPtr[-hm->Width] - CurrHMPtr[+hm->Width];

	    /* end */
            CurrHMPtr = hm->CurrLineEnd[i];
            DXPtr = hm->DXLineEnd[i];
            DYPtr = hm->DYLineEnd[i];

            *DXPtr = 0;
            *DYPtr = CurrHMPtr[-hm->Width] - CurrHMPtr[+hm->Width];
        }


        /* process (1, 0) to (width-2, 0)*/
        CurrHMPtr = hm->CurrLineBegin[0]+1;
        DXPtr = hm->DXLineBegin[0]+1;
        DYPtr = hm->DYLineBegin[0]+1;

        while (CurrHMPtr < hm->CurrLineEnd[0])
        {
            *DXPtr = CurrHMPtr[-1] - CurrHMPtr[+1];
            *DYPtr = 0;

            CurrHMPtr++;
            DXPtr++;
            DYPtr++;
        }

        /* process (1, height-1) to (width-2, height-1)*/
        CurrHMPtr = hm->CurrLineBegin[hm->Height-1]+1;
        DXPtr = hm->DXLineBegin[hm->Height-1]+1;
        DYPtr = hm->DYLineBegin[hm->Height-1]+1;

        while (CurrHMPtr < hm->CurrLineEnd[hm->Height-1])
        {
            *DXPtr = CurrHMPtr[-1] - CurrHMPtr[+1];
            *DYPtr = 0;

            CurrHMPtr++;
            DXPtr++;
            DYPtr++;
        }


#endif
    }


    void HeightMap_CheckHeight(HeightMap* const hm)
    {
        int i, j, currValue, prevValue;
        for (i = 0; i < hm->Width; ++i)
        {
            for (j = 0; j < hm->Height; ++j)
            {
                currValue = GetHMDXValue(hm, i, j);
                prevValue = GetHMDYValue(hm, i, j);

                if ( currValue != 0 )
                {
                    printf("currValue(%d, %d) = %d\n", i, j, currValue);
                }

                if ( prevValue != 0 )
                {
                    printf("prevValue(%d, %d) = %d\n", i, j, prevValue);
                }

            }
        }
    }

#ifdef __cplusplus
}
#endif



/* EOF */



