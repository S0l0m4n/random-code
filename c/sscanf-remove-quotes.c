/**
 * In this example, we demonstrate removing the quotes around a string using
 * sscanf, and we store the result in the very same variable.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

bool removeQuotes(char* input, char* output)
{
  char fq, lq;
  return (2 == sscanf(input, "%c%[^\"]s%c", &fq, output, &lq));
}

int main(void)
{
  char input[] = "\"pebbles\"";
  char output[100];
  printf("Original string: %s\n", input);
  bool success = removeQuotes(input, output);
  printf("Modified string: %s [%s]\n", output, success ? "done" : "fail");

  char response[] = "+USORD: 0,27,\"deadbeef01";
  int socket;
  unsigned int len;
  int ret = sscanf(response, "+USORD: %d,%u\"", &socket, &len);
  if (2 == ret)
  {
    printf("Matched +USORD string\n");
  }

  return 0;
}
