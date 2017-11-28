#include "common.h"
#include "memlib.h"

#define MAX_HEAP (20 * (1 << 20))   /* 20 MB */

/* Private global variables */
static char *mem_heap;      /* Points to first byte of the heap */
static char *mem_brk;       /* Points to last byte of heap plus 1 */
static char *mem_max_addr;  /* Max legal heap addr plus 1 */

/**
 * mem_init - Initialize the memory system model.
 *
 *                            ^
 *                            | 
 *                            |
 *      mem_brk --> +++++++++++++++++++++
 *                  +                   +
 *                  +                   +
 *                  +                   +
 *                  +                   +
 *                  +      heap         +
 *                  +                   +
 *                  +                   +
 *                  +                   +
 *                  +                   +
 *                  +                   +
 *      mem_heap -->+++++++++++++++++++++   
 */
void mem_init(void)
{
    mem_heap = (char *) Malloc(MAX_HEAP);
    mem_brk = (char *)mem_heap;
    mem_max_addr = (char *)(mem_heap + MAX_HEAP);
}

/**
 * mem_sbrk - Simple model of the sbrk function. Extends the heap
 * by incr bytes and returns the start address of the new area.
 * In this model, the heap cannot be shrunk.
 *
 *                     BEFORE                      AFTER
 *                  +++++++++++++              ++++++++++++++
 *                  +           +              +            +
 *                  +           +              +            +
 *                  +           +              +------------+ <-- mem_brk
 *                  +           +              +   incr     +
 *  mem_brk -->     +-----------+              +............+ <-- returned pointer
 *                  +           +              +            +
 *                  +           +              +            +
 *  mem_heap -->    +++++++++++++              ++++++++++++++ <- mem_heap
 */
void *mem_sbrk(int incr)
{
    char *old_brk = mem_brk;
    
    if ((incr < 0) || ((mem_brk + incr) > mem_max_addr)) {
        errno = ENOMEM;
        fprintf(stderr, "ERROR: mem_brk failed. Ran out of memory...\n");
        return (void *)-1;
    }

    mem_brk += incr;
    return (void *)old_brk;
}

/**
 * mem_reset_brk - resets the simulated brk pointer to make an empty heap.
 */
void mem_reset_brk(void)
{
    mem_brk = (char *) mem_heap;
}

/**
 * mem_heap_lo - returns the address of the first heap byte.
 */
void *mem_heap_lo(void)
{
    return (void *)mem_heap;
}

/**
 * mem_heap_hi - return address of the last heap byte
 */
void *mem_heap_hi(void)
{
    return (void *) (mem_brk - 1);
}

/**
 * mem_heapsize - returns the heap size in bytes.
 */
size_t mem_heapsize(void)
{
    return (size_t)(mem_brk - mem_heap);
}

/**
 * mem_pagesize - returns the page size of the system.
 */
size_t mem_pagesize(void)
{
    return (size_t)getpagesize();
}
