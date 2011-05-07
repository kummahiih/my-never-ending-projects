#include <stdio.h>
#include <string.h>
#include <math.h>
#include "parse_settings.h"
#include "heap.h"

huffCodes GlobalHuffCodes;
ringBuffer tTheSecondQueue;

//returns 1, if nothing to read or if an error occured
//else return 0
int readEncoded( int verbose )
{
  FILE * pfParsed = stdin;
  int i,index;
  char cReaded;
  
  huffNode * ptCharFreq = GlobalHuffCodes.ptCharFreq;

  if( verbose == 1 )
      printf("\n(D: readEncoded: )");

  if( feof( pfParsed) != 0 )
    return 1;

  for( i = 0; i < BLOCKSIZE  && feof( pfParsed) == 0; i++ )
  {
    cReaded = fgetc(pfParsed);
    index = indexOf(cReaded);

    if( verbose ) printf("\n(D: read character:  %c %d %d", cReaded, cReaded ,  index );
    
    if( cReaded == -1 ) break; //EOF

    GlobalHuffCodes.psReadChars[i] = cReaded;
    
    if( cReaded > LASTCHAR || cReaded < FIRSTCHAR )
    {
      
      fprintf( stderr, "\nError: Illegal character: %c %d", cReaded, cReaded );
      return 1;
    }
    else
    {
        if(verbose == 1)
            printf("\n(D: Read: %c %d %d )", cReaded, cReaded, index );
        if( ptCharFreq[ index ].nFreq >= OVERMAXFREQ )
             ptCharFreq[ index ].nFreq = 1;
        else
            ptCharFreq[ index ].nFreq++;
    }
  }

  GlobalHuffCodes.nReadChars = i;

  if(verbose == 1)
       for( i = 0; i < CHARS; i++)
            printf("\n(D: %d %c %d %d )", i, ptCharFreq[i].cCharacter, ptCharFreq[i].cCharacter, ptCharFreq[i].nFreq );


  return 0;
}


/*

         1. Remove two nodes with the lowest weight from the queues.
*/

huffNode * minFromQueues(int verbose)
{
    huffNode * ptFirstLow = 0;
    huffNode ** pptCharFreqHeap = GlobalHuffCodes.pptCharFreqHeap;
    huffNode * peekedFromHeap;
    huffNode * peekedFromRingBuffer;

    if( verbose == 1 ) printf("\n(D: peekHeap)");

    peekedFromHeap = peekHeap( pptCharFreqHeap );

    if( verbose == 1 ) printf("\n(D: peekQueue)");

    peekedFromRingBuffer = peekQueue(  &tTheSecondQueue );

    if( verbose == 1 )
        printf("\n(D: pointers: %x %x)", peekedFromHeap, peekedFromRingBuffer );

    if( peekedFromHeap != 0 && 
        ( tTheSecondQueue.nSize == 0 || peekedFromRingBuffer->nFreq > peekedFromHeap->nFreq  ) )
    {
        if( verbose == 1 ) printf("\n(D:from heap)");
        ptFirstLow = popPointerHeap( pptCharFreqHeap , verbose );
    }
    else
    {
        if( verbose == 1 ) printf("\n(D:from ringbuffer)");
        ptFirstLow = getFromQueue( &tTheSecondQueue );
    }
  return ptFirstLow;
}

/*
Creating the tree:

   1. Start with as many leaves as there are symbols.

   2. Queue all leaf nodes into the first queue (in order).

   3. While there is more than one node in the queues:

         1. Remove two nodes with the lowest weight from the queues.

         2. Create a new internal node, with the two just-removed nodes as children 
            (either node can be either child) and the sum of their weights as the new weight.

         3. Update the parent links in the two just-removed nodes to point to the just-created parent node.

         4. Queue the new node into the second queue.

   4. The remaining node is the root node; the tree has now been generated.
*/

huffNode * getNewNode( int verbose )
{
    huffNode * ptNewNode = &GlobalHuffCodes.ptNotLeafs[ GlobalHuffCodes.nNextNotleaf ];
    GlobalHuffCodes.nNextNotleaf++;

    if( verbose == 1 ) printf("\n(D:new nodeaddress fetched: %d )", ptNewNode);

    if( GlobalHuffCodes.nNextNotleaf > RINGBUFFERSIZE + 1 )
    {
        fprintf(stderr,"\nError: too many not leaf nodes\n");
        if( verbose == 1 ) printf("\n(D: %d %d)", GlobalHuffCodes.nNextNotleaf, CHARS );
        return 0;
    }
    return ptNewNode;
}

huffNode * makeHuffman( int verbose )
{
  huffNode * ptCharFreq;

  huffNode * ptFirstLow = 0;
  huffNode * ptSecondLow = 0;

  huffNode * ptNewNode =0;

  huffNode * ptHuffRoot = 0;

  //huffNode * ptNewNode;
  huffNode ** pptCharFreqHeap;

  ptCharFreq = GlobalHuffCodes.ptCharFreq;

  pptCharFreqHeap  =  createPointerHeap( ptCharFreq, GlobalHuffCodes.pptCharFreqHeap, verbose );

  initializeRignBuffer( &tTheSecondQueue );

  GlobalHuffCodes.nNextNotleaf = 0;
  
  //3. While there is more than one node in the queues:
  while( !( peekHeap( pptCharFreqHeap ) == 0 && tTheSecondQueue.nSize == 1)  )
  {
    //1. Remove two nodes with the lowest weight from the queues.
    if( verbose == 1 ) printf("\n(D:1)");

    ptFirstLow = minFromQueues( verbose);

    if( verbose == 1 ) printf("\n(D:1 %c %d %d)",ptFirstLow->cCharacter,ptFirstLow->cCharacter,ptFirstLow->nFreq );

    if( verbose == 1 ) printf("\n(D:2)");

    ptSecondLow = minFromQueues( verbose);

    if( verbose == 1 ) printf("\n(D: initializing)");

    if( ptSecondLow != 0)
    {
        if( verbose == 1 ) printf("\n(D:2 %c %d %d)\n",ptSecondLow->cCharacter,ptSecondLow->cCharacter,ptSecondLow->nFreq );
        //2. Create a new internal node, with the two just-removed nodes as children 
        //(either node can be either child) and the sum of their weights as the new weight.
        ptNewNode = getNewNode(  verbose );

        if( ptNewNode == 0)
            return 0;

        ptNewNode->cCharacter = 0;
        ptNewNode->nFreq = ptFirstLow->nFreq + ptSecondLow->nFreq;
        ptNewNode->ptOneChild = ptFirstLow;
        ptNewNode->ptZeroChild =  ptSecondLow; //zeroes are less expensive :)
        ptNewNode->ptParent = 0;

        if( verbose == 1 ) printf("\n(D:new node initialized.)");

        //3. Update the parent links in the two just-removed nodes to point to the just-created parent node.
        ptFirstLow->ptParent = ptNewNode;
        ptSecondLow->ptParent = ptNewNode;
    }
    else
    {
        ptNewNode = getNewNode(  verbose );
        if( ptNewNode == 0)
            return 0;
        
        ptNewNode->ptZeroChild = ptFirstLow;
        ptNewNode->ptOneChild = 0;
        ptFirstLow->ptParent = ptNewNode;

        ptNewNode->nFreq = ptFirstLow->nFreq;
        ptNewNode->cCharacter = 0;

    }
     
     //4. Queue the new node into the second queue.
     if( putToRignBuffer(ptNewNode, &tTheSecondQueue ) == 1 )
     {

        fprintf(stderr,"\nError: ringbuffer full\n");
        if( verbose == 1 ) { printf("\nD: size:%d max:%d )", tTheSecondQueue.nSize, RINGBUFFERSIZE ); }

        return 0;
     }
     if( verbose == 1 ) printf("\n(D: iteration end)");
  }

  ptHuffRoot = getFromQueue( &tTheSecondQueue );  

  ptHuffRoot->ptParent = 0;
  GlobalHuffCodes.ptHuffRoot = ptHuffRoot;

  if( verbose == 1 ) printHeap( GlobalHuffCodes.pptCharFreqHeap ) ;

  return ptHuffRoot;
}


void huffTreePrint( huffNode * ptHuffRoot, int n )
{
    int i;
    if( ptHuffRoot->ptZeroChild != 0 )
    {
        printf("\nD:");
        for(i=0;i<n;i++)
            printf("|");
        printf(".0 %d)",ptHuffRoot->ptZeroChild->nFreq);
        huffTreePrint( ptHuffRoot->ptZeroChild,  n +1 );
    }
    if( ptHuffRoot->ptOneChild != 0 )
    {
        printf("\nD:");
        for(i=0;i<n;i++)
            printf("|");
        printf(".1 %d)",ptHuffRoot->ptOneChild->nFreq);
        huffTreePrint( ptHuffRoot->ptOneChild,  n +1 );
    }
    if( ptHuffRoot->cCharacter != 0 )
    {
        printf("\nD:");
        for(i=0;i<n;i++)
            printf("|");
        printf(".%c %d %d)", ptHuffRoot->cCharacter, ptHuffRoot->cCharacter, ptHuffRoot->nFreq);
    }
}


void printHuffTree()
{
    printf("\nD:Huffman tree: %d)",GlobalHuffCodes.ptHuffRoot->nFreq);
    huffTreePrint( GlobalHuffCodes.ptHuffRoot, 1 );
    printf("\nD:-------)\n");
}

void printHuffCodeRecursion(huffNode * ptHuffLeaf, huffNode * ptChild )
{
    if( ptHuffLeaf == 0)
        return;
    if( ptChild == 0 )
        printHuffCodeRecursion(ptHuffLeaf->ptParent, ptHuffLeaf );
    else
    {
        printHuffCodeRecursion(ptHuffLeaf->ptParent, ptHuffLeaf );
        if( ptChild == ptHuffLeaf->ptOneChild )
            printf("1");
        else  
            printf("0");
    }       

}

void printHuffCode( int index )
{
    if( GlobalHuffCodes.ptCharFreq[index].ptParent == 0 )
        return;

    printf("\n%c ", GlobalHuffCodes.ptCharFreq[index].cCharacter );
    printHuffCodeRecursion( &GlobalHuffCodes.ptCharFreq[index], 0 );
}

int printJustHuffCode( int index )
{
    if( GlobalHuffCodes.ptCharFreq[index].ptParent == 0 )
    {
        fprintf(stderr,"\nError: can not translate '%c' index %d\n", GlobalHuffCodes.ptCharFreq[index].cCharacter, index );
        fprintf(stderr,"\n       parent: %d freq: %d ", GlobalHuffCodes.ptCharFreq[index].ptParent, GlobalHuffCodes.ptCharFreq[index].nFreq );
        printHuffTree();
        printHeap( GlobalHuffCodes.pptCharFreqHeap );
        return 1;
    }
    else
        printHuffCodeRecursion( &GlobalHuffCodes.ptCharFreq[index], 0 );
    return 0;
}

//0 == success
int createPath( char * pcTemp, int nEndOfBinary, int verbose)
{
    int i;
    huffNode * ptCurrent = GlobalHuffCodes.ptHuffRoot;
    huffNode * ptNew = 0;

    int index = indexOf( pcTemp[0] );

    if( verbose == 1 )
       printf("\n(D: create path begin: string:%s)", pcTemp);

    if( GlobalHuffCodes.ptCharFreq[ index ].ptParent != 0 )
        return 1;

    
    for( i = 2; i < nEndOfBinary; i++ )
    {
        if( verbose == 1 )
            printf("\n(D:c%c)\n", pcTemp[i]);
        if( pcTemp[i] == '0' )
        {
            if( ptCurrent->ptZeroChild == 0 )
            {
                ptNew = getNewNode(  verbose );
                if( ptNew == 0) 
                    return 1;
                ptCurrent->ptZeroChild = ptNew;
                ptNew->ptParent = ptCurrent;
                ptNew->nFreq = 0;
                ptNew->cCharacter = 0;
                ptNew->ptOneChild = 0;
                ptNew->ptZeroChild = 0;
            }
            else if( ptCurrent->ptZeroChild->cCharacter != 0 )
                return 1;
            ptCurrent = ptCurrent->ptZeroChild;
        }
        else
        {
            if( ptCurrent->ptOneChild == 0 )
            {
                ptNew = getNewNode(  verbose );
                if( ptNew == 0) 
                    return 1;
                ptCurrent->ptOneChild = ptNew;
                ptNew->ptParent = ptCurrent;
                ptNew->nFreq = 0;
                ptNew->cCharacter = 0;
                ptNew->ptOneChild = 0;
                ptNew->ptZeroChild = 0;
            }
            else if( ptCurrent->ptOneChild->cCharacter != 0 )
                return 1;
            ptCurrent = ptCurrent->ptOneChild;
        }
    }

    if( pcTemp[nEndOfBinary ] == '0' )
    {
        ptCurrent->ptZeroChild = &GlobalHuffCodes.ptCharFreq[ index ];
        GlobalHuffCodes.ptCharFreq[ index ].ptParent = ptCurrent;
    }
    else
    {
        ptCurrent->ptOneChild = &GlobalHuffCodes.ptCharFreq[ index ];
        GlobalHuffCodes.ptCharFreq[ index ].ptParent = ptCurrent;
    }

    return 0;
}

//0 == success
int parseHuffmanPairs(int verbose )
{
    char pcTemp[CHARS + 5 ];
    char cCodedChar;

    int nEndOfBinary;
    
    if( verbose == 1 )
        printf("\n(D: Parse pairs: )");
    
    if( fgets(pcTemp, CHARS + 5, stdin) == 0 || feof( stdin) != 0 ||  strncmp("Pairs:\n",pcTemp, 7) != 0 )
    {
        fgetc(stdin);
        return 1;
    }

    while( fgets(pcTemp, CHARS + 5, stdin) != 0 && feof( stdin) == 0 )
    {
        if( strncmp("Data:\n",pcTemp, 6) == 0 )
        {
            if( verbose == 1 )
                printf("\n(D: \"Data:\"-string found so all pairs are now parsed)");
            return 0;
        }
        cCodedChar = pcTemp[0];

        if( verbose == 1 )
            printf("\n(D: %c %d)",cCodedChar,cCodedChar);

        if( cCodedChar > LASTCHAR || cCodedChar <  FIRSTCHAR ) //btw. this checks if the string is empty
            return 1;

        if( pcTemp[1] != ' ' )
            return 1;
    
        for( nEndOfBinary = 2; nEndOfBinary < CHARS + 5 && pcTemp[nEndOfBinary] != 0; nEndOfBinary ++)
            if( ! ( pcTemp[nEndOfBinary] == '0' || pcTemp[nEndOfBinary] == '1' || pcTemp[nEndOfBinary] == '\n') )
                return 1;
        if( pcTemp[nEndOfBinary -1] != '\n' )
            return 1;

        nEndOfBinary-=2;

        if( nEndOfBinary == 1 )
            return 1;

        if( createPath( pcTemp, nEndOfBinary, verbose ) != 0 )
        {
            fprintf(stderr, "\nError: huffman-code-path creating error.");
            return 1;
        }

    }
    return 1;
}

//0 --> no errors ( success )
int checHuffmanTreeCFS( huffNode * ptCurrent ,int verbose )
{
    if( ptCurrent->cCharacter != 0 && ptCurrent->ptOneChild == 0 && ptCurrent->ptZeroChild == 0)
        return 0;
    else if ( ptCurrent->cCharacter != 0 )
    {
        if( verbose == 1 ) printf("\n(D: char with leafs c%c)",ptCurrent->cCharacter);
        return 1;
    }
    else if( ptCurrent->ptOneChild == 0  || ptCurrent->ptZeroChild == 0 )
    {
        if( verbose == 1 ) printf("\n(D: node that has no childs )");
        return 1;
    }
    else if( checHuffmanTreeCFS( ptCurrent->ptZeroChild,verbose ) == 0 && checHuffmanTreeCFS( ptCurrent->ptOneChild,verbose ) == 0 )
        return 0;
    else
        return 1;
}

//0 --> no errors ( success )
int checkHuffmanCodes( int verbose )
{
    if( GlobalHuffCodes.nNextNotleaf == 1 && ( 
        GlobalHuffCodes.ptHuffRoot->ptZeroChild != 0 && GlobalHuffCodes.ptHuffRoot->ptOneChild == 0 ||
        GlobalHuffCodes.ptHuffRoot->ptZeroChild == 0 && GlobalHuffCodes.ptHuffRoot->ptOneChild != 0 
        ) )
        return 0;

    return checHuffmanTreeCFS( GlobalHuffCodes.ptHuffRoot, verbose );
}

//0 == success
int decodeReadData( char * pcTemp, int verbose )
{
    int i;
    huffNode * ptCurrent = GlobalHuffCodes.ptHuffRoot;

    if( verbose == 1 )
        printf("\n(D: Decoding:%s:", pcTemp );

    for( i=0; pcTemp[i] != 0; i++)
    {
        if( verbose == 1 )
            printf("%c",pcTemp[i]);

        if( pcTemp[i] == '0' )
        {
            if( ptCurrent->ptZeroChild == 0 )
                return 1;
            ptCurrent = ptCurrent->ptZeroChild;
        }
        else if( pcTemp[i] == '1' )
        {
            if( ptCurrent->ptOneChild == 0 )
                return 1;
            ptCurrent = ptCurrent->ptOneChild;
        }
        
    }
    if( verbose == 1 )
        printf(")");
    if( ptCurrent->cCharacter == 0 )
        return 1;
    else printf("%c",ptCurrent->cCharacter);

    return 0;
}

//0 == success
int parseEncodedData( int verbose )
{
    int i=0;
    char cRead;
    char pcTemp[CHARS + 5 ];
    int nSpaceFound = 0;
    int nBinaryFound = 0;
    int nBeginOfData = 1;

    if( verbose == 1 )
        printf("\n(D: Data parsing starts)");

    while( feof( stdin) == 0 )
    {
        cRead = getc(stdin);

        if( verbose == 1 )
            printf("\n(D: %c %d)", cRead, cRead );

        if( cRead == -1 )//EOF
        {
            if( nBinaryFound == 0 && nBeginOfData == 1 )
                return 1;
            else
                return 0;
        }

        if( cRead != '0' && cRead != '1')
        {
            if( cRead == ' ')
            {
                if( verbose == 1 )
                    printf("\n(D: space found)");
                pcTemp[i] = 0;
                nSpaceFound = 1;
            
                if( nBeginOfData == 1 )
                {
                    fprintf(stderr,"\nError: there should not be a space before the data");
                    return 1;
                }
            }
            else if( cRead == '\n')
            {
                if( verbose == 1 ) printf("\n(D: newline found)", cRead, cRead );
                if( nSpaceFound == 0 && nBinaryFound == 0)
                {
                    if( verbose == 1 ) printf("\n(D: end of data)", cRead, cRead );
                    return 0;
                }
                fprintf(stderr,"\nError: there should not be a newline in the midle of code ( terminate with space)");
                return 1;
            }
        }
        else
        {
            nBinaryFound = 1;
            pcTemp[i] = cRead;
            i++;
            
            if( verbose == 1 )
                printf("\n(D: accepted: %c %d)", cRead, cRead );


        }

        nBeginOfData = 0;

        if( i > CHARS  ) // maximum length of encoded data
            return 1;

        if( nSpaceFound == 1 && nBinaryFound == 1)
        {
            if( decodeReadData(pcTemp, verbose ) != 0 )
            {
                fprintf(stderr, "\nError: decoding error");
                return 1;
            }
            i = 0;
            nSpaceFound = 0;
            nBinaryFound = 0;
        }
    }

    if( verbose == 1 )
        printf("\n(D: this point of code should not be reached)" );
    
    return 1;
}



//0 == success
int decodeBlock( int verbose )
{
    GlobalHuffCodes.nReadChars = 0;



    if( parseHuffmanPairs(verbose ) != 0 )
    {
         if( feof( stdin) == 0 )
            fprintf(stderr, "\nError: huffman-pairs parsing error");
        return 1;
    }



    if( checkHuffmanCodes(verbose ) != 0 )
    {
        if( verbose ) printHuffTree();
        fprintf(stderr, "\nError: huffman-tree-check");
        return 1;
    }

    if( verbose )
        printHuffTree();

    if( parseEncodedData(verbose ) != 0 )
    {
        fprintf(stderr, "\nError: parsing or decoding error");
        return 1;
    }

    GlobalHuffCodes.nReadChars = 1;
    return 0;
}

void resetRootNode()
{
  GlobalHuffCodes.ptHuffRoot = &GlobalHuffCodes.ptNotLeafs[ 0 ];
  GlobalHuffCodes.nNextNotleaf = 1;

  GlobalHuffCodes.ptHuffRoot->ptParent = GlobalHuffCodes.ptHuffRoot;
  GlobalHuffCodes.ptHuffRoot->cCharacter = 0;
}

int main( int argc, char **argv )
{
  settings * ptHuffTodo = 0;
  int verbose;
  int i;
  int nBlocks = 0;
  ptHuffTodo = parseOptions( argc, argv );


  if( ptHuffTodo == 0 )
    return 1;
  verbose = ptHuffTodo->nVerbose;


  if( verbose == 1 )
      printf("\n(D: begin ...)");

  if( ptHuffTodo->nEncode )
  {
    InitializeHuffNodeArray( GlobalHuffCodes.ptCharFreq, verbose );
    
    while( readEncoded( verbose ) == 0 )
    {
        if( GlobalHuffCodes.nReadChars == 0 && nBlocks == 0)
        {
            fprintf(stderr,"\nError: No data to encode.");
            break;
        }

        if( makeHuffman( verbose ) == 0 )
        {
            fprintf(stderr,"\nError: Huffman-coding error");
            break;
        }

        if( verbose )
            printHuffTree();

        if( ptHuffTodo->nWords )
        {
            printf("Pairs:");
            for( i = 0; i < CHARS; i++)
                printHuffCode(i);
        }
        if( ptHuffTodo->nCode )
        {
            printf("\nData:\n");
            for( i = 0; i < GlobalHuffCodes.nReadChars ; i++)
            {
                if( printJustHuffCode(indexOf(GlobalHuffCodes.psReadChars[i])) != 0 )
                    return 1;
                printf(" ");
            }
            printf("\n");
        }

        resetHuffNodeArray(GlobalHuffCodes.ptCharFreq);
        nBlocks ++;
    }
    if( nBlocks == 0 || nBlocks != 0 && GlobalHuffCodes.nReadChars != 0 ) //ugly fix
        return 1;
  }
  if( ptHuffTodo->nDecode )
  {
      InitializeHuffNodeArray( GlobalHuffCodes.ptCharFreq, verbose );

      InitializeHuffNodeArrayForMidleNodes( GlobalHuffCodes.ptNotLeafs, verbose );
      resetRootNode();

      if( verbose == 1 )
        printf("\n(D: Decode)");
      while( decodeBlock( verbose ) == 0 )
      {
        if( verbose == 1 )
            printf("\n(D: Next block)");

        InitializeHuffNodeArrayForMidleNodes( GlobalHuffCodes.ptNotLeafs, verbose );
        resetHuffNodeArray(GlobalHuffCodes.ptCharFreq);
        resetRootNode();
        nBlocks ++;
      }
      if( nBlocks == 0 || nBlocks != 0 && GlobalHuffCodes.nReadChars != 0 ) //ugly fix
         return 1;
  }
  return 0;
}
