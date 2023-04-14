
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 1024

int main()
{
  //Testing Coalescing/Splitting functionality, desired # of coalesces is 3
  char* ptr1 = (char*) malloc(BLOCK_SIZE);
  char* ptr2 = (char*) malloc(BLOCK_SIZE);
    
  //Ensure memory gets touched so the compiler doesnt throw warnings
  ptr1 = ptr1;
  ptr2 = ptr2;

  free(ptr1);
  free(ptr2);

  char* ptr3 = (char*) malloc(BLOCK_SIZE);
  char* ptr4 = (char*) malloc(BLOCK_SIZE);
  char* ptr5 = (char*) malloc(BLOCK_SIZE);

  ptr3 = ptr3;
  ptr4 = ptr4;
  ptr5 = ptr5;

  free(ptr3);
  free(ptr4);
  free(ptr5);

}