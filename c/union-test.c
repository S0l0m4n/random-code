/**
  * union test
  */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef struct
{
  uint8_t major;
  uint8_t minor;
  uint16_t build;
} version_st;

typedef union
{
  version_st f;
  uint32_t full32;
} version_un;


int main(void)
{
  version_un pir_on_ver =
  {
    .f =
    {
      .major = 1,
      .minor = 3,
      .build = 4,
    },
  };
  version_un hwv;
  uint32_t hw_versions[3] = {0x00010000, 0x00040301, 0x00100302};
  uint8_t i;

  printf("pir = %7d ( %2d.%2d.%2d )\n",
      pir_on_ver.full32,
      pir_on_ver.f.major,
      pir_on_ver.f.minor,
      pir_on_ver.f.build);

  for (i = 0; i < 3; i++)
  {
    hwv.full32 = hw_versions[i];
    printf("hwv = %7d ( %2d.%2d.%2d ) : ",
        hwv.full32,
        hwv.f.major,
        hwv.f.minor,
        hwv.f.build);

    if (pir_on_ver.full32 < hwv.full32)
    {
      printf("less");
    }
    else if (pir_on_ver.full32 == hwv.full32)
    {
      printf("equal");
    }
    else
    {
      printf("more");
    };

    printf("\n");
  }

  return 0;
}
