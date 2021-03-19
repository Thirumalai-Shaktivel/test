#include "pl0.h"
#include <stdio.h>

void error(const char msg[])
{
    printf("%s\n", msg);
}

void nextsym(void)
{
    sym = ident;
}

int main()
{
    printf("OK\n");
    return 0;
}
