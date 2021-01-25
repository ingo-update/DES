#include <stdio.h>
#include <stdlib.h>

#include "keygen.h"
#include "tables.h"

int shift[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

WORD48 *keygen(struct options opt)
{
  WORD48 *subkeys;
  WORD28 c, d;
  WORD56 protokey;
  int i;

  /* Allocate subkey array. */
  subkeys = (WORD48 *) malloc(16 * sizeof(WORD48));
  if (NULL == subkeys)
    {
      fprintf(stderr,"keygen() failed, couldn't allocate subkey memory.\n");
      exit(EXIT_FAILURE);
    }

  /* Do permuted choice 1. */
  protokey = pc1(opt.key);

  /* Initiate c and d. */
  c = (protokey >> 28) & 0xfffffff;
  d = protokey & 0xfffffff;

  /* Generate the subkeys. */
  for (i = 0 ; i < 16 ; ++i)
    {
      c = ((((WORD56) c) << 28) | c) >> (28 - shift[i]) & 0xfffffff;
      d = ((((WORD56) d) << 28) | d) >> (28 - shift[i]) & 0xfffffff;

      /* Do permuted choice 2 and store the result as a subkey. (reverse order for decrypting) */
      subkeys[opt.decrypt ? 15 - i : i] = pc2((((WORD48) c) << 28) | d);
    }

  return subkeys;
}
