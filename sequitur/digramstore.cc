#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <map>


#include "digramstore.h"

using namespace std;

//slong nDigrams = 0;

//symbols *table[PRIME] = {0};

bool compareDigrams( digram one, digram two )
{
    return one.first < two.first || ( one.first == two.first && one.second < two.second );

}

map< digram, symbols * , digramCmp> symbolsMap;

symbols *getDigram( slong first, slong second, int verbose )
{
    digram key;
    key.first = first;
    key.second = second;

    if( verbose != 0 )
      printf("D: get digram: %ld %ld\n", first, second );

    map<digram, symbols *, digramCmp >::iterator iter = symbolsMap.find(key);

  if( iter != symbolsMap.end() ) return iter->second;
  else return 0;
}


void updateDigram( slong first, slong second, symbols * stored )
{
    //printf("update digram %ld %ld\n", first, second ); 
    setDigram( first, second, stored );
}

//TODO: check max digrams elsewhere
void setDigram( slong first, slong second, symbols * stored, int verbose )
{

    digram key;
    key.first = first;
    key.second = second;

    if( verbose != 0 )                                                                                              
      printf("D: set digram %ld %ld\n", first, second );   



    int debug = 0;
    if(!( first > 0 && second > 0 && stored > 0))
        debug = 1;

    if( getDigram(first, second) != 0 )
        debug = 1;

    //printf("set digram %ld %ld\n", first, second );
    symbolsMap[key] = stored;
}

void deleteDigram( slong first, slong second, symbols * deleted )
{
    digram key;
    key.first = first;
    key.second = second;
    

    if( deleted == 0 )
    {
      //printf("delete digram without checking %ld %ld\n", first, second );
      symbolsMap.erase( key );
      return;
    }
    else
    {
      
        map<digram, symbols *, digramCmp >::iterator iter = symbolsMap.find(key);
        if( iter != symbolsMap.end() && iter->second == deleted )
	{
	  //printf("delete digram (checked) %ld %ld\n", first, second );
	  symbolsMap.erase( iter );
	}
    }   
}

slong digramStoreSize()
{
    return symbolsMap.size();
}

slong maxDigramStoreSize()
{
    return symbolsMap.max_size();
}



/*symbols **find_digram(symbols *s)
{
    return 0;
}*/


/*symbols **find_digram(symbols *s)
{
  ulong one = s->raw_value();
  ulong two = s->next()->raw_value();

  slong jump = HASH2(one);
  slong insert = -1;
  slong i = HASH(one, two);

  while (1) {
    symbols *m = table[i];
    if (!m) {
      nDigrams++;
      if (insert == -1) insert = i;
      return &table[insert];
    } 
    else if (slong(m) == 1) insert = i;
    else if (m->raw_value() == one && m->next()->raw_value() == two) 
      return &table[i];
    i = (i + jump) % PRIME;
  }
}*/



//void remove_digram()

