#include <stdio.h>
#include <stdlib.h>

#include "options.h"
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

  for (i = 0 ; i < 15 ; ++i)
    {
      if (k0[i] != k1[15-i])
	{
	  ++ fail;
	  fprintf(stderr, "FAIL: Encrypt key %d differs from Decrypt key %d.\n", i, 15 - i);
	}
    }

  return fail;
}

int main()
{
  int fail = 0;

  fail += test_keygen();

  return fail;
}
