/*****************************************************************************************
 * memlib.h - A module that simulates the memory system.
 ****************************************************************************************/

/**
 * mem_init - Initialize the memory system model
 */
void mem_init(void);

/**
 * mem_sbrk - Simple model of the sbrk function. Extends the heap by
 * @incr bytes and returns the start address of the new area.
 * In this model, the heap cannot be shrunk.
 */
void mem_sbrk(int incr);

/**
 * mem_reset_brk - Resets the simulated brk pointer to make an empty heap.
 */
void mem_reset_brk(void);

/**
 * mem_heap_lo - returns the address of the first heap byte.
 */
void *mem_heap_lo(void);

/**
 * mem_heap_hi - returns the address of the last heap byte.
 */
void *mem_heap_hi(void);

/**
 * mem_heapsize - returns the heap size in bytes.
 */
size_t mem_heapsize(void);

/**
 * mem_pagesize - returns the page size of the system.
 */
size_t mem_pagesize(void);
