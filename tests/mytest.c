#include <stdlib.h>
#include <stdio.h>

int main()
{
    char* ptr[4000];
    for(int i = 0; i < 4000; i++)
    {
        ptr[i] = malloc(1);
    }
    for(int i = 0; i < 2000; i++)
    {
        free(ptr[i]);
    }

    char* ptr2 = malloc(20000);
    char* ptr3 = realloc(ptr2, 25000);
    free(ptr3);

    char* ptr4 = calloc(5,50);
    free(ptr4);

    for(int i = 0; i < 2000; i++)
    {
        free(ptr[4000-i]);
    }
    return 0;
}