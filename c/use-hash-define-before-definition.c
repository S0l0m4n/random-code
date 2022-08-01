/**
  * use hash define before definition
  */


#include <stdio.h>


#define CONSTANT_VALUE (42)


int main(void)
{
  printf("This is a constant: %u\n", CONSTANT_VALUE);
  return 0;
}


/* XXX: We cannot place the definition of CONSTANT_VALUE */
//#define CONSTANT_VALUE (42)
