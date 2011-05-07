#include <stdio.h>

#define MAX_LINE_LENGTH 2048


struct line
{
  char * text;
  struct line * previous;
  int count;
};

typedef struct line line;

line * newLine( char * buff,  line * previous, int count );

line * createList( FILE * inputFile);
void deleteList( line * namelist );
char ** makeTable( line * namelist );

