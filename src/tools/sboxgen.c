#include <stdio.h>

/*-----------------------------------------------------------------------------
 * Function: int main()
 *
 * Purpose: 
 * The S-Boxes in the book are tables of 4 rows and 16 columns, where the
 * column addres is b4b3b2b1 and the row addres is b5b0. This program 
 * converts the tables to linear arrays where the addres is b5b4b3b2b1b0. 
 *
 * If the S-Box data are read into a table then the mapping is as follows:
 * 
 *      000000 -> row = 00 column = 0000 : table elemant 0
 *      000001 -> row = 01 column = 0000 : table element 15
 *      000010 -> row = 00 column = 0001 : table element 1
 * ...
 *      100000 -> row = 10 column = 0000 : table element 32
 *      100001 -> row = 11 column = 0000 : table element 48
 *      100010 -> row = 10 column = 0001 : table element 33
 * ...
 *      111110 -> row = 10 column = 1111 : table element 47
 *      111111 -> row = 11 column = 1111 : table element 63
 *
 *---------------------------------------------------------------------------*/

int main()
{
  int s;         /* S-Box counter. */
  int i;         /* index counter. */
  int table[64]; /* Current S-Box data. */

  printf("#include \"types.h\"\n\n");

  for (s = 0 ; s < 8 ; ++s)
    {
      /* read the S-Box data. */
      for(i = 0; i < 64 ; ++i)
	{
	  scanf("%d", &(table[i]));
	}

      /* Print the S-Box as an array that can be used as a lookup table. */
      printf("WORD4 s%d[64] = {\n  ", s+1);
      for (i = 0 ; i < 8 ; ++i)
	{
	  printf("%d, %d, ", table[i], table[i+16]);
	}
      printf("\n  ");
      for (i = 0 ; i < 8 ; ++i)
	{
	  printf("%d, %d, ", table[i+8], table[i+24]);
	}
      printf("\n  ");
      for (i = 0 ; i < 8 ; ++i)
	{
	  printf("%d, %d, ", table[i+32], table[i+48]);
	}
      printf("\n  ");
      for (i = 0 ; i < 7 ; ++i)
	{
	  printf("%d, %d, ", table[i+40], table[i+56]);
	}

      /* No comma after the last element in the array. */
      printf("%d, %d\n};\n\n", table[47], table[63]);
    }

  printf("WORD32 sboxes(WORD48 in)\n{\n");
  printf("  return (WORD32)\n");
  for (i = 0 ; i < 8 ; ++i)
    {
      printf("    s%d[(WORD6) ((in >> %2d) & 0x3f)] << %2d |\n",i+1, 6*(7-i), 4*(7-i));
    }
  printf("    0;\n}\n");
  
  return 0;
}
