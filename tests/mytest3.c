#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 1024

int main() {
    // Test block splitting functionality
    char* block = (char*) malloc(BLOCK_SIZE);
    printf("Allocated block at address %p\n", block);
    free(block);
    printf("Freed block at address %p\n", block);
    char* split_block1 = (char*) malloc(BLOCK_SIZE / 4);
    printf("Allocated split block 1 at address %p\n", split_block1);
    char* split_block2 = (char*) malloc(BLOCK_SIZE / 4);
    printf("Allocated split block 2 at address %p\n", split_block2);
    free(split_block1);
    printf("Freed split block 1 at address %p\n", split_block1);
    free(split_block2);
    printf("Freed split block 2 at address %p\n", split_block2);
    return 0;
}
