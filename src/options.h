#include "types.h"

#ifndef _OPTIONS_H_
#define _OPTIONS_H_

/* Options structure */
struct options
{
  WORD64 key;
  char *infile;
  char *outfile;
  int decrypt;
  int valid;
};

struct options parse_options(int argc, char **argv);

#endif
