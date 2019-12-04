/**
  * my atoi
  */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CMD_BAD_ATOI        0xffffffff

#define SIZEOF(x)           (sizeof(x)/sizeof(x[0]))

const char * A[] =
{
  "0",
  "1",
  "",
  "3423",
  "032",
  "99900394",
  "994.440.",
  "-3211",
  "!**&%!",
  "4233!",
  "11111",
  "-5",
  "5.0",
  "08343",
  "0xffffffff",
  "4294967295",
  "4294967294",
  "a",
};

uint32_t CMD_atoi(const char *c)
{
  uint32_t num = 0;

  /* Check that there is at least one character */
  num = ('\0' == *c) ? CMD_BAD_ATOI : 0;

	while (('\0' != *c) && (CMD_BAD_ATOI != num))
  {
    num = ((0x30 <= *c) && (*c <= 0x39)) ?
      (10 * num + (*c - 0x30)) : CMD_BAD_ATOI;
    c++;
  }

  return num;
}
    
int main(void)
{
  uint8_t i = 0;
  
  for (i = 0; i < SIZEOF(A); i++)
  {
    printf("%12s -> %12u | %12u\n", A[i], CMD_atoi(A[i]), (uint32_t)atoi(A[i]));
  }

  return 0;
}
