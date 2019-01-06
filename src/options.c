#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "options.h"

static void _print_usage(char *prog_name)
{
  fprintf(stderr, "Usage: %s [-d] [-i file] [-o file] key\nkey must be 16 hex characters.\n", prog_name);
}

static WORD64 _readkey(char *keystring, int *valid)
{
  WORD64 key;
  int i;
  char k;

  key = 0;
  for (i = 0 ; i < 16 ; ++i)
    {
      key = key << 4;
      k = keystring[i];
      if ('0' <= k && '9' >= k)
	{
	  key += k - '0';
	}
      else if ('a' <= k && 'f' >= k)
	{
	  key += k - 'a' + 10;
	}
      else if ('A' <= k && 'F' >= k)
	{
	  key += k - 'A' + 10;
	}
      else
	{
	  fprintf(stderr, "Illegal key character '%c'\n", k);
	  *valid = 0;
	  return 0;
	}
    }
  return key;
}

struct options parse_options(int argc, char **argv)
{
  struct options opt;
  int i;

  opt.infile = NULL;
  opt.outfile = NULL;
  opt.decrypt = 0;
  opt.valid = 0;
  for (i = 1 ; i < argc ; ++i)
    {
      if ('-' != argv[i][0])
	{
	  if (16 == strlen(argv[i]))
	    {
	      opt.valid = 1;
	      opt.key = _readkey(argv[i], &opt.valid);
	    }
	  else
	    {
	      fprintf(stderr, "Bad key length %d\n", (int) strlen(argv[i]));
	      _print_usage(argv[0]);
	      opt.valid = 0;
	      return opt;
	    }
	}
      else
	{
	  switch(argv[i][1])
	    {
	    case 'd':
	      opt.decrypt = 1;
	      break;
	    case 'i':
	      opt.infile = argv[++i];
	      break;
	    case 'o':
	      opt.outfile = argv[++i];
	      break;
	    default:
	      fprintf(stderr, "Illegal option %s\n", argv[i]);
	      _print_usage(argv[0]);
	      opt.valid = 0;
	      return opt;
	    }
	}
    }
  return opt;
}
