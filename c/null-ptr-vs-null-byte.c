/**
 * null pointer vs null byte
 */

#include <stdbool.h>
#include <stdio.h>

int main(void)
{
    char hostIpAddr[16] = {'\0'};
    char* pHostIpAddr = hostIpAddr;
    bool ret = (NULL != pHostIpAddr);

    printf("ret = %d pHost -> %p pHost[0] = %x\n", ret, pHostIpAddr, pHostIpAddr[0]);

    return 1;
}
