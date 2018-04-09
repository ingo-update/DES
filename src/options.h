#include "types.h"

#ifndef _OPTIONS_H_
#define _OPTIONS_H_
/* Options structure */
struct options
{
  WORD64 key;
  int decrypt;
  char *infile;
  char *outfile;
};

struct options parse_options(int argc, char **argv);

#endif
