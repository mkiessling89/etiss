#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    char x[10] = {1};
    char y[10] = {2};
    memcpy(y, x, 10);
    printf("hello world!\n");
}
