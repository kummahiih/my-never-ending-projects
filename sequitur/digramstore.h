#include <iostream>
#include <map>
#include <assert.h>

using namespace std;

class symbols;
class rules;



class insertpoint;

typedef unsigned long  ulong;

typedef signed long  slong;

#define LAST_CHAR 255
#define INSERT_SYMBOL ( LAST_CHAR + 1 )
#define FIRST_RULE ( LAST_CHAR+2 )
#define RULE_TABLE_START (FIRST_RULE + 1 )

struct digram
{
    ulong first;
    ulong second;
};
typedef struct digram digram;

struct digramCmp {
    bool operator()( const digram s1, const digram s2 ) const {
      return s1.first < s2.first || ( s1.first == s2.first && s1.second < s2.second );
    }
};


//returns 0 if not found
symbols *getDigram( slong first, slong second , int verbose = 0);

//TODO: check max digrams elsewhere

void updateDigram( slong first, slong second, symbols * stored );

void setDigram( slong first, slong second, symbols * stored , int verbose = 0);

void deleteDigram( slong first, slong second, symbols * deleted = 0 );

slong digramStoreSize();

slong maxDigramStoreSize();

// pick a prime! (large enough to hold every digram in the grammar, with room
// to spare

//#define PRIME 2265539
//#define PRIME  62265551
// 62265551 * 0.8 = 49812440  
//                  4294967296
//                  67108864 == 2^26
// Standard open addressing or double hashing. See Knuth.
//.. chance this static hash-table to STL's map ...

//#define PRIME    62265551

                //268435456


//FIX
//#define HASH(one, two) (((one) << 27 | (two)) % PRIME)
//#define HASH2(one) (17 - ((one) % 17))

//#define MAXDIGRAMS 67108864
//extern slong nDigrams;

//symbols **find_digram(symbols *s);


