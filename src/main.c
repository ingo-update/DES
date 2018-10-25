#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "options.h"
#include "keygen.h"
#include "feistel.h"
#include "tables.h"

int main(int argc, char **argv)
{
  struct options opt;
  FILE *in;
  FILE *out;
  WORD48 *K;
  WORD64 inbuf, outbuf;
  int bytes;

  /* Parse command line options. */
  opt = parse_options(argc, argv);

  /* Generate subkeys */
  K = keygen(opt);

  /* Set I/O streams */
  if (NULL == opt.infile)
    {
      in = stdin;
    }
  else
    {
      in = fopen(opt.infile,"r");
      if (NULL == in)
	{
	  fprintf(stderr, "Could not open file '%s' for input.\n", opt.infile);
	  exit(EXIT_FAILURE);
	}
    }

  if (NULL == opt.outfile)
    {
      out = stdout;
    }
  else
    {
      out = fopen(opt.outfile,"w");
      if (NULL == out)
	{
	  fprintf(stderr, "Could not open file '%s' for output.\n", opt.outfile);
	  exit(EXIT_FAILURE);
	}
    }

  /* Main loop */
  while (!feof(in))
    {
      inbuf = 0x0a0a0a0a0a0a0a0a;
      bytes = fread(&inbuf, 1, 8, in);

      if (0 != bytes)
	{
	  outbuf = pi(feistel(ip(inbuf), K));
	  fwrite(&outbuf, 1, 8, out);
	}
    }

  fclose(in);
  fclose(out);

  return EXIT_SUCCESS;
}
