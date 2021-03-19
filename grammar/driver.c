#include "pl0.h"
#include <stdio.h>

void error(const char msg[])
{
    printf("%s\n", msg);
    exit(1);
}

int main()
{
    nextsym();
    program();
    printf("OK\n");
    return 0;
}
