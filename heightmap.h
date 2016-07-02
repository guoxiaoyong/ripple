#ifndef  __HEIGHTMAP_H__
#define  __HEIGHTMAP_H__


#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef 
struct stru_HeightMap
{
    /* Private Data */
    int* CurrHeightMap;
    int* PrevHeightMap;
    int* DX;
    int* DY;

    int** CurrLineBegin;
    int** CurrLineEnd;
    int** PrevLineBegin;
    int** PrevLineEnd;
   
    int** iCurrLineBegin;
    int** iCurrLineEnd;
    int** iPrevLineBegin;
    int** iPrevLineEnd;

    int** DXLineBegin;
    int** DXLineEnd;
    int** DYLineBegin;
    int** DYLineEnd;

    int Width;
    int Height;
    int InnerWidth;
    int InnerHeight;
    int Size;
} HeightMap;

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

    int BoundaryHandler(int x, int LB, int UB, char mod);

    /* Constructor */
    int  HeightMap_Init( HeightMap* const hm,
                         const int width,
                         const int height );
    /*Destruct */
    void HeightMap_Release(HeightMap* hm);

    /* routines for accessing data */
    int GetCurrHMValue(const HeightMap* const hm,
                       const int x,
                       const int y);
    int GetPrevHMValue(const HeightMap* const hm,
                       const int x,
                       const int y);
    int SetCurrHMValue(HeightMap* const hm,
                       const int x,
                       const int y,
                       const int value);
    int SetPrevHMValue(HeightMap* const hm,
                       const int x,
                       const int y,
                       const int value);

    int GetHMDXValue(const HeightMap* const hm,
                     const int x,
                     const int y);
    int GetHMDYValue(const HeightMap* const hm,
                     const int x,
                     const int y);
    int SetHMDXValue(HeightMap* const hm,
                     const int x,
                     const int y,
                     const int value);
    int SetHMDYValue(HeightMap* const hm,
                     const int x,
                     const int y,
                     const int value);

    void HeightMap_Next(HeightMap* const hm);
    void HeightMap_PreRendering(HeightMap* const hm);

    /* for debug */
    void HeightMap_CheckHeight(HeightMap* const hm);

#ifdef __cplusplus
}
#endif


#endif


/* EOF */


