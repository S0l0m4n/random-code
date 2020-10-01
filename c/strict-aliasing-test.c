/**
  * strict aliasing test
  *
  * The function func() is meant to return a constant 1 but does not because it
  * fails the strict aliasing rule, causing undefined compiler behaviour.
  * 
  * Similarly, foo() returns 0 under optimisation (x and y are not aliased in
  * this case).
  *
  * For example, when I compile this program I get:
  *   $: gcc -Wall <prog>     # no optimisation, foo works as expected
  *   $: ./a.out
  *   > 1
  *   > -858993459
  *
  *   $: gcc -Wall -O2 <prog> # optimisation, foo is broken (but func works)
  *   $: ./a.out
  *   $: 0
  *   $: 1
  */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Value of 'a' can be accessed/modified either
// through 'a' or through 'b'
int a = 5;
int* b = &a;

uint32_t foo(uint16_t *x, uint32_t *y)
{
  *x = 0;
  *y = 1;
  return *x;
}

int func(double* b)
{
  a = 1;

  // The below statement modifies 'a'
  *b = 56.1;

  return (a);
}

int main()
{
  uint32_t l;
  printf("%d\n", foo((uint16_t *)&l, &l));
  printf("%d\n", func((double *)&a));
  return 0;
}
