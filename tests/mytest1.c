#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NUM_BLOCKS 8192
#define BLOCK_SIZE 512

int main()
{
  printf("Test for malloc and free with lots of input\n");

  // Allocate a large chunk of memory
  char* big_block = (char*) malloc(NUM_BLOCKS * BLOCK_SIZE);
  if ( !big_block )
  {
    perror("Failed to allocate memory for the big block\n");
    return 1;
  }

  // Allocate an array of small chunks of memory
  char* small_blocks[NUM_BLOCKS];
  for (int i = 0; i < NUM_BLOCKS; i++) 
  {
    small_blocks[i] = (char*) malloc(BLOCK_SIZE);
    if ( !small_blocks[i] ) 
    {
    printf("Error: failed to allocate small block %d\n", i);
    return 1;
    }
  }

  // Free every other small block
  for (int i = 0; i < NUM_BLOCKS; i += 2) {
    free( small_blocks[i] );
  }

  // Allocate a few more small blocks
  char* more_small_blocks[NUM_BLOCKS];
  for (int i = 0; i < NUM_BLOCKS; i += 3) {
    more_small_blocks[i] = (char*) malloc(BLOCK_SIZE);
    if ( !more_small_blocks[i] ) 
    {
      printf("Error: failed to allocate small block %d\n", i);
      return 1;
    }
  }

  // Free the big block
  free(big_block);

  // Free the remaining small blocks
  for (int i = 1; i < NUM_BLOCKS; i += 2) {
    free( small_blocks[i] );
  }
  for (int i = 0; i < NUM_BLOCKS; i += 3) {
    free( more_small_blocks[i] );
  }

  // Allocate a very large block
  char* huge_block = (char*) malloc(NUM_BLOCKS * BLOCK_SIZE * 10);
  if ( !huge_block ) 
  {
    printf("Error: failed to allocate huge block\n");
    return 1;
  }

  // Free the huge block
  free(huge_block);

  printf("Test 3 completed successfully\n");
  return 0;
}
