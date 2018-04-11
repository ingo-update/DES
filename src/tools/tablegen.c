#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b) ((a > b) ? a : b)

int main(int argc, char **argv)
{
  int inbits, outbits, i, c;

  inbits = atoi(argv[1]);
  outbits = atoi(argv[2]);

  printf("#include \"types.h\"\n\n");
  printf("WORD%d %s(WORD%d i)\n{\n", outbits, argv[3], inbits);
  printf("  WORD%d out = 0;\n", outbits);
  printf("  WORD%d in = i;\n\n", MAX(inbits, outbits));
  
  printf("  out = \n");
  for (i = outbits ; i > 0 ; --i)
    {
      scanf("%d",&c);
      printf("    (((in >> %2d) & 0x1) << %2d) |\n",(inbits - c), (i - 1));
    }
  printf("    0;\n\n  return out;\n}\n");
  
  return 0;
}
