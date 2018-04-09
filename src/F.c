#include "F.h"
#include "sboxes.h"
#include "tables.h"

WORD32 F(WORD48 subkey, WORD32 right)
{
  return p(sboxes(e(right) ^ subkey));
}
