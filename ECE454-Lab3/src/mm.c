/*
 * ECE454 Lab 3 - Malloc
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "维密天使",
    /* First member's first name and last name */
    "Weizhou:Wang",
    /* First member's student number */
    "1004421262",
    /* Second member's first name and last name (do not modify if working alone) */
    "",
    /* Second member's student number (do not modify if working alone) */
    ""
};

/*************************************************************************
 * Basic Constants and Macros
 * You are not required to use these macros but may find them helpful.
*************************************************************************/
#define WSIZE       sizeof(void *)            /* word size (bytes) */
#define DSIZE       (2 * WSIZE)            /* doubleword size (bytes) */
#define CHUNKSIZE   (1<<7)      /* initial heap size (bytes) */

#define MAX(x,y) ((x) > (y)?(x) :(y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)          (*(uintptr_t *)(p))
#define PUT(p,val)      (*(uintptr_t *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)     (GET(p) & ~(DSIZE - 1))
#define GET_ALLOC(p)    (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)        ((char *)(bp) - WSIZE)
#define FTRP(bp)        ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/**********************************************************
 * The number of classes in the free_list
 * 10 Classes, number are in words
 * 4,5-8,9-16,17-32,33-64,65-128,129-256,257-512,512-1024,1025-inf.
 **********************************************************/
#define NUM_CLASS 10

void* heap_listp = NULL;

/**********************************************************
 * free_block
 * Define the wrapper for a free block payload -> An element in a linked-list
 * Including the predecessor and successor
 **********************************************************/
typedef struct free_block {
    struct free_block* pre;  // Predecessor
    struct free_block* succ; // Successor
} free_block;
/* A pointer points to the start of the free list*/
free_block* free_list[NUM_CLASS];

/**********************************************************
 * mm_init
 * Initialize the heap, including "allocation" of the
 * prologue and epilogue
 **********************************************************/
int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1)
            return -1;
    PUT(heap_listp, 0);                         // alignment padding
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));   // prologue header
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));   // prologue footer
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));    // epilogue header
    heap_listp += DSIZE;
    for (int i = 0; i < NUM_CLASS; ++i) {
        free_list[i] = NULL;
    }

    return 0;
}

/**********************************************************
* round_up
* Round up a request size to 2^n size
* https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
**********************************************************/
size_t round_up(size_t asize)
{
    asize--;
    asize |= asize >> 1;
    asize |= asize >> 2;
    asize |= asize >> 4;
    asize |= asize >> 8;
    asize |= asize >> 16;
    asize++;

    return asize;
}

/**********************************************************
* locate_list
* Find the class index in free_list
**********************************************************/
int locate_list(size_t size)
{
    size = (size >> 3) - 1;
    int count = 0;
    while (size != 0) {
        size = size >> 1;
        ++count;
    }
    count -= 2;
    return count >= NUM_CLASS ? NUM_CLASS - 1 : count;
}

/**********************************************************
* add_to_list
* Add the free_block to the front of free_list
**********************************************************/
void add_to_list(free_block* bp)
{   
    int index = locate_list(GET_SIZE(HDRP(bp)));
    /* The list is empty */
    if (free_list[index] == NULL) {
        free_list[index] = bp;
        bp->pre = NULL;
        bp->succ = NULL;
    } else {
        free_list[index]->pre = bp;
        bp->pre = NULL;
        bp->succ = free_list[index];
        free_list[index] = bp;
    }

    return;
}

/**********************************************************
* remove_from_list
* Remove the free_block from the free_list
**********************************************************/
void remove_from_list(free_block* bp) {
    /* The block is at the front of the list */
    if (bp->pre == NULL) {
        int index = locate_list(GET_SIZE(HDRP(bp)));
        /* There is more than one element in the list */
        if (bp->succ != NULL) {
            bp->succ->pre = NULL;
            free_list[index] = bp->succ;
            return;
        } else {
            free_list[index] = NULL;
            return;
        }
    /* The block is at the end of the list */
    } else if (bp->succ == NULL) {
        bp->pre->succ = NULL;
        return;
    /* The block is in the middle of the list */
    } else {
        bp->pre->succ = bp->succ;
        bp->succ->pre = bp->pre;
        return;
    }
}

/**********************************************************
 * coalesce
 * Covers the 4 cases discussed in the text:
 * - both neighbours are allocated
 * - the next block is available for coalescing
 * - the previous block is available for coalescing
 * - both neighbours are available for coalescing
 **********************************************************/
void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {       /* Case 1 */
        return bp;
    }

    else if (prev_alloc && !next_alloc) { /* Case 2 */
        remove_from_list((free_block *)NEXT_BLKP(bp));
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        return (bp);
    }

    else if (!prev_alloc && next_alloc) { /* Case 3 */
        remove_from_list((free_block *)PREV_BLKP(bp));
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        return (PREV_BLKP(bp));
    }

    else {            /* Case 4 */
        remove_from_list((free_block *)PREV_BLKP(bp));
        remove_from_list((free_block *)NEXT_BLKP(bp));
        size += GET_SIZE(HDRP(PREV_BLKP(bp)))  +
            GET_SIZE(FTRP(NEXT_BLKP(bp)))  ;
        PUT(HDRP(PREV_BLKP(bp)), PACK(size,0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size,0));
        return (PREV_BLKP(bp));
    }
}

/**********************************************************
 * extend_heap
 * Extend the heap by "words" words, maintaining alignment
 * requirements of course. Free the former epilogue block
 * and reallocate its new header
 **********************************************************/
void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain alignments */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
    if ( (bp = mem_sbrk(size)) == (void *)-1 )
        return NULL;

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0));                // free block header
    PUT(FTRP(bp), PACK(size, 0));                // free block footer
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));        // new epilogue header

    /* Coalesce if the previous block was free */
    return coalesce(bp);
}


/**********************************************************
 * find_fit
 * Traverse the heap searching for a block to fit asize
 * Return NULL if no free blocks can handle that size
 * Assumed that asize is aligned
 **********************************************************/
void * find_fit(size_t asize)
{
    size_t index = locate_list(asize);
    
    if (free_list[index] != NULL) {
        /* Search appropriate free list for block */
        free_block* ans = NULL;
        size_t diff = SIZE_MAX;
        for (free_block* bp = free_list[index]; bp != NULL; bp = bp->succ) {
            if (asize <= GET_SIZE(HDRP(bp)) && GET_SIZE(HDRP(bp)) - asize < diff) {
                diff = GET_SIZE(HDRP(bp)) - asize;
                ans = bp;
            }
        }
        if (ans != NULL) {
            remove_from_list(ans);
            return ans;
        }
    }
    index += 1;
    /* If no block is found, try next larger class, repeat until found*/
    while (index < NUM_CLASS) {
        if (free_list[index] != NULL) {
            free_block* bp = free_list[index];
            /* There is more than one element in the list */
            if (bp->succ != NULL) {
                bp->succ->pre = NULL;
                free_list[index] = bp->succ;
            } else {
                free_list[index] = NULL;
            }
            return bp;
        }
        index += 1;
    }
    /* If still not found, grow heap */
    return NULL;
}

/**********************************************************
 * place
 * Mark the block as allocated
 **********************************************************/
void place(void* bp, size_t asize)
{
    /* Get the current block size */
    size_t bsize = GET_SIZE(HDRP(bp));

    size_t diff_size = bsize - asize;
    if (diff_size >= 2 * DSIZE) {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        PUT(HDRP(NEXT_BLKP(bp)), PACK(diff_size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(diff_size, 0));
        add_to_list((free_block*)NEXT_BLKP(bp));
    } else {
        PUT(HDRP(bp), PACK(bsize, 1));
        PUT(FTRP(bp), PACK(bsize, 1));
    }
}

/**********************************************************
 * mm_free
 * Free the block and coalesce with neighbouring blocks
 **********************************************************/
void mm_free(void *bp)
{
    if(bp == NULL){
      return;
    }

    size_t size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size,0));
    PUT(FTRP(bp), PACK(size,0));
    add_to_list(coalesce(bp));
}


/**********************************************************
 * mm_malloc
 * Allocate a block of size bytes.
 * The type of search is determined by find_fit
 * The decision of splitting the block, or not is determined
 *   in place(..)
 * If no block satisfies the request, the heap is extended
 **********************************************************/
void *mm_malloc(size_t size)
{
    size_t asize; /* adjusted block size */
    size_t extendsize; /* amount to extend heap if no fit */
    char * bp;

    /* Ignore spurious requests */
    if (size == 0)
        return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= DSIZE) {
        asize = 2 * DSIZE;
    } else {
        asize = DSIZE * ((size + (DSIZE) + (DSIZE-1))/ DSIZE);
    }

    if (asize <= (1 << 8)) {
        asize = round_up(asize);
    }
        
    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
        return NULL;
    place(bp, asize);
    return bp;

}

/**********************************************************
 * mm_realloc
 * Implemented simply in terms of mm_malloc and mm_free
 *********************************************************/
void *mm_realloc(void *oldptr, size_t size)
{
    /* If size == 0 then this is just free, and we return NULL. */
    if(size == 0){
      mm_free(oldptr);
      return NULL;
    }
    /* If oldptr is NULL, then this is just malloc. */
    if (oldptr == NULL)
      return (mm_malloc(size));

    void *newptr;
    size_t copySize = GET_SIZE(HDRP(oldptr));

    /* Adjust block size to include overhead and alignment reqs. */
    size_t asize;
    if (size <= DSIZE)
        asize = 2 * DSIZE;
    else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE-1))/ DSIZE);

    /* If enlarging the block */
    if (asize > copySize) {
        /* First try to coalesce */
        void* temp_ptr = coalesce(oldptr);
        if (GET_SIZE(HDRP(temp_ptr)) >= asize) {
            memmove(temp_ptr, oldptr, copySize - DSIZE);
            place(temp_ptr, GET_SIZE(HDRP(temp_ptr)));
            return temp_ptr;
        }
        /* If cannot, have to malloc new space*/
        newptr = mm_malloc(size);
        if (newptr == NULL)
            return NULL;
        memcpy(newptr, oldptr, copySize - DSIZE);
        add_to_list((free_block*)temp_ptr);
        return newptr;
    /* If shrinking the block */
    } else {
        return oldptr;
    }
}

/**********************************************************
 * mm_check
 * Check the consistency of the memory heap
 * Return nonzero if the heap is consistant.
 *********************************************************/
int mm_check(void){
  return 1;
}
