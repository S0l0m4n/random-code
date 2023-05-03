/**
 * About the %*s format specifier; it depends on which function we're discussing.
 * - In `printf`, the `*` indicates the minimum number of characters to print,
 *   e.g. `printf("%4s", foo)` means print at least four characters when
 *   printing `foo`, even if `foo` is not that long.
 * - In `scanf`, the `*` indicates that the corresponding argument is optional:
 *   e.g. `scanf("%s %*s %d", foo, bar, baz)`
 *   In this line, we must read a string for `foo` and a number for `baz`, but
 *   we can skip supplying the string for `bar`.
  */

#include <stdio.h>
#include <string.h>

int main(void)
{
    char buff[30];

    while (EOF != scanf("%20s", buff))
    {
        printf("> %s [%lu]\n", buff, strlen(buff));
    }

    return 0;
}
