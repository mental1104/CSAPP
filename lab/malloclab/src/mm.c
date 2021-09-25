/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "EEVEE",
    /* First member's full name */
    "Blue Espeon",
    /* First member's email address */
    "mental1104@gmail.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1<<12) /* Extend heap by this amount (bytes) */
#define MINBLOCKSIZE 16

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define PACK(size, alloc) ((size) | (alloc)) /* Pack a size and allocated bit into a word */

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))
/* Read the size and the alloc field field from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
/* Given block ptr bp, compute address of its header and footer*/
#define HDRP(bp) ((char*)(bp) - WSIZE)
#define FTRP(bp) ((char*)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)
/* Given block ptr bp, compute address of next and prev blocks */
#define NEXT_BLKP(bp) ((char*)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((char*)(bp) - GET_SIZE((char*)(bp)-DSIZE))
/* Get and set prev or next pointer from address p */
#define GET_PREV(p) (*(unsigned int *)(p))
#define SET_PREV(p, prev) (*(unsigned int *)(p) = (prev))
#define GET_NEXT(p) (*((unsigned int *)(p)+1))
#define SET_NEXT(p, val) (*((unsigned int *)(p)+1) = (val))

static char* heap_listp = NULL;
static char* block_list_start = NULL;

static void* extend_heap(size_t words);
void* get_freelisthead(size_t size);
static void remove_from_free_list(void *bp);
static void insert_to_free_list(void* bp);
static void* coalesce(void *bp);
static void* find_fit(size_t asize);
static void place(void* bp, size_t asize);

void* get_freelisthead(size_t size){
    int temp = 0;
    if(size <= 32) temp = 0;
    else if(size <= 64) temp = 1;
    else if(size <= 128) temp = 2;
    else if(size <= 256) temp = 3;
    else if(size <= 512) temp = 4;
    else if(size <= 1024) temp = 5;
    else if(size <= 2048) temp = 6;
    else if(size <= 4096) temp = 7;
    else temp = 8;

    //Locate the appropriate root of segregate list. 
    return block_list_start + (temp*WSIZE);

}

static void* extend_heap(size_t words){
    char* bp;
    size_t size;

    //Allocate an even number of words to maintain alignment 
    size = (words%2) ? (words+1)*WSIZE: words*WSIZE;
    if((long)(bp = mem_sbrk(size)) == -1)
        return NULL;
    
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));

    SET_PREV(bp, NULL);
    SET_NEXT(bp, NULL);

    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

    return coalesce(bp);
}

static void remove_from_free_list(void *bp){
    if(bp == NULL || GET_ALLOC(HDRP(bp)))
        return;
    
    void* root = get_freelisthead(GET_SIZE(HDRP(bp)));
    void* prev = GET_PREV(bp);
    void* next = GET_NEXT(bp);

    SET_PREV(bp, NULL);//delete the contents of bp
    SET_NEXT(bp, NULL);

    if(prev == NULL){
        if(next!=NULL) SET_PREV(next, NULL);
        PUT(root, next);
    }
    else{
        if(next!=NULL) SET_PREV(next, prev);
        SET_NEXT(prev, next);
    }
}

static void insert_to_free_list(void* bp){
    if(bp == NULL)
        return;
    
    void* root = get_freelisthead(GET_SIZE(HDRP(bp)));
    void* prev = root;
    void* next = GET(root);

    while(next!=NULL){
        if(GET_SIZE(HDRP(next))>=GET_SIZE(HDRP(bp)))
            break;
        prev =  next;
        next = GET_NEXT(next);
    }

    if(prev == root){
        PUT(root, bp);
        SET_PREV(bp, NULL);
        SET_NEXT(bp, next);
        if(next!=NULL) SET_PREV(next, bp);
    }
    else{
        SET_NEXT(bp, next);
        SET_NEXT(prev, bp);
        SET_PREV(bp, prev);
        if(next!=NULL) SET_PREV(next, bp);
    }
}

static void* coalesce(void *bp){
    void* prev_bp = PREV_BLKP(bp);
    void* next_bp = NEXT_BLKP(bp);
    size_t prev_alloc = GET_ALLOC(FTRP(prev_bp));
    size_t next_alloc = GET_ALLOC(HDRP(next_bp));

    size_t size = GET_SIZE(HDRP(bp));

    if(prev_alloc && next_alloc)
        ;//Do nothing.
    else if(prev_alloc && !next_alloc){
        remove_from_free_list(next_bp);
        size += GET_SIZE(HDRP(next_bp));
        PUT(HDRP(bp), PACK(size,0));
        PUT(FTRP(bp), PACK(size,0));
    }
    else if(!prev_alloc && next_alloc){
        remove_from_free_list(prev_bp);
        size += GET_SIZE(HDRP(prev_bp));
        PUT(FTRP(bp), PACK(size,0));//Put foot first, otherwise it will overwrite some information.
        PUT(HDRP(prev_bp), PACK(size,0));

        bp = prev_bp;//new block comes in the prev block.
    }
    else{
        remove_from_free_list(prev_bp);
        remove_from_free_list(next_bp);
        size += GET_SIZE(HDRP(prev_bp)) + GET_SIZE(HDRP(next_bp));//two free block
        PUT(FTRP(next_bp), PACK(size,0));
        PUT(HDRP(prev_bp), PACK(size,0));

        bp = prev_bp;
    }
    insert_to_free_list(bp);
    return bp;

}

static void* find_fit(size_t asize){
    for(void* root = get_freelisthead(asize); root!=(heap_listp-WSIZE); root+=WSIZE){
        void* bp = GET(root);
        while(bp){
            if(GET_SIZE(HDRP(bp))>=asize) return bp;
            bp = GET_NEXT(bp);
        }
    }
    return NULL;
}

static void place(void* bp, size_t asize){
    size_t csize = GET_SIZE(HDRP(bp));
    remove_from_free_list(bp);
    if((csize - asize) >= MINBLOCKSIZE){
        PUT(HDRP(bp), PACK(asize,1));
        PUT(FTRP(bp), PACK(asize,1));
        void* new_bp = NEXT_BLKP(bp);
        PUT(HDRP(new_bp), PACK(csize-asize,0));
        PUT(FTRP(new_bp), PACK(csize-asize,0));
        SET_PREV(new_bp, 0);
        SET_NEXT(new_bp, 0);
        coalesce(new_bp);
    }else{
        PUT(HDRP(bp), PACK(csize,1));
        PUT(FTRP(bp), PACK(csize,1));
    }
}
/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if((heap_listp = mem_sbrk(12*WSIZE)) == (void *)-1)
        return -1;
    //Segregate lists.
    PUT(heap_listp, 0);//(0,32)
    PUT(heap_listp+(1*WSIZE), 0);//(33, 64)
    PUT(heap_listp+(2*WSIZE), 0);//(65, 128)
    PUT(heap_listp+(3*WSIZE), 0);//(129, 256)
    PUT(heap_listp+(4*WSIZE), 0);//(257, 512)
    PUT(heap_listp+(5*WSIZE), 0);//(513, 1024)
    PUT(heap_listp+(6*WSIZE), 0);//(1025, 2048)
    PUT(heap_listp+(7*WSIZE), 0);//(2049, 4096)
    PUT(heap_listp+(8*WSIZE), 0);//(4097, +inf)
    //Prologue block
    PUT(heap_listp+(9*WSIZE), PACK(DSIZE,1));
    PUT(heap_listp+(10*WSIZE), PACK(DSIZE,1));
    //Epilogue block
    PUT(heap_listp+(11*WSIZE), PACK(0,1));

    block_list_start = heap_listp;
    heap_listp += (10*WSIZE);

    if(extend_heap(CHUNKSIZE/WSIZE) == NULL)
        return -1;
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize;
    size_t extendsize;
    char *bp;

    /* Ignore spurious requests */
    if(size == 0)
        return NULL;
    
    /* Adjust block size to include overhead and alignment reqs. */
    if(size <= DSIZE)
        asize = 2*DSIZE;
    else    
        asize = DSIZE * ((size + (DSIZE) + (DSIZE-1))/DSIZE);
    
    /* Search the free list for a fit */
    if((bp = find_fit(asize))!= NULL){
        place(bp,asize);
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize, CHUNKSIZE);
    if((bp = extend_heap(extendsize/WSIZE)) == NULL)
        return NULL;
    place(bp, asize);
    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    if(ptr == NULL)
        return;
    size_t size = GET_SIZE(HDRP(ptr));
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));

    SET_PREV(ptr, NULL);
    SET_NEXT(ptr, NULL);

    coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    size = GET_SIZE(HDRP(oldptr));
    copySize = GET_SIZE(HDRP(newptr));
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize-WSIZE);
    mm_free(oldptr);
    return newptr;
}














