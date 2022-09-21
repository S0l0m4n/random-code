/**
  * int max
  */

#include <stdint.h>
#include <stdio.h>

int main(void)
{
  uint32_t x = 0xffffffff;

  printf("x = %u, x+1 = %u\n", x, x+1);

  return 0;
}
