#include <stdio.h>

#ifndef FILESTORE
#define FILESTORE

class Filestore
{

 public:
  Filestore( char* apsOutFile, char ** appsFiles, int anInFiles, int anAtTime);
  ~Filestore();
  int getError(){ return nError; }
  FILE** getNextFiles();
  FILE * getOutFile() {return pfOutFile;}  
  int getnFilesLeft() { return nInFiles - nFirstNotOpen; }
  int getCurrentlyOpen(){ return nCurrentlyOpen;}

 private:
  void closeInFiles();
  void closeOutFile();
  

  int nError;
  int nInFiles;
 
  int nAtTime;

  char ** ppsFiles;
  
  char * psOutFile;
  
  FILE ** ppfOpenedInfiles;
  int nCurrentlyOpen;
  int nFirstNotOpen;

  FILE * pfOutFile;
};

#endif
