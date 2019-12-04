/**
  * enum init test
  */

#include <stdlib.h>
#include <stdio.h>

enum COLOUR
{
  RED,
  GREEN,
  BLUE,
  YELLOW,
};

int main(void)
{
  enum COLOUR c1, c2, c3; /* uninitialised enums */
  printf("c1 = %x, c2 = %x, c3 = %x\n", c1, c2, c3);
  return 0;
}
