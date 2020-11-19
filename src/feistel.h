#include "types.h"

#define F(k, x) p(k ^ sboxes(e(x)))

WORD64 feistel(WORD64 inbuf, WORD48 *K);
