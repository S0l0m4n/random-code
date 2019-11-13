/**
  * Test strcmp/strncmp behaviour
  */

#include <stdio.h>
#include <string.h>

#define TEST_STRING "Middle"

int main(int argc, char **argv)
{
  int ret = 0;
  char *str = NULL;
  
  str = "Short";
  ret = strcmp(str, TEST_STRING);
  printf("Case 1: str is less than %s   : ret = %d\n", TEST_STRING, ret);

  str = TEST_STRING;
  ret = strcmp(str, TEST_STRING);
  printf("Case 1: str is the same   : ret = %d\n", ret);

  str = "Longest";
  ret = strcmp(str, TEST_STRING);
  printf("Case 1: str is longer     : ret = %d\n", ret);

  str = "Longest";
  ret = strcmp(str, TEST_STRING);
  printf("Case 1: str is longer     : ret = %d\n", ret);

  str = "Middl";
  ret = strcmp(str, TEST_STRING);
  printf("Case 1: str is less than %s   : ret = %d\n", TEST_STRING, ret);

  str = "MiddleN";
  ret = strcmp(str, TEST_STRING);
  printf("Case 1: str is less than %s   : ret = %d\n", TEST_STRING, ret);

  return 0;
}
