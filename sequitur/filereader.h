#include <stdio.h>
#ifndef SEQUITUR_FILEREADER
#define SEQUITUR_FILEREADER


class FileReader
{
 public:
  FileReader(  FILE ** fileTable, int nFiles );
  ~FileReader();
  int toNextPhase();
  int readCharFromSomeFile(char * toHere, int verbose = 0);
 private:
  int inError;
  FILE ** ippFilesIn;
  int * ipnReadMarks;
  int     inFilesIn;
  int inTheFirst;
};
#endif
