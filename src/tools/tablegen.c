#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b) ((a > b) ? a : b)

int main(int argc, char **argv)
{
  int i = 1;
  int c, inbits, outbits;
  
  inbits = atoi(argv[1]);
  outbits = atoi(argv[2]);

  printf("#include \"types.h\"\n\n");
  printf("WORD%d %s(WORD%d i)\n{\n", outbits, argv[3], inbits);
  printf("  WORD%d out = 0;\n", outbits);
  printf("  WORD%d in = i;\n\n", MAX(inbits, outbits));
  
  printf("  out = \n");
  while (!feof(stdin))
    {
      scanf("%d",&c);
      printf("    (((in >> %2d) & 0x1) << %2d) |\n",(inbits - c),(outbits - i));
      ++i;
    }
  printf("    0;\n\n  return out;\n}\n");
  
  return 0;
}
