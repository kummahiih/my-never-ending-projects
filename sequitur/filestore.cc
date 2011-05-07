#include <stdio.h>
#include <malloc.h>
#include "filestore.h"

Filestore::Filestore( char* apsOutFile, char ** appsFiles, int anInFiles, int anAtTime )
{
  nError = 0;
  nInFiles = anInFiles;
  psOutFile = apsOutFile;

 
  ppsFiles = appsFiles;
 
  psOutFile = apsOutFile;
  nAtTime = anAtTime;
  ppfOpenedInfiles =  (FILE**)malloc(sizeof(FILE*)*nAtTime);
  if( ppfOpenedInfiles == 0 )
    nError = 1;
  nFirstNotOpen = 0;
  pfOutFile = fopen(apsOutFile,"wb");
  if( pfOutFile == 0 )
    nError = 1;

}

Filestore::~Filestore()
{
  closeInFiles();
  closeOutFile();

  if( ppfOpenedInfiles != 0 )
    free( ppfOpenedInfiles );
}

void Filestore::closeInFiles()
{
  int i;
  if( ppfOpenedInfiles != 0 )
    for(int i=0;i<nAtTime && ppfOpenedInfiles[i] != 0; i++)
    {
      fclose(ppfOpenedInfiles[i]);
      ppfOpenedInfiles[i] = 0;
      
    }
}

void Filestore::closeOutFile()
{
  if(pfOutFile != 0)
    fclose(pfOutFile);
}

FILE ** Filestore::getNextFiles()
{
  int i;
  int nOpenFailed = 0;
  closeInFiles();
  nCurrentlyOpen = 0;
  for( i=0; i - nOpenFailed < nAtTime && nFirstNotOpen + i <  nInFiles &&  nError == 0; i++)
  {
    ppfOpenedInfiles[ i - nOpenFailed] = fopen( ppsFiles[nFirstNotOpen + i ], "rb" );
    if(  ppfOpenedInfiles[ i - nOpenFailed] == 0)
    {
      perror("Bad file. Trying next one.. ");
      nOpenFailed++;
    }
    else
      nCurrentlyOpen++;
  }
  nFirstNotOpen = nFirstNotOpen + i;
  return ppfOpenedInfiles;
}
