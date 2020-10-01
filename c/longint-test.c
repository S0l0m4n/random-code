/**
  * long int test
  */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

int main(void)
{
  /* time_t should be a long long int, i.e. 64 bits */
  //time_t now = 2088658186;
  time_t now = -2206309110;

  printf("now [ld]  = %ld\n", now);
  printf("    [lld] = %lld\n", now);

  return 0;
}
