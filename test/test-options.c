#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "options.h"

/* Some option arrays */
char *short_key[] = {"test", "abcd"};
char *long_key[] = {"test", "12345678901234567890"};
char *illegal_option[] = {"test", "-a", "-b"};
char *bad_key[] = {"test", "abcdefghijklmnop"};
char *good_key[] = {"test", "1111222233334444"};
char *decrypt[] = {"test", "1111222233334444", "-d"};
char *infile[] = {"test", "-i", "foo"};
char *outfile[] = {"test", "-o", "bar"};

/* TODO Split into several functions for clarity */
int test_options()
{
  int fail;
  unsigned int a, b;
  struct options o1;

  fail = 0;
  o1 = parse_options(0, NULL);
  if (0 != o1.valid)
    {
      ++fail;
      fprintf(stderr, "FAIL: Accepted empty options.\n");
    }

  o1 = parse_options(2, short_key);
  if (0 != o1.valid)
    {
      ++fail;
      fprintf(stderr, "FAIL: Accepted too short key '%s'.\n", short_key[0]);
    }

  o1 = parse_options(2, long_key);
  if (0 != o1.valid)
    {
      ++fail;
      fprintf(stderr, "FAIL: Accepted too long key '%s'.\n", long_key[0]);
    }

  o1 = parse_options(3, illegal_option);
  if (0 != o1.valid)
    {
      ++fail;
      fprintf(stderr, "FAIL: Accepted illegal option '%s'.\n", illegal_option[0]);
    }

  o1 = parse_options(2, bad_key);
  a = (o1.key >> 32) & 0xffffffff;
  b = o1.key & 0xffffffff;
  if (0 != o1.valid)
    {
      ++fail;
      fprintf(stderr, "FAIL: Accepted bad key '%s' as 0x%x%x.\n", bad_key[1], a, b);
    }

  o1 = parse_options(2, good_key);
  a = (o1.key >> 32) & 0xffffffff;
  b = o1.key & 0xffffffff;
  if (0 == o1.valid)
    {
      ++fail;
      fprintf(stderr, "FAIL: Did not accept good key '%s'.\n", good_key[1]);
    }
  else if (a != 0x11112222 || b != 0x33334444)
    {
      ++fail;
      fprintf(stderr, "FAIL: Read good key '%s' as 0x%x%x.\n", good_key[1], a, b);
    }
  if (1 == o1.decrypt)
    {
      ++fail;
      fprintf(stderr, "FAIL: Set decrypt mode without decrypt flag -d present.\n");
    }

  o1 = parse_options(3, decrypt);
  if (0 == o1.decrypt)
    {
      ++fail;
      fprintf(stderr, "FAIL: Did not recognize decrypt flag -d.\n");
    }

  o1 = parse_options(3, infile);
  if (strncmp("foo", o1.infile, 3))
    {
      ++fail;
      fprintf(stderr, "FAIL: Read infile 'foo' as '%s'.\n", o1.infile);
    }

  o1 = parse_options(2, outfile);
  if (strncmp("bar", o1.outfile, 3))
    {
      ++fail;
      fprintf(stderr, "FAIL: Read outfile 'bar' as '%s'.\n", o1.outfile);
    }

  return fail;
}

int main()
{
  int fail;

  fprintf(stdout, "test_options: ");
  fail = 0;

  fail += test_options();

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
