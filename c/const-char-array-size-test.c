/**
  * Test if the compiler complains if an array is smaller than the size of a
  * const char string
  *
  * ANS: It gives you a warning.
  */


#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#define MAX_LEN       7
#define NUM           2

const char strings[NUM][MAX_LEN + 1] =
{
  "string",
  "bigstring"
};


int main(void)
{
  uint8_t i = 0;

  for (i = 0; i < NUM; i++)
  {
    printf("%s\n", strings[i]);
  }

  return 0;
}
