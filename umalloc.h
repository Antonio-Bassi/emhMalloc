/**
 * @file    umalloc.h
 * @author  Antonio Vitor Grossi Bassi (antoniovitor.gb@gmail.com)
 * @brief   umalloc core function set.
 * @version alpha
 * @date    2022-10-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#define UMALLOC_BITS_PER_BYTE   8
#define UMALLOC_MAX_HEAPS       0x60
#define UMALLOC_N_HEAPS         4

#if !defined(UMALLOC_N_HEAPS)
#define uNUM_OF_HEAPS 2
#elif   UMALLOC_N_HEAPS > UMALLOC_MAX_HEAPS
#undef  UMALLOC_N_HEAPS
#define UMALLOC_N_HEAPS 2
#warning "umalloc error, number of heaps macro exceeds the maximum, macro redefined to 2"
#endif /* uNUM_OF_HEAPS */

typedef signed char u_heap_id_t;

typedef struct u_block_link_t
{
    size_t u_block_size;
    struct u_block_link_t* u_next_free;
}u_block_link_t;

typedef struct u_heap_link_t
{
    u_block_link_t  u_start;
    u_block_link_t* u_end;
    size_t          u_free_bytes;
    size_t          u_remain_bytes;   
}u_heap_link_t;

u_heap_id_t u_create(void* addr, size_t heap_size);
void*       u_malloc(u_heap_id_t heap_id, size_t size);
void*       u_calloc(u_heap_id_t heap_id, size_t size);
void*       u_realloc(u_heap_id_t heap_id, size_t size);
void        u_free(u_heap_id_t heap_id, void* addr);