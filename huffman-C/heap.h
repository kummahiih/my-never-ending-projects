#define BLOCKSIZE 512
#define OVERMAXFREQ ( BLOCKSIZE + 1 )
#define FIRSTCHAR 32
#define LASTCHAR 126

#define CHARS (LASTCHAR - FIRSTCHAR + 1)
#define RINGBUFFERSIZE CHARS + 5



struct huffNode
{
  char cCharacter;
  int nFreq;
  struct huffNode * ptZeroChild; //if these are zero this is a leaf
  struct huffNode * ptOneChild;
  struct huffNode * ptParent;

};

typedef struct huffNode  huffNode;

struct ringBuffer
{
  int nBegin;
  int nEnd;
  int nSize;
  huffNode * pptTheSecondQueue[ RINGBUFFERSIZE ];
};
typedef struct ringBuffer  ringBuffer;

struct huffCodes
{
  huffNode ptCharFreq[CHARS];       //the leaf nodes of the huffman-tree
  huffNode * pptCharFreqHeap[CHARS];
  huffNode * ptHuffRoot;
  int nReadChars;
  char psReadChars[BLOCKSIZE+5];
  huffNode  ptNotLeafs[RINGBUFFERSIZE];
  int nNextNotleaf;
};
typedef struct huffCodes  huffCodes;




int indexOf( char cReaded );
void  InitializeHuffNodeArray( huffNode * ptCharFreq, int verbose );
void  InitializeHuffNodeArrayForMidleNodes( huffNode * ptCharFreq, int verbose );
void resetHuffNodeArray( huffNode * ptCharFreq );
void printHeap( huffNode ** pptCharFreqHeap );
huffNode ** createPointerHeap( huffNode * ptCharFreq, huffNode ** pptCharFreqHeap , int verbose );
huffNode * popPointerHeap( huffNode ** pptCharFreqHeap, int verbose  );
huffNode * peekHeap(huffNode ** pptCharFreqHeap );


void initializeRignBuffer( ringBuffer * ptInitialized );
int putToRignBuffer( huffNode * ptAdded, ringBuffer * ptQueue );
huffNode * getFromQueue( ringBuffer * ptQueue );
huffNode * peekQueue( ringBuffer * ptQueue );

