#include <stdio.h>
#include <stdlib.h>

#include "types.h"

int test_type_sizes()
{
  int fail = 0;

  if (sizeof(WORD64) < 64 / 8)
    {
      fprintf(stderr, "Failure: WORD64 is too short: (%d)\n", (int) sizeof(WORD64));
      ++fail;
    }
  if (sizeof(WORD56) < 56 / 8)
    {
      fprintf(stderr, "Failure: WORD56 is too short: (%d)\n", (int) sizeof(WORD56));
      ++fail;
    }
  if (sizeof(WORD48) < 48 / 8)
    {
      fprintf(stderr, "Failure: WORD48 is too short: (%d)\n", (int) sizeof(WORD48));
      ++fail;
    }
  if (sizeof(WORD32) < 32 / 8)
    {
      fprintf(stderr, "Failure: WORD32 is too short: (%d)\n", (int) sizeof(WORD32));
      ++fail;
    }
  if (sizeof(WORD28) < 28 / 8)
    {
      fprintf(stderr, "Failure: WORD28 is too short (%d)\n", (int) sizeof(WORD28));
      ++fail;
    }

  return fail;
}

int main()
{
  int fail;

  fprintf(stdout, "test-types: ");
  fail = 0;

  fail += test_type_sizes();

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
