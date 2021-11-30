/**
  * addition problem
  */


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
  unsigned int w, x, y, z;
  unsigned int A, B, C;
  bool found = false;

  printf(" w | x | y | z  =>    A  +   B  =  sum  cf.   C\n");
  printf("-----------------------------------------------\n");

  for (y = 0; (y <= 9) && !found; y++)
  {
    for (z = 0; (z <= 9) && !found; z++)
    {
      for (w = 0; (w <= 9) && !found; w++)
      {
        for (x = 0; (x <= 9) && !found; x++)
        {
          A = 100*x + 10*y + z;
          B = 100*y + 10*w + y;
          C = 1000*z + 100*y + 10*z + w;

          printf(" %d | %d | %d | %d  =>  %3d  + %3d  = %4d  cf %4d\n",
                    w,   x,   y,   z,       A,    B,  A+B,     C);

          if ((C == A + B) && ((w > 0) || (x > 0) || (y > 0) || (z > 0)))
          {
            found = true;
          }
        }
      }
    }
  }

  return 0;
}
