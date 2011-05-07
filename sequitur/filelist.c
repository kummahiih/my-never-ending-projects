#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "filelist.h"


line * newLine( char * buff,line * previous, int count )
{
  int i;
  int len = strlen(buff)+1;
  line * returned = (line*)malloc(sizeof(line));
  if( returned == 0) return 0;
  returned->text = (char*)malloc( sizeof(char)*(len) );
  returned->text[len] = 0;
  if( returned->text == 0  )
  {
    free( returned );
    return 0;
  }
  
  for(i=0;i<len&& buff[i] != 0; i++)
  {
    if( buff[i] != 0xa && buff[i] != 0xd )
    {
       returned->text[i] = buff[i];
    }
    else
      returned->text[i] = 0;
   }
  returned->previous = previous;
  returned->count = count;
  return returned;
}

line * createList( FILE * inputFile)
{
  line * previous_line = 0;
  char buff[MAX_LINE_LENGTH];
  int i;
  char * luettu= 0;
  for(i=0;i<MAX_LINE_LENGTH;buff[i]=0,i++);
  
  for( i=0;;i++) 
  {
    luettu = fgets(buff, MAX_LINE_LENGTH, inputFile);
    if(luettu == 0)
      break;
    previous_line = newLine(buff, previous_line, i);
    if(  previous_line == 0 )
    {
      deleteList( previous_line );
      return 0;
    }
  }
  return  previous_line;
}


void deleteList( line * namelist )
{
  
  line * previous = namelist; 
  line * deleted = namelist;
  while( previous != 0 )
  {
    deleted = previous;
    previous = previous->previous;
    
    if( deleted->text != 0 )
    {

      //printf("%s\n",deleted->text);
      free( deleted->text );
    }
    
    free( deleted );
  }
}

char ** makeTable( line * namelist )
{
  char ** stringTable;
  int i;
  int count = namelist->count+1;
  if( namelist == 0 || count == 0)
    return 0;
  stringTable = ( char **)malloc( count*sizeof(char*));
  if( stringTable == 0)
    return 0;
  for( i=count-1;i >= 0 && namelist !=0; i--, namelist = namelist->previous)
  {
    stringTable[i] = namelist->text;
  }
  return stringTable;
}
