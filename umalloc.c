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

#define UMALLOC_MIN_BLOCK_SIZE  ( ( size_t )( u_block_link_size << 1 ) )

static u_heap_link_t u_static_links[UMALLOC_N_HEAPS] = {0};

static size_t u_allocation_bit = ( (size_t) 1 ) << ( ( sizeof( size_t ) * UMALLOC_BITS_PER_BYTE ) - 1 );

static void u_link_free_block(u_heap_link_t* u_link, u_block_link_t* u_block)
{
    u_block_link_t* u_iterator;
    uint8_t* u_addr;

    /*
     * Iterate through the links until an address higher than the one of the 
     * given block is found. 
     */
    for(u_iterator = &u_link->u_start; u_iterator->u_next_free < u_block; u_iterator = u_iterator->u_next_free );

    /* Is the block being linked and the block linked after contiguous? */
    u_addr = (uint8_t *) u_iterator;
    if( ( u_addr + u_iterator->u_block_size ) == ( ( uint8_t*) u_block ) )
    {
        u_iterator->u_block_size += u_block->u_block_size;
        u_block = u_iterator;
    }
    
    /* Is the block being linked and the one linked before contiguous? */
    u_addr = (uint8_t *) u_block;
    if( ( u_addr + u_block->u_block_size ) == ( ( uint8_t* ) u_iterator->u_next_free ) )
    {
        if( u_link->u_end == u_iterator->u_next_free )
        {
            u_block->u_next_free = u_link->u_end;
        }
        else
        {
            u_block->u_block_size += u_iterator->u_next_free->u_block_size;
            u_block->u_next_free = u_iterator->u_next_free->u_next_free;
        }
    }
    else
    {
        u_block->u_next_free = u_iterator->u_next_free;
    }

    if( u_iterator != u_block )
    {
        u_iterator->u_next_free = u_block;
    }

    return;
}

u_heap_id_t u_create(void* addr, size_t heap_size)
{
    u_heap_id_t     u_heap_idx = 0;
    u_heap_link_t   *u_first_free_link = u_static_links;
    u_block_link_t  *u_first_free_block;
    uint8_t         *u_aligned_addr;
    size_t          u_addr;
    size_t          u_total_heap_size = heap_size;

    /* Search for available links */
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

void* u_malloc(u_heap_id_t heap_id, size_t req_size)
{
    void* u_block_ptr = NULL;

    /* Is heap ID not valid? */
    if( ( 0 > heap_id ) || ( UMALLOC_MAX_HEAPS < heap_id ) )
    {
        return u_block_ptr;
    }

    u_heap_link_t  *u_link = &u_static_links[heap_id];
    u_block_link_t *u_block, *u_prev_block, *u_new_block;

    /*
     * Check if the requested size is valid and can fit the
     * allocation control bit.
     */
    if( ( 0 == ( req_size & u_allocation_bit ) ) && ( 0 < req_size ) )
    {   
        /* Perform necessary corrections on requested size. */
        req_size += u_block_link_size;
        if( 0 != ( req_size & UMALLOC_BYTE_ALIGN_MASK ) )
        {
            req_size += ( UMALLOC_BYTE_ALIGNMENT - ( req_size & UMALLOC_BYTE_ALIGN_MASK ) );
        }

        /* Is requested size possible to fit in ? */
        if( req_size <= u_link->u_free_bytes )
        {
            u_prev_block    = &u_link->u_start;
            u_block         = u_link->u_start.u_next_free;

            while( ( u_block->u_block_size < req_size ) && ( NULL != u_block->u_next_free ) )
            {
                u_prev_block    = u_block;
                u_block         = u_block->u_next_free;
            }

            /* Have we cycled through the entire list? */
            if( u_link->u_end != u_block )
            {
                /* If we are here it means a suitable block has been found. */
                u_block_ptr = ( void* )( ( ( uint8_t* ) u_prev_block->u_next_free ) + u_block_link_size );
                u_prev_block->u_next_free = u_block->u_next_free;

                if(  UMALLOC_MIN_BLOCK_SIZE < ( u_block->u_block_size - req_size ) )
                {
                    u_new_block = ( void* )( ( ( uint8_t* ) u_block ) + req_size );
                    u_new_block->u_block_size = u_block->u_block_size - req_size;
                    u_block->u_block_size = req_size;

                    /* Insert block into list of free links */
                    u_link_free_block(u_link, u_new_block);
                }

                u_link->u_free_bytes -= u_block->u_block_size;
                if ( u_link->u_free_bytes < u_link->u_remain_bytes )
                {
                    u_link->u_remain_bytes = u_link->u_free_bytes;
                }

                u_block->u_block_size |= u_allocation_bit;
                u_block->u_next_free = NULL;
            }
        }
    }
    return u_block_ptr;
}

void u_free(u_heap_id_t heap_id, void* addr)
{
    uint8_t* u_addr = (uint8_t *) addr;
    u_block_link_t *u_block;
    u_heap_link_t  *u_link = &u_static_links[heap_id];

    if( NULL != addr )
    {
        u_addr -= u_block_link_size;
        u_block = (void *) u_addr;
        
        if( ( 0 != ( u_block->u_block_size & u_allocation_bit ) ) && 
            ( NULL == u_block->u_next_free ) )
        {
            u_block->u_block_size &= ~(u_allocation_bit);
            u_link->u_free_bytes += u_block->u_block_size;
            u_link_free_block(u_link, u_block);
        }
    }
    return;
}



