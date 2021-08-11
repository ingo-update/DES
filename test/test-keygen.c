#include <stdio.h>
#include <stdlib.h>

#include "options.h"
#include "key.h"
#include "keygen.h"

int test_keygen()
{
  int fail = 0;
  int i;
  struct options opt;
  WORD48 *k0, *k1;

  opt.key = 0x0123456789abcdef;

  opt.decrypt = 0;
  k0 = keygen(opt);
  opt.decrypt = 1;
  k1 = keygen(opt);

  for (i = 0 ; i < N_SUBKEYS - 1 ; ++i)
    {
      if (k0[i] != k1[N_SUBKEYS - i - 1])
	{
	  ++ fail;
	  fprintf(stderr, "FAIL: Encrypt key %d differs from Decrypt key %d.\n", i, N_SUBKEYS - i - 1);
	}
    }

  return fail;
}

int main()
{
  int fail;

  fprintf(stdout, "test_keygen: ");
  fail = 0;

  fail += test_keygen();

  if (0 == fail)
    {
      fprintf(stdout, "SUCCESS\n");
      return EXIT_SUCCESS;
    }
  else
    {
      fprintf(stdout, "FAILURE; %d tests failed.\n", fail);
      return EXIT_FAILURE;
    }
}
