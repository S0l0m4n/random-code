/**
  * Print a decimal number as a hex number
  */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    unsigned int allowedBands = 185473183;
    unsigned int allowedBandsHex = 0xb0e189f;
    printf("allowed bands = 0x%08x\n", allowedBands);
    if (allowedBands == allowedBandsHex)
    {
        printf("-> all bands are enabled!\n");
    }
}
