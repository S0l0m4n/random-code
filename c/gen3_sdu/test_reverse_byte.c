/**
  * TEST FUNCTION
  */


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))


typedef struct
{
  uint8_t x;
  uint8_t y;
} test_case_st;


test_case_st test_cases[] =
{
  /* [  0 ] */  { 0x00, 0x00 },
  /* [  1 ] */  { 0x01, 0x80 },
  /* [  2 ] */  { 0x02, 0x40 },
  /* [  3 ] */  { 0x03, 0xc0 },
  /* [  4 ] */  { 0x05, 0xa0 },
  /* [  5 ] */  { 0x0f, 0xf0 },
  /* [  6 ] */  { 0x1f, 0xf8 },
  /* [  7 ] */  { 0xaa, 0x55 },
};


/**
 * @brief Reverse bits in a byte
 */
static inline uint8_t REVERSE_U8( uint8_t x )
{
  uint8_t y = 0;
  uint8_t i;

  for (i = 0; i < 8; i++)
  {
    y |= ( (1 & (x >> i)) << (7 - i) );
  }

  return y;
}


int main( void )
{
  uint8_t i;
  uint8_t num_tests = ARRAY_SIZE(test_cases);
  bool pass = true;
  test_case_st tc;
  uint8_t y;

  for (i = 0; (i < num_tests) && pass; i++)
  {
    /* --- set up test --- */
    tc = test_cases[i];
    /* --- */

    /* run test */
    y = REVERSE_U8(tc.x);

    /* check result */
    if (tc.y != y)
    {
      pass = false;
    }

    /* print result */
    printf("test %2d: %02x -> %02x -> %s",
        i, tc.x, tc.y, pass ? "PASS" : "FAIL");

    if (pass)
    {
      printf("\n");
    }
    else /* fail */
    {
      /* print actual result */
      printf(" %02x\n", y);
    }
  }

  if (pass && (i == num_tests))
  {
    printf("-> all tests passed!\n");
  }

  return 0;
}
