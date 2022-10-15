/**
 * @file    umalloc.c
 * @author  Antonio Vitor Grossi Bassi (antoniovitor.gb@gmail.com)
 * @brief   umalloc source code.
 * @version alpha - 0.1
 * @date    2022-10-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "ualign.h"
#include "umalloc.h"

static const size_t u_block_link_size = ( ( sizeof(u_block_link_t) + (size_t)( UMALLOC_BYTE_ALIGNMENT - 1 ) ) ) & ~( ( size_t ) UMALLOC_BYTE_ALIGN_MASK );

#define UMALLOC_MIN_BLOCK_SIZE  ( ( size_t )( u_block_link_size << 1 ) );

static u_heap_link_t u_static_links[UMALLOC_N_HEAPS] = {0};


u_heap_id_t u_create(void* addr, size_t heap_size)
{
    u_heap_id_t     u_heap_idx = 0;
    u_heap_link_t*  u_first_free_link = u_static_links;
    u_block_link_t* u_first_free_block;
    uint8_t*        u_aligned_addr;
    size_t          u_addr;
    size_t          u_total_heap_size = heap_size;

    // Search for available links
    for(u_heap_idx = 0; u_heap_idx < UMALLOC_N_HEAPS; u_heap_idx++)
    {
        if( NULL == u_first_free_link[u_heap_idx].u_end )
        {
            break;   
        }
    }
    
    /*
     * No links are available at the moment.
     * This means the maximum number of manageable heaps was reached.
     * Therefore return an invalid heap id.
     */
    if( u_heap_idx >= UMALLOC_N_HEAPS )
    {
        u_heap_idx = -1;
        return u_heap_idx;
    }

    /*
     * The address must be a aligned. 
     * Perform necessary corrections
     */
    u_addr = (size_t)( addr );
    if( 0 != ( u_addr & UMALLOC_BYTE_ALIGN_MASK ) )
    {
        u_addr += (UMALLOC_BYTE_ALIGNMENT - 1);
        u_addr &= ~( (size_t) UMALLOC_BYTE_ALIGN_MASK );
        u_total_heap_size -= u_addr - ( (size_t) addr ); 
    }
    u_aligned_addr = (uint8_t*) u_addr;

    u_first_free_link[u_heap_idx].u_start.u_next_free   = (void*) u_aligned_addr;
    u_first_free_link[u_heap_idx].u_start.u_block_size  = (size_t) 0;

    u_addr = ( (size_t) u_aligned_addr ) + u_total_heap_size;
    u_addr -= u_block_link_size;
    u_addr &= ~( (size_t) UMALLOC_BYTE_ALIGN_MASK );

    u_first_free_link[u_heap_idx].u_end = (void*) u_addr;
    u_first_free_link[u_heap_idx].u_end->u_block_size = 0;
    u_first_free_link[u_heap_idx].u_end->u_next_free = NULL;

    u_first_free_block                  = (void*) u_aligned_addr;
    u_first_free_block->u_block_size    = u_addr - ( (size_t) u_first_free_block );
    u_first_free_block->u_next_free     = u_first_free_link[u_heap_idx].u_end;

    u_first_free_link[u_heap_idx].u_free_bytes      = u_first_free_block->u_block_size;
    u_first_free_link[u_heap_idx].u_remain_bytes    = u_first_free_block->u_block_size;

    return u_heap_idx;
}

void* u_malloc(u_heap_id_t heap_id, size_t size)
{
    void* u_block_ptr = NULL;
    if( ( 0 > heap_id ) || ( UMALLOC_MAX_HEAPS < heap_id ) )
    {
        return u_block_ptr;
    }

    u_block_link_t* u_heap = &u_static_links[heap_id].u_start;
    


}



