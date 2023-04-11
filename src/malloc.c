#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define ALIGN4(s)         (((((s) - 1) >> 2) << 2) + 4)
#define BLOCK_DATA(b)     ((b) + 1)
#define BLOCK_HEADER(ptr) ((struct _block *)(ptr) - 1)

static int atexit_registered = 0;
static int num_mallocs       = 0;
static int num_frees         = 0;
static int num_reuses        = 0; //anytime ffblock doesnt return null
static int num_grows         = 0;
static int num_splits        = 0;
static int num_coalesces     = 0;
static int num_blocks        = 0; //only calculate at the end
static int num_requested     = 0;
static int max_heap          = 0;

/*
 *  \brief printStatistics
 *
 *  \param none
 *
 *  Prints the heap statistics upon process exit.  Registered
 *  via atexit()
 *
 *  \return none
 */
void printStatistics( void )
{
  printf("\nheap management statistics\n");
  printf("mallocs:\t%d\n", num_mallocs );
  printf("frees:\t\t%d\n", num_frees );
  printf("reuses:\t\t%d\n", num_reuses );
  printf("grows:\t\t%d\n", num_grows );
  printf("splits:\t\t%d\n", num_splits );
  printf("coalesces:\t%d\n", num_coalesces );
  printf("blocks:\t\t%d\n", num_blocks );
  printf("requested:\t%d\n", num_requested );
  printf("max heap:\t%d\n", max_heap );
}

struct _block 
{
   size_t  size;         /* Size of the allocated _block of memory in bytes */
   struct _block *next;  /* Pointer to the next _block of allcated memory   */
   bool   free;          /* Is this _block free?                            */
   char   padding[3];    /* Padding: IENTRTMzMjAgU3ByaW5nIDIwMjM            */
};


struct _block *heapList = NULL; /* Free list to track the _blocks available */
struct _block *nextFit = NULL;
/*
 * \brief findFreeBlock
 *
 * \param last pointer to the linked list of free _blocks
 * \param size size of the _block needed in bytes 
 *
 * \return a _block that fits the request or NULL if no free _block matches
 *
 * \TODO Implement Next Fit
 * \TODO Implement Best Fit
 * \TODO Implement Worst Fit
 */
struct _block *findFreeBlock(struct _block **last, size_t size) 
{
   struct _block *curr = heapList;

#if defined FIT && FIT == 0
   /* First fit */
   //
   // While we haven't run off the end of the linked list and
   // while the current node we point to isn't free or isn't big enough
   // then continue to iterate over the list.  This loop ends either
   // with curr pointing to NULL, meaning we've run to the end of the list
   // without finding a node or it ends pointing to a free node that has enough
   // space for the request.
   // 
   while (curr && !(curr->free && curr->size >= size)) 
   {
      *last = curr;
      curr  = curr->next;
   }
#endif

#if defined BEST && BEST == 0
   //While the list pointer is valid:
   //check if the block is free, if it has enough space for the request,
   //and how much space is left over if it gets used.
   //after iterating through the list, pick the block with least amount
   //of leftover space
   long int remainder = INT64_MAX;
   struct _block *winner = NULL;
   while(curr)
   {
      if(curr->free && curr->size >= size && (long int)(curr->size - size) < remainder)
      {
         remainder = (long int)(curr->size - size);
         // *last->next = curr;
         winner = curr;
      }
      *last = curr;
      curr  = curr->next;
   }
   curr = winner;
#endif

#if defined WORST && WORST == 0
   //While the list pointer is valid:
   //check if the block is free if it has enough space for the request,
   //and how much space is left over if it gets used.
   //after iterating through the list, pick the block with the most
   //leftover space
   long int remainder = INT64_MIN;
   struct _block *winner = NULL;
   while(curr)
   {
      if(curr->free && curr->size >= size && (long int)(curr->size - size) > remainder)
      {
         remainder = (long int)(curr->size - size);
         winner = curr;
      }
      *last = curr;
      curr  = curr->next;
   }
   curr = winner;
#endif

// \TODO Put your Next Fit code in this #ifdef block
#if defined NEXT && NEXT == 0

   //if both exist, make curr = nextFit
   if(curr && nextFit)
   {
      curr = nextFit;
   }

   //perform first fit
   while (curr && !(curr->free && curr->size >= size)) 
   {
      *last = curr;
      curr  = curr->next;
   }

   //if we reach the end of the list and found no block
   //curr will be NULL but nextFit will not be NULL, so wrap around
   //loop until we reach nextFit. If still no block found, set curr to NULL
   if(!curr && nextFit)
   {
      curr = heapList;
      while(curr != nextFit && !(curr->free && curr->size >= size))
      {
         *last = curr;
         curr = curr->next;
      }
      if(curr == nextFit)
      {
         curr = NULL;
      }
   }
   
#endif
   if(curr)
   {
      num_blocks++;
   }
   nextFit = curr;
   return curr;
}

/*
 * \brief growheap
 *
 * Given a requested size of memory, use sbrk() to dynamically 
 * increase the data segment of the calling process.  Updates
 * the free list with the newly allocated memory.
 *
 * \param last tail of the free _block list
 * \param size size in bytes to request from the OS
 *
 * \return returns the newly allocated _block of NULL if failed
 */
struct _block *growHeap(struct _block *last, size_t size) 
{
   /* Request more space from OS */
   struct _block *curr = (struct _block *)sbrk(0);
   struct _block *prev = (struct _block *)sbrk(sizeof(struct _block) + size);

   assert(curr == prev);

   /* OS allocation failed */
   if (curr == (struct _block *)-1) 
   {
      return NULL;
   }

   /* Update heapList if not set */
   if (heapList == NULL) 
   {
      heapList = curr;
   }

   /* Attach new _block to previous _block */
   if (last) 
   {
      last->next = curr;
   }

   /* Update _block metadata:
      Set the size of the new block and initialize the new block to "free".
      Set its next pointer to NULL since it's now the tail of the linked list.
   */
   num_grows++;
   if(curr)
   {
      num_blocks++;
   }
   curr->size = size;
   curr->next = NULL;
   curr->free = false;
   // nextFit = curr;
   return curr;
}

/*
 * \brief malloc
 *
 * finds a free _block of heap memory for the calling process.
 * if there is no free _block that satisfies the request then grows the 
 * heap and returns a new _block
 *
 * \param size size of the requested memory in bytes
 *
 * \return returns the requested memory allocation to the calling process 
 * or NULL if failed
 */
void *malloc(size_t size) 
{
   if( atexit_registered == 0 )
   {
      atexit_registered = 1;
      atexit( printStatistics );
   }
   num_requested += size;
   /* Align to multiple of 4 */
   size = ALIGN4(size);

   /* Handle 0 size */
   if (size == 0) 
   {
      return NULL;
   }

   /* Look for free _block.  If a free block isn't found then we need to grow our heap. */

   struct _block *last = heapList;
   struct _block *next = findFreeBlock(&last, size);

   /* Could not find free _block, so grow heap */
   if (next == NULL) 
   {
      next = growHeap(last, size);
   }
   else
   {
      num_reuses++;
   }
   /* Could not find free _block or grow heap, so just return NULL */
   if (next == NULL) 
   {
      return NULL;
   }

   //block splitting
   if( next && (long int)(next->size - size) > (sizeof(struct _block) + 4) )
   {
   //declare a newBlock and temporarily point it to the next from findFreeBlock
   //set it's next pointer to next's next pointer to preserve the end of the original block
   //set its size to next's size minus the requested size and block size
   struct _block* newBlock = next;
   newBlock->next = next->next;
   newBlock->size = next->size - size - sizeof(struct _block);

   //set the found block's size to requested size + the block size
   //set it's next pointer to point to the address size+block bytes from where it starts
   //set it to inuse
   next->size = size + sizeof(struct _block);
   next->next = (struct _block*)((char*)next + size + sizeof(struct _block));
   // next->free = false;

   //change newBlock's starting point to where next->next is now pointing
   //set it as free to use on next request
   newBlock = next->next;
   newBlock->free = true;
   num_splits++;
   num_blocks++;
   }

   /* Mark _block as in use */
   next->free = false;
   num_mallocs++;
   /* Return data address associated with _block to the user */
   // nextFit = next;
   return BLOCK_DATA(next);
}

/*
 * \brief free
 *
 * frees the memory _block pointed to by pointer. if the _block is adjacent
 * to another _block then coalesces (combines) them
 *
 * \param ptr the heap memory to free
 *
 * \return none
 */
void free(void *ptr) 
{
   num_frees++;
   if (ptr == NULL) 
   {
      return;
   }

   /* Make _block as free */
   struct _block *curr = BLOCK_HEADER(ptr);
   assert(curr->free == 0);
   curr->free = true;


   /* TODO: Coalesce free _blocks.  If the next block or previous block 
            are free then combine them with this block being freed.
   */
  struct _block* coalEsced = heapList;
  while(coalEsced)
  {
   if(coalEsced->free && coalEsced->next && coalEsced->next->free) //find the previous
   {
      coalEsced->size += coalEsced->next->size + sizeof(struct _block);
      coalEsced->next = coalEsced->next->next;
      num_coalesces++;
      num_blocks--;
   }
   coalEsced = coalEsced->next;
  }
}

void *calloc( size_t nmemb, size_t size )
{
   void* ptr = malloc(nmemb*size);
   memset(ptr, 0, nmemb*size);
   return ptr;
}

void *realloc( void *ptr, size_t size )
{
   void* newPtr = malloc(size);
   memcpy(newPtr, ptr, size);
   free(ptr);
   return newPtr;
}



/* vim: IENTRTMzMjAgU3ByaW5nIDIwMjM= -----------------------------------------*/
/* vim: set expandtab sts=3 sw=3 ts=6 ft=cpp: --------------------------------*/
