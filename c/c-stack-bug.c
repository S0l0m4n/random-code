#include <stdio.h>

void set_strings(char*** strings) {
  char* strs[] = {"banana"};
  *strings = strs;
}

int main() {
  char** strings;
  set_strings(&strings);
  printf("First print: '%s'\n", strings[0]);
  char* s = "abc";
  printf("Second print: '%s'\n", strings[0]);
}
