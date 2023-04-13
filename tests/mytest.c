#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 128

int main() {
    // Test block splitting functionality
    void* block = malloc(BLOCK_SIZE);
    printf("Allocated block at address %p\n", block);
    free(block);
    printf("Freed block at address %p\n", block);
    void* split_block1 = malloc(BLOCK_SIZE / 2);
    printf("Allocated split block 1 at address %p\n", split_block1);
    void* split_block2 = malloc(BLOCK_SIZE / 2);
    printf("Allocated split block 2 at address %p\n", split_block2);
    free(split_block1);
    printf("Freed split block 1 at address %p\n", split_block1);
    free(split_block2);
    printf("Freed split block 2 at address %p\n", split_block2);
    return 0;
}
