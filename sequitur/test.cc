#include <stdio.h>
#include <malloc.h>

#include "parse_settings.h"
#include "filelist.h"
#include "filestore.h"
#include "filereader.h"


#include "sequitur_simple_mod.h"




int main(  int argc, char ** argv )
{
  
  settings *todo = 0;            //things to do. parsed from the user
  line * namelist = 0;           //list of names. currently it is from stdin or from command line arguments
  char ** nametable = 0;         //nametable read from a file
  Filestore * fileHandler= 0;    //opens a set of files
  FILE ** phaseFileTable = 0;    //currently open files
  filerule ** baseRules = 0;     //table for pointers to baserules
  
  int nBaseruleOfset = 0;        //the ofset of currently used rules
  int nNextFreeBaseruleSlot = 0; //the index of next free rule slot
  int nError = 0;                // != 0 if some error has occured in this function.
  int i; //the general index for fors

  int index;
  char readChar;


  slong charsRead =0;
  slong nFileIterations = 0;
  slong nFilesLeft = 666;

  todo = parseOptions( argc, argv );
  
  if( todo == 0 || todo->nHelp == 1 || (  todo->nFromStdin == 0 &&  todo->nDirectly ==0 )    )
    return 0;
  
  if( todo->nFromStdin != 0 )
  {
    namelist = createList(stdin);
    nametable = makeTable( namelist );
    todo->ppsInput = nametable;
    todo->nInput = namelist->count + 1;
    if( todo->nVerbose != 0 )
      printSetting( todo );
  }
  
  if(todo->nVerbose != 0 ) printf("D: in files %d\n",todo->nInput );
  
  fileHandler = new Filestore( todo->psOutput, todo->ppsInput, todo->nInput, todo->nOpenAtOnce );
  
  baseRules = (filerule ** )malloc( sizeof( filerule * )*( todo->nInput )  );
  if ( baseRules == 0 )
    nError = 1;
  
  for(i=0;i< todo->nInput; baseRules[i]=0, i++);
  
  printf("Gurumeditation begins.\n");
  nNextFreeBaseruleSlot = 0;
  do
  {
    
    FileReader * inputFileReader = new FileReader(  fileHandler->getNextFiles(), fileHandler->getCurrentlyOpen()  );

    if(todo->nVerbose != 0 ) 
      printf("D:files to go: %d errors: %d\n currently open: %d", fileHandler->getnFilesLeft(), fileHandler->getError(),fileHandler->getCurrentlyOpen() );

    if(todo->nVerbose != 0 ) printf("D:next files\n");
    
    nBaseruleOfset = nNextFreeBaseruleSlot;
    nNextFreeBaseruleSlot += fileHandler->getCurrentlyOpen();

    if(todo->nVerbose != 0 ) printf("D: creating new filerules\n");
    for( i = nBaseruleOfset; i < nNextFreeBaseruleSlot && nError==0; i++)
    {
      if(todo->nVerbose != 0 ) printf("D: new filerule\n");
      filerule * newone =new filerule();
      baseRules[i] = newone;
      if(  baseRules[i]==0 )
	nError = 1;
    }
    
    
    for( nFilesLeft = inputFileReader->toNextPhase(); 
	 nFilesLeft > 0 && ( maxDigramStoreSize() == -1 || digramStoreSize() < maxDigramStoreSize() ) && nError == 0; 
	 nFilesLeft = inputFileReader->toNextPhase() )
    {
      nFileIterations++;
      
      if( nFileIterations % 1024 == 0 )
      {
        printf("fle:%ld fit:%ld c:%ld ",nFilesLeft, nFileIterations , charsRead);
        printf( "d:%ld dm:%ld\n", digramStoreSize(), maxDigramStoreSize() );
      }

      for(index = inputFileReader->readCharFromSomeFile(& readChar ); 
	  index != -1 && nError == 0;
	  index  = inputFileReader->readCharFromSomeFile(& readChar)  )
      {
	if(todo->nVerbose != 0 )printf("D: read: %c from %d ofset: %d last: %d\n",readChar, index, nBaseruleOfset, nNextFreeBaseruleSlot);
	if(todo->nVerbose != 0 )printf("D: %d\n %d",baseRules[nBaseruleOfset + index], baseRules[nBaseruleOfset + index]->freq());
	if( baseRules[nBaseruleOfset + index]->insert( (readChar+256)%256, todo->nVerbose ) != 0 ) //convert a char to its ASCII value...
	  nError = 1;
        charsRead ++;
      }
      
      
    }
    
    if(todo->nVerbose != 0 )printf("D: delete fileinputreader\n");
    delete inputFileReader;
  }while( fileHandler->getnFilesLeft() > 0 && fileHandler->getError() == 0 && nError ==  0);



  for(i = 0; i < nNextFreeBaseruleSlot; i++)
        baseRules[i]->endInserting();
  if( nError == 0)
    print( fileHandler->getOutFile(), (rules**) baseRules, nNextFreeBaseruleSlot  );

  
  if( baseRules != 0 )
  {
    for( i=0; i< nNextFreeBaseruleSlot; i++)
      if(  baseRules[i] != 0)
	delete baseRules[i];
    free(baseRules);
  }
  delete fileHandler;
  deleteList( namelist );
  free(todo);
  if(nametable != 0)
    free( nametable);
  return 0;
}
