/**
 * This is an example of string manipulation in C.
 * Given the input string "pebbles", we return "pebles".
 */
#include <stdio.h>

int whatDoesThisDo(char *str)
{
  if (str == NULL || *str == '\0') return 0;

  int value = 1, index = 1;

  while (*(str+value))
  {
    if (str[value] != str[value - 1])
    {
      str[index] = str[value];
      index++;
    }
    
    value++;
  }

  *(str+index) = '\0';

  return index;
}

int main(void)
{
  char input[] = "pebbles";
  whatDoesThisDo(input);
  puts(input);
  return 0;
}
