/**
  * safe_strcpy testing
  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFSIZE      10
#define MAXSTRLEN     100

#define SIZEOF(x)     (sizeof(x)/sizeof(*x))

static const char SrcStrings[][100] =
{
  "Test",
  "Hello",
  "Helloworld",
  "Helloworld!",
  "xxxxyyyyzzzzaaaa",
  "Hello1234",
  "1234567890",
  "xxx",
  "",
  "u-bloxR",
  "DONE",
};

/**
  * Safe version of strcpy and strncpy. Only copy up to n chars from src into
  * dest, INCLUDING the null byte. If there are less than n chars in src, only
  * these chars will be copied. Note that:
  * - strcpy is not safe because src may be bigger than dest (buffer overflow)
  * - strncpy is not safe because it doesn't automatically include the null byte
  */
static char * safe_strcpy(char *dest, char *src, size_t n)
{
  size_t i = 0;

  while (('\0' != src[i]) && (i < n-1))
  {
    dest[i] = src[i];
    i++;
  }

  dest[i] = '\0';

  return dest;
}

static int min(int a, int b)
{
  return (a < b) ? a : b;
}

int main(void)
{
  int i, len;
  char * src;
  char dest[BUFFSIZE];

  for (i = 0; i < SIZEOF(SrcStrings); i++)
  {
    src = (char *) SrcStrings[i];
    safe_strcpy(dest, src, BUFFSIZE);
    printf("%02d: src = %-20s | dest = %-20s | limit = %2d chars\n",
        i, src, dest, BUFFSIZE);
  }

  printf("---\n");

  for (i = 0; i < SIZEOF(SrcStrings); i++)
  {
    src = (char *) SrcStrings[i];
    len = min(strlen(src) + 1, BUFFSIZE);
    safe_strcpy(dest, src, len);
    printf("%02d: src = %-20s | dest = %-20s | limit = %2d chars\n",
        i, src, dest, len);
  }

  return 0;
}
