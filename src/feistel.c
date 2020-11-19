#include "feistel.h"
#include "tables.h"
#include "sboxes.h"

WORD64 feistel(WORD64 inbuf, WORD48 *K)
{
  WORD32 left, right, temp;
  int i;

  right = inbuf & 0xffffffff;
  left = (inbuf >> 32) & 0xffffffff;

  for (i = 0 ; i < 16 ; ++i)
    {
      temp = right;
      right = (left ^ F(K[i], right));
      left = temp;
    }

  return ((WORD64) right << 32) | left;
}
