#include <stdio.h>
#include <string.h>
//#include <malloc.h>
#include <stdlib.h>
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
  char *encode[] =  {"-e","--encode"};
  char *words[] =   {"-w","--words"};
  char *code[] =    {"-c","--code"};
  char *decode[] =  {"-d","--decode"};
  int i = 1;

  settings * ptNew = ( settings * ) malloc( sizeof( settings) );

  if( ptNew == ( settings * ) 0 )
  {
      fprintf(stderr,"\nSettingsAllocationError\n");
    return ( settings * ) 0;
  }

  ptNew->nVerbose = 0;
  ptNew->nEncode = 0;
  ptNew->nDecode = 0;
  ptNew->nWords = 0;
  ptNew->nCode = 0;
  
  //Usage: %s [-v|--verbose] ( (-h |--help)|( ( (-e|--encode) [-w|--words] [-c|--code])|(-d | --decode) ) )

  if( argc == 1 )
    printUsage( argv[0] );

  if( checkOption( argc, argv, verbose, &i, 0 ) == 0)
  {
    ptNew->nVerbose = 1;
    printf("(D: Verbose on)\n");
  }

  if( checkOption( argc, argv, help, &i,ptNew->nVerbose ) == 0)
  {
    printHelp( argv[0] );
  }
  else if( checkOption( argc, argv, decode, &i ,ptNew->nVerbose) == 0)
  {
    if(ptNew->nVerbose == 1 )
      printf("(D: decode on)\n");
    ptNew->nDecode = 1;
  }
  else if( checkOption( argc, argv, encode, &i ,ptNew->nVerbose) == 0)
  {
    if(ptNew->nVerbose == 1 )
      printf("(D: encode on)\n");
    ptNew->nEncode = 1;
    if( checkOption( argc, argv, words, &i ,ptNew->nVerbose) == 0)
      ptNew->nWords = 1;
    if( checkOption( argc, argv, code, &i ,ptNew->nVerbose) == 0)
      ptNew->nCode = 1;
  }
  
  if(ptNew->nVerbose == 1 )
    printSetting( ptNew );
  if( i != argc )
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
  printf("(D:ptNew->nVerbose %d)\n",ptNew->nVerbose);
  printf("(D:ptNew->nEncode %d)\n",ptNew->nEncode);
  printf("(D:ptNew->nDecode %d)\n",ptNew->nDecode);
  printf("(D:ptNew->nWords %d)\n",ptNew->nWords);
  printf("(D:ptNew->nCode %d)\n",ptNew->nCode);
}

void printHelp( char * sProgramname)
{
printf( "Takes some command-line parameters, when called:\n");
printf( "  -e | --encode  \n");
printf( "  -d | --decode\n");
printf( "  -w | --words\n");
printf( "  -c | --code\n");
printf( "  -h | --help\n");
printf( "\n");
printf( "\n");
printf( "The Huffman program( %s )\n",sProgramname);
printf( "| \n");
printf( "|-- no parameters --> usage-text\n");
printf( "|\n");
printf( "|-- \"-e\" or \"--encode\" --> encode the input data \n");
printf( "|                            |-- \"-w\" or \"--words\" --> prints only the ASCII to Huffman pairs.\n");
printf( "|                            |\n");
printf( "|                            |-- \"-c\" or \"--code\"  --> prints only the encoded data to stdout.\n");
printf( "|                            |\n");
printf( "|                            .-- \"-w\" or \"--words\" and \"-c\" or \"--code\"--> prints both ASCII to hufman and encoded data to stdout \n");
printf( "|\n");
printf( "|-- \"-d\" or \"--decode\"  --> prints the decoded data to stdout\n");
printf( "|\n");
printf( "|-- \"-h\" or \"--help\"    --> prints the help-text\n");
printf( "|\n");
printf( "|-- \"-v\" or \"--verbose\" --> prints debug-messages to standard out stdout\n");
printf( "|                          | \n");
printf( "|                          |-- no parameters --> usage-text\n");
printf( "|                          |\n");
printf( "|                          |-- \"-e\" or \"--encode\" --> encode the input data \n");
printf( "|                          |                            |-- \"-w\" or \"--words\" --> prints only the ASCII to Huffman pairs.\n");
printf( "|                          |                            |\n");
printf( "|                          |                            |-- \"-c\" or \"--code\"  --> prints only the encoded data to stdout \n");
printf( "|                          |                            |\n");
printf( "|                          |                            .-- \"-w\" or \"--words\" and \"-c\" or \"--code\"--> prints both ASCII to hufman and encoded data to stdout \n");
printf( "|                          |\n");
printf( "|                          .-- \"-d\" or \"--decode\" --> prints the decoded data to stdout\n");
printf( "|\n");
printf( "|-- \"-h\" or \"--help\"   --> prints the help-text\n");
printf( "|\n");
printf( ".-- something else --> error-message and usage-text\n");
printf( "\n");
}

void printUsage( char * sProgramname )
{
  printf("Usage: %s [-v|--verbose] ( (-h |--help)|( ( (-e|--encode) [-w|--words] [-c|--code])|(-d | --decode) ) )\n", sProgramname );
  printf("-h or --help for help.\n" );
}
