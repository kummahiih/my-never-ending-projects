struct settings
{
  int nVerbose;
  int nDirectly;
  int nInDirectly;
  int nFromStdin;
  int nOpenAtOnce;
  char * psOutput;
  char ** ppsInput;
  int nInput;
  int nError;
  int nHelp;
};

typedef struct settings  settings;

int checkOption( int argc, char **argv, char **psOptions, int *i , int verbose);
settings * parseOptions( int argc, char **argv );
void printSetting(settings * ptNew);
void printHelp( char * sProgramname);
void printUsage( char * sProgramname );
