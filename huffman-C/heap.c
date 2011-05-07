#include <stdio.h>
#include <string.h>
//#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include "heap.h"

int indexOf( char cReaded )
{
    return ( (int)cReaded + 256 )%256 - FIRSTCHAR;
}

void  InitializeHuffNodeArray( huffNode * ptCharFreq, int verbose )
{
  int i;
       if( verbose == 1 )
      printf("\n(D: initialization)");

  for( i = 0; i < CHARS; i++)
  {
    ptCharFreq[i].cCharacter = FIRSTCHAR + i;
    ptCharFreq[i].nFreq = OVERMAXFREQ;
    ptCharFreq[i].ptZeroChild = 0;
    ptCharFreq[i].ptOneChild = 0;
    ptCharFreq[i].ptParent = 0;
  }
}


void  InitializeHuffNodeArrayForMidleNodes( huffNode * ptCharFreq, int verbose )
{
  int i;
       if( verbose == 1 )
      printf("\n(D: initialization)");

  for( i = 0; i < RINGBUFFERSIZE; i++)
  {
    ptCharFreq[i].nFreq = OVERMAXFREQ;
    ptCharFreq[i].ptZeroChild = 0;
    ptCharFreq[i].ptOneChild = 0;
    ptCharFreq[i].ptParent = 0;
  }
}


void resetHuffNodeArray(huffNode * ptCharFreq)
{
  int i;
  for( i = 0; i < CHARS; i++)
  {
    ptCharFreq[i].nFreq = OVERMAXFREQ;
    ptCharFreq[i].ptParent = 0;
  }
}



void printHeap( huffNode ** pptCharFreqHeap )
{
    int i;
    for( i = 0; i < CHARS; i++)
        if( pptCharFreqHeap[i] != 0 )
        {
            printf("\n(D: i%d c%c dc%d #%d  |->i%d ",
                          i, pptCharFreqHeap[i]->cCharacter, 
                                  pptCharFreqHeap[i]->cCharacter, 
                                       pptCharFreqHeap[i]->nFreq, 
                                             i*2 + 1);

            if( i*2 + 1 < CHARS && pptCharFreqHeap[ i*2 + 1] != 0 ) 
                printf("c%c #%d",pptCharFreqHeap[ i*2 + 1]->cCharacter, pptCharFreqHeap[ i*2 + 1]->nFreq );
            printf(" ->i%d", i*2 + 2);
            if( i*2 + 2 < CHARS && pptCharFreqHeap[ i*2 + 2] != 0 ) 
                printf(" c%c #%d",pptCharFreqHeap[ i*2 + 2]->cCharacter, pptCharFreqHeap[ i*2 + 2]->nFreq );
            printf(")");
        }
}

huffNode ** createPointerHeap( huffNode * ptCharFreq, huffNode ** pptCharFreqHeap, int verbose )
{
    int i;
    //int nNewi;
    //int l;
    //int r;
    int nParent;
    int nChild;
    huffNode * ptTemp;

    if( verbose == 1 ) printf("\n(D: heapify)");

    for( i = 0; i < CHARS; i++)
    {
        pptCharFreqHeap[i] = &ptCharFreq[i];

        nChild = i;

        if( verbose == 1 ) printf("\n(D: i%d)",i);

        while( nChild > 0 )
        {
         
            nParent = ( nChild - 2 + nChild%2 )/2; // varmasti n„in

            if( verbose == 1 ) printf("\n(D:   child: %d parent %d)", nChild, nParent );

            if( pptCharFreqHeap[ nChild ]->nFreq < pptCharFreqHeap[ nParent ]->nFreq )
            {

                ptTemp = pptCharFreqHeap[ nParent ];

                pptCharFreqHeap[ nParent ] = pptCharFreqHeap[ nChild ];

                pptCharFreqHeap[ nChild ] = ptTemp;

                nChild = nParent;
            }
            else
                nChild = 0;
        }

    }

    

    /*for( i = CHARS/2; i >= 0; i--)
    {
        nNewi = i;
        l = nNewi*2+1;
        r = nNewi*2+2;

        if( verbose == 1 ) printf("\n(D: i%d c%c #%d  li%d ri%d)",nNewi,pptCharFreqHeap[nNewi]->cCharacter,pptCharFreqHeap[nNewi]->nFreq,l,r);

        if( l < CHARS && pptCharFreqHeap[l]->nFreq < pptCharFreqHeap[nNewi]->nFreq )
            nNewi = l;
        if( r < CHARS && pptCharFreqHeap[r]->nFreq < pptCharFreqHeap[nNewi]->nFreq  )
            nNewi = r;

        if( verbose == 1 ) printf("\n(D:     swapped to i%d c%c #%d)",nNewi,pptCharFreqHeap[nNewi]->cCharacter,pptCharFreqHeap[nNewi]->nFreq);

        if( i != nNewi )
        {
                ptTemp = pptCharFreqHeap[ i ];
                pptCharFreqHeap[ i ] = pptCharFreqHeap[ nNewi ];
                pptCharFreqHeap[ nNewi ] = ptTemp;
        }
    }*/
     
    if( verbose == 1 )
    {
        printf("\n(D: Sorted heap:)");
        printHeap( pptCharFreqHeap ) ;
    }
    return pptCharFreqHeap;
}

huffNode * popPointerHeap( huffNode ** pptCharFreqHeap, int verbose )
{
    int nCurrentIndex = 0;
    huffNode * ptReturned = pptCharFreqHeap[0];
    int nLeft;
    int nRight;
    int nNewMin;

    if( verbose == 1 ) printf("\n(D:pop)");
        
    if( pptCharFreqHeap[ nCurrentIndex ] == 0 || pptCharFreqHeap[ nCurrentIndex ]->nFreq == OVERMAXFREQ )
            return 0;

    if( verbose == 1 ) printf("\n(D: popping: %c %d %d )", ptReturned->cCharacter, ptReturned->cCharacter, ptReturned->nFreq );

    while( nCurrentIndex < CHARS )
    {
        nLeft  = nCurrentIndex*2 + 1;
        nRight = nCurrentIndex*2 + 2;

        if( nLeft >= CHARS || pptCharFreqHeap[ nLeft ] == 0 || pptCharFreqHeap[ nLeft ]->nFreq >= OVERMAXFREQ )
           nLeft = -1; 
        
        if( nRight >= CHARS || pptCharFreqHeap[ nRight ] == 0 || pptCharFreqHeap[ nRight ]->nFreq >= OVERMAXFREQ )
           nRight = -1; 

        if( nLeft != -1 && nRight != -1 )
        {
            if( verbose == 1 ) 
                printf("\n(D: compare c%c i%d #%d vs. c%c i%d #%d)",
                    pptCharFreqHeap[ nLeft ]->cCharacter, nLeft, pptCharFreqHeap[ nLeft ]->nFreq,
                    pptCharFreqHeap[ nRight ]->cCharacter, nRight, pptCharFreqHeap[ nRight ]->nFreq );

            
            if( pptCharFreqHeap[ nLeft ]->nFreq <= pptCharFreqHeap[ nRight ]->nFreq )
                nNewMin = nLeft;
            else
                nNewMin = nRight;
        }
        else if( nLeft != -1 )
        {
            if( verbose == 1 )
            {
                printf("\n(D:   directly nLeft)");
                printf("\n(D:     nRight; %d )", nRight );
            }
            nNewMin = nLeft;
        }
        else if( nRight != -1 )
        {
            if( verbose == 1 )
            {
                printf("\n(D:   directly nRight)");
                printf("\n(D:     nLeft; %d )", nLeft );
            }
            nNewMin = nRight;
        }
        else
        {
            if( verbose == 1 ) printf("\n(D: zero found %d)",nCurrentIndex);
            pptCharFreqHeap[ nCurrentIndex ] = 0;
            break;
        }

        if( verbose == 1 ) printf("\n(D: c%c i%d<-i%d c%c)", pptCharFreqHeap[ nCurrentIndex ]->cCharacter, nCurrentIndex,nNewMin, pptCharFreqHeap[ nNewMin ]->cCharacter );

        pptCharFreqHeap[ nCurrentIndex ] = pptCharFreqHeap[ nNewMin ];
        
        nCurrentIndex = nNewMin;

        
    }

    if( verbose == 1 ) printf("\n(D: popping end");

    return ptReturned;
}

huffNode * peekHeap(huffNode ** pptCharFreqHeap )
{
    if( pptCharFreqHeap[0] == 0 || pptCharFreqHeap[0]->nFreq == OVERMAXFREQ )
        return 0;
    return pptCharFreqHeap[0];
}


//-----------------------------------------------------

void initializeRignBuffer( ringBuffer * ptInitialized )
{
    ptInitialized->nBegin = 0;
    ptInitialized->nEnd = 0;
    ptInitialized->nSize = 0;
}

//1 == error, 0 == no error
int putToRignBuffer( huffNode * ptAdded, ringBuffer * ptQueue )
{
    int nNewEnd = (ptQueue->nEnd + 1 )%RINGBUFFERSIZE;
    if( nNewEnd == ptQueue->nBegin )
        return 1;
    ptQueue->pptTheSecondQueue[ ptQueue->nEnd ] = ptAdded;
     ptQueue->nEnd = nNewEnd;

    ptQueue->nSize++;
    return 0;
}

huffNode * getFromQueue( ringBuffer * ptQueue )
{
    int nOldBegin = ptQueue->nBegin;
    if( ptQueue->nBegin == ptQueue->nEnd )
        return 0;
    ptQueue->nBegin = ( nOldBegin + 1 )%RINGBUFFERSIZE;
    ptQueue->nSize--;
    return ptQueue->pptTheSecondQueue[ nOldBegin ];
}

huffNode * peekQueue( ringBuffer * ptQueue )
{
    return ptQueue->pptTheSecondQueue[ptQueue->nBegin];
}

