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
    char *s = "x := 1+(1+x*3-4)*4/n .";
    cur = (unsigned char*)s;
    nextsym();
    program();
    printf("OK\n");
    return 0;
}
