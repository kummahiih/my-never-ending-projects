struct settings
{
  int nVerbose;
  int nEncode;
  int nDecode;
  int nWords;
  int nCode;
};

typedef struct settings  settings;

int checkOption( int argc, char **argv, char **psOptions, int *i , int verbose);
settings * parseOptions( int argc, char **argv );
void printSetting(settings * ptNew);
void printHelp( char * sProgramname);
void printUsage( char * sProgramname );
