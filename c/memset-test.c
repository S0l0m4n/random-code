/**
  * memset test
  */

#include <stdio.h>
#include <string.h>

#define NUM_OF_SAMPLES (10)

int main(void)
{
  int pSamples[NUM_OF_SAMPLES];

  int sample = 342;

  // test 1
  printf("Use for loop: ");
  for (int i = 0; i < NUM_OF_SAMPLES; i++)
  {
    pSamples[i] = sample;
    printf("%d ", pSamples[i]);
  }
  printf("\n");

  // test 2 - doesn't work because memset can only copy at char level (or byte
  // level), not (4-byte) integers
  memset(pSamples, sample, NUM_OF_SAMPLES);
  printf("Use memset: ");
  for (int i = 0; i < NUM_OF_SAMPLES; i++)
  {
    printf("%d ", pSamples[i]);
  }
  printf("\n");

  return 0;
}
