#include "pl0.h"
#include <stdio.h>
#include <stdlib.h>

extern unsigned char *cur;

void error(const char msg[])
{
    printf("%s\n", msg);
    exit(1);
}

int main()
{
    char *s = "call primes .";
    cur = (unsigned char*)s;
    nextsym();
    program();
    printf("OK\n");
    return 0;
}
