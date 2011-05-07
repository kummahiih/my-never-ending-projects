#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "parse_settings.h"

int checkOption( int argc, char **argv, char **psOptions, int *i , int verbose)
{
  if ( verbose == 1 )
    printf("(D:i %d)\n",*i);
  if( *i < argc && ( strcmp( argv[*i], psOptions[0] ) == 0 || strcmp( argv[*i], psOptions[1]  ) == 0 ) )
  {
    (*i)++;
    if ( verbose == 1 )
    printf("(D:true.. i %d)\n",*i);
    return 0;
  }
  else
    return 1;
}

settings * parseOptions( int argc, char **argv )
{
  char *help[] =    {"-h","--help"};
  char *verbose[] = {"-v","--verbose"};
  char *atTime[] =  {"-n","--filesAtTime"};
  char *output[] =  {"-o","--output"};
  char *source[] =  {"-s","--sourceFiles"};
  char *fromstdin[] = { "-i","--fromStdin"};
  char *file[] =    {"-f","--fileList"};

  int i = 1,j=0;

  settings * ptNew = ( settings * ) malloc( sizeof( settings) );

  if( ptNew == ( settings * ) 0 )
  {
      fprintf(stderr,"\nSettingsAllocationError\n");
    return ( settings * ) 0;
  }
  
  ptNew->nVerbose = 0;
  ptNew->nDirectly = 0;
  ptNew->nInDirectly = 0;
  ptNew->nFromStdin = 0;
  ptNew->nOpenAtOnce = 0;
  ptNew->psOutput = 0;
  ptNew->ppsInput = 0;
  ptNew->nInput = 0;
  ptNew->nError = 0;
  ptNew->nHelp = 0;
  
  //Usage: %s [-v|--verbose] ( 
  //   (-h |--help)|( 
  //      ( (-o|--output) OUTPUTFILE  (-n|--filesAtTime) INTEGER  ((-s|--sourceFiles)|(-f|--fileList))  FILENAME)
  //      )
  //   )

  if( argc == 1 )
    printUsage( argv[0] );

  if( checkOption( argc, argv, verbose, &i, 0 ) == 0 && ptNew->nError == 0)
  {
    ptNew->nVerbose = 1;
    printf("(D: Verbose on)\n");
  }

  if( checkOption( argc, argv, help, &i,ptNew->nVerbose ) == 0  && ptNew->nError == 0)
  {
    ptNew->nHelp = 1;
    printHelp( argv[0] );
  }
  else
  {
     if( checkOption( argc, argv, output, &i ,ptNew->nVerbose) == 0  && ptNew->nError == 0)
     {
       if( i < argc )
	 ptNew->psOutput = argv[i];
       else
	 ptNew->nError = 1;

       if(ptNew->nVerbose == 1 )
	 printf("(D: output set)\n");
       i++;
     }
     else
       ptNew->nError = 1;

     if( checkOption( argc, argv, atTime , &i,ptNew->nVerbose ) == 0  && ptNew->nError == 0)
     {
       if(ptNew->nVerbose == 1 )
	 printf("(D: getting input amount )\n");
      
       if(  ptNew->nError == 0 )
       {
	 for(j=0;argv[i][j] != 0 &&  ptNew->nError == 0;j++)
	 {
	   if( argv[i][j] < 48 || argv[i][j] > 57 )
	     ptNew->nError = 1;
	   else
	     ptNew->nOpenAtOnce =  ptNew->nOpenAtOnce*10 + (int)(argv[i][j] )-48;
	 }
       }
       i++;
     }
     else
       ptNew->nError =1;


     if( checkOption( argc, argv, source, &i ,ptNew->nVerbose) == 0  && ptNew->nError == 0)
       ptNew->nInDirectly = 1;
     else if( checkOption( argc, argv, file, &i ,ptNew->nVerbose) == 0  && ptNew->nError == 0)
       ptNew->nDirectly = 1;
     else if( checkOption( argc, argv, fromstdin, &i ,ptNew->nVerbose) == 0  && ptNew->nError == 0)
       ptNew->nFromStdin = 1;
     else
        ptNew->nError =1;

     if(ptNew->nVerbose == 1 )
       printf("(D: Input mode set)\n");

     if(  ptNew->nDirectly != 0 ||  ptNew->nInDirectly != 0  && ptNew->nError == 0 )
     {
      
       if( i < argc )
       {
	 ptNew->ppsInput = &argv[i];
	 ptNew->nInput = argc - i;
	 i = argc;
       }
       else
	 ptNew->nError = 1;

       if(ptNew->nVerbose == 1 )
	 printf("(D: inputs set)\n");
     }
  }
  
  
  if(ptNew->nVerbose == 1 )
    printSetting( ptNew );
  if( i != argc ||  ptNew->nError != 0 )
  {
    free( ptNew );
    if(ptNew->nVerbose == 1 )
      printf("(D: argc %d i % i)\n",argc, i);
    printUsage( argv[0] );
    return ( settings * ) 0;
  }
  return ptNew;
}

void printSetting(settings * ptNew)
{
  int i = 0;
  printf("  ptNew->nVerbose %d\n", ptNew->nVerbose);
  printf("  ptNew->nDirectly %d\n", ptNew->nDirectly);
  printf("  ptNew->nInDirectly  %d\n",ptNew->nInDirectly);
  printf("  ptNew->nFromStdin  %d\n",ptNew->nFromStdin);
  printf("  ptNew->nOpenAtOnce  %d\n",   ptNew->nOpenAtOnce );
  printf("  ptNew->psOutput  %d\n",  ptNew->psOutput  );
  printf("  ptNew->ppsInput  %d\n",  ptNew->ppsInput);
  printf("  ptNew->nInput  %d\n",  ptNew->nInput  );
  printf("  ptNew->nError  %d\n",ptNew->nError);

   printf("inputfiles: \n");
  
  for( i = 0; i < ptNew->nInput; i++ )
    printf("%s\n", ptNew->ppsInput[i] );
  

}

void printHelp( char * sProgramname)
{
  printf( "Takes some command-line parameters, when called:\n");
  printf( "Example:\n");
  printf( "find *.jpg > files && ./%s  -o test.cfg -n 2 -i < files\n",sProgramname );
}

void printUsage( char * sProgramname )
{

  printf("Usage: %s [-v|--verbose]( (-h |--help)|(((-o|--output) OUTPUTFILE  (-n|--filesAtTime) INTEGER ((-i|--fromStdin)|((-s|--sourceFiles)|(-f|--fileList))(  FILENAME)+)))",  sProgramname );
  
  printf("-h or --help for help.\n" );
}
