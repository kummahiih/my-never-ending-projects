#include <malloc.h>
#include <string.h>
#include "cmd_line.h"
#include "filereader.h"

#include "sequitur_simple_mod.h"

extern map< digram, symbols * , digramCmp> symbolsMap;

void printDigrams()
{
   map<digram, symbols *, digramCmp >::const_iterator iter;
   symbols * printed;
    for (iter=symbolsMap.begin(); iter != symbolsMap.end(); ++iter) 
   {
        printed = (symbols *)(iter->second);
        slong one = printed->value();
        slong two = printed->next()->value();
        
        if( printed->nt() )  one = printed->rule()->index() + RULE_TABLE_START ;
        if( printed->next()->nt() ) two = printed->next()->rule()->index() + RULE_TABLE_START ;

        cout << "(";
        if( printed->prev()->is_guard() ) 
            cout << "A" << printed->prev()->rule()->index() + RULE_TABLE_START;
        else if( printed->prev()->nt() )
            cout << "R"<< printed->prev()->rule()->index() + RULE_TABLE_START;
        else
            cout << "D"<< printed->prev()->value();

        cout <<" "<< one <<","<< two << " ";

        if( printed->next()->next()->is_guard() ) 
            cout << "A" << printed->next()->next()->rule()->index() + RULE_TABLE_START;
        else if(  printed->next()->next()->nt() )
            cout << "R"<< printed->next()->next()->rule()->index() + RULE_TABLE_START;
        else
            cout << "D"<< printed->next()->next()->value();
            
        cout <<") ";
    }
    cout<<endl;

}


int main(  int argc, char ** argv )
{
  //cout << "Aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  Settings *settings = new Settings( argc, argv );
  printf("infiles %d errors %d\n",settings->GetInFileTableSize(), settings->GetZeroIfNoError() );
  filerule ** baseRules = NULL;
  slong charsRead =0;
  slong nFileIterations = 0;
  slong nFilesLeft = 0;
  int i;

  if( settings->GetVerbose() == 1)
  {
    printf("\nVerbose mode on\n");
    settings->PrintSettings();
  }
  while(1)
  {
    if( settings->GetZeroIfNoError() != 0)
      break;

    FileReader * inputFileReader= new FileReader( settings->GetInFileTable(), settings->GetInFileTableSize() );

    baseRules = (filerule ** )malloc( sizeof( filerule * )* settings->GetInFileTableSize()  );

    if( baseRules == NULL )
      break;

    for(i = 0; i < settings->GetInFileTableSize(); i++)
        baseRules[i] =  new filerule();

    // max amount of digrams in digram table <= (read chars) - number of files - paranoia
    charsRead = 0;//-settings->GetInFileTableSize() - 71551; 

    char a = 0;
    int b = 0;
    printf("Gurumeditation\n");
    if( settings->GetVerbose() == 1) 
    printf(" max digrams: %ld", maxDigramStoreSize() );
    for( nFilesLeft = inputFileReader->toNextPhase(); nFilesLeft > 0 && ( maxDigramStoreSize() == -1 || digramStoreSize() < maxDigramStoreSize() ); nFilesLeft = inputFileReader->toNextPhase())
    {
      nFileIterations++;
      if( settings->GetVerbose() == 1)
      {
          print( stdout, (rules**) baseRules, settings->GetInFileTableSize() );
          printf("----\n");
          printDigrams();
          printf("new fileiteration round  begins -----------------------------\n");
	  printf("files left: %d \n", nFilesLeft);
      }

      if( nFileIterations % 1024 == 0 )
      {
        printf("fle:%ld fit:%ld c:%ld ",nFilesLeft, nFileIterations , charsRead);
        printf( "d:%ld dm:%ld\n", digramStoreSize(), maxDigramStoreSize() );
      }
      
      
      for(b = inputFileReader->readCharFromSomeFile(&a); b != -1 ;b  = inputFileReader->readCharFromSomeFile(&a)  )
      {
          
        charsRead ++;
        if( settings->GetVerbose() == 1)
            printf("---------\nadding character %d to the grammar %d \n", (a+256)%256 , b );

        if( baseRules[b]->insert( (a+256)%256, settings->GetVerbose() ) != 0 ) //convert a char to its ASCII value...
            break;

        if( digramStoreSize() >= maxDigramStoreSize() && maxDigramStoreSize() != -1 )
        {
          fprintf(stderr, "Too many digrams. todo: use STL's map\n");
          break;
        }

      }
      //if( settings->GetVerbose() == 1)  printf("\n b:%d  a:%c\n",b, a);
    }
    printf("\n");

    for(i = 0; i < settings->GetInFileTableSize(); i++)
        baseRules[i]->endInserting();

    //print the rules to the output-file
    print(settings->getIpFileOut(), (rules**) baseRules, settings->GetInFileTableSize() );

    delete inputFileReader;

    settings->saveOut();

    printf("All ok. free memory. file-iterations: %ld chars: %ld digrams: %ld / %ld\n",nFileIterations,charsRead, digramStoreSize(), maxDigramStoreSize());
    if( settings->GetVerbose() == 1)
        print( stdout, (rules**) baseRules, settings->GetInFileTableSize() );
    break;
  }

  //printf("\n all done. free memory");
  /*if( baseRules != NULL )
  {
    for(int i = 0; i < settings->GetInFileTableSize(); i++)
      if ( baseRules[i] != NULL ) delete baseRules[i];
    if(  settings->GetInFileTableSize() != 0 ) 
      free( baseRules );
  }*/
  delete settings;
}
