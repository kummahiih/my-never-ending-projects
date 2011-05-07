#include <malloc.h>
#include <string.h>
#include "filereader.h"


FileReader::FileReader( FILE ** fileTable, int nFiles)
{

  this->ippFilesIn = fileTable;
  this->inFilesIn =  nFiles;

  this->inError = 0;

  this->ipnReadMarks = (int*)malloc( sizeof(int) * this->inFilesIn );
  if( this->ipnReadMarks  == NULL )
    this->inError = 1;
}

FileReader::~FileReader()
{
  if( this->ipnReadMarks != NULL)
    free(this->ipnReadMarks);
}

int FileReader::toNextPhase()
{
  int nFilesLeft = 0;
  if (this->inError == 1)
    return 0;
  inTheFirst = -1;
  
  for(int i = 0; i < this->inFilesIn; i++)
  {
    if( feof( ippFilesIn[i] ) == 0 /*&& ftell( ippFilesIn[i] )  != SEEK_END && ferror( ippFilesIn[i] ) == 0 */ )
    {
      //TESTif( inTheFirst == -1 )
      //TEST inTheFirst = 0;
      ipnReadMarks[i] = i;
      nFilesLeft++;
    }
    else
    {
      //printf("\nmarked -1 %d ", i); 
      ipnReadMarks[i] = -1;
    }
    if ( ferror( ippFilesIn[i] ) != 0 )
      //perror("Error in one filestream. I don't care.");
      printf("Error in one filestream. I don't care.: %d", ferror( ippFilesIn[i] ) );
   }
  //printf("files left: %d", nFilesLeft );
  return nFilesLeft;
}

int FileReader::readCharFromSomeFile(char * toHere, int verbose)
{
  int readChar = 0;
  if( verbose!= 0 ) printf("\nreadloop %d_ ", inFilesIn);
  for(int i = 0/*TESTinTheFirst*/; i < inFilesIn /*TEST&&  ipnReadMarks[i] != -1*/; i++ )
  {
    if( verbose!= 0 ) printf("_%d %d_",i,feof(  ippFilesIn[ i ] ) );
    if( ipnReadMarks[i] != -1 )
    {
      int temp = ipnReadMarks[i];
      ipnReadMarks[i] = -1;
      
      readChar = fgetc( ippFilesIn[ i ] );
      
      if( verbose!= 0 ) printf("temp:%d ",temp );
     
      if( feof(  ippFilesIn[ i ] ) == 0 )
      {
	
	inTheFirst = i + 1;
	*toHere = readChar;
	return temp;
      }
      
    }
  }
  return -1;
}
