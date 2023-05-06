/**
 *  emh_malloc
 *  Copyright (C) 2022, Antonio Vitor Grossi Bassi
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * @file    emh_malloc.c
 * @author  Antonio Vitor Grossi Bassi (antoniovitor.gb@gmail.com)
 * @brief   umalloc source code.
 * @version 1.6
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

#include "emh_align.h"
#include "emh_malloc.h"
#include "emh_port.h"

static const size_t emh_blockLinkSize = ( ( sizeof(emh_blockLink_t) + (size_t)( EMH_MALLOC_BYTE_ALIGNMENT - 1 ) ) ) & ~( ( size_t ) EMH_MALLOC_BYTE_ALIGN_MASK );

static emh_heapLink_t emh_heapLinks[EMH_MALLOC_N_HEAPS] = {0};

static size_t emh_allocBit  = ( (size_t) 1 ) << ( ( sizeof( size_t ) * EMH_MALLOC_BITS_PER_BYTE ) - 16 );
static size_t emh_heapIdMsk = ( (size_t) EMH_MALLOC_HEAP_ID_BITMASK ) << ( ( sizeof( size_t ) * EMH_MALLOC_BITS_PER_BYTE ) - 15 );

#define EMH_MALLOC_MIN_BLOCK_SIZE  ( ( size_t )( emh_blockLinkSize << 1 ) )

#ifndef __emh_create_zone__
#error "emh_malloc: ERROR -> No semaphore or mutex initialisation function was defined. Check emh_malloc/emh_port.h"
#endif /* __emh_create_zone__ */

#ifndef __emh_lock_zone__
#error  "emh_malloc: ERROR -> No semaphore or mutex locking function was defined. Check emh_malloc/emh_port.h"
#endif /* __emh_lock_zone__ */

#ifndef __emh_unlock_zone__
#error "emh_malloc: ERROR -> No semaphore or mutex realeasing function was defined. emh_malloc/emh_port.h"
#endif /* __emh_unlock_zone__ */

/**
 * @brief Packs heap id information by casting it into a size_t and shifting bits
 *        to the appropriate region.
 * @param heapId heap id to be packed into a size_t variable.
 * @return size_t 
 */
static size_t emh_packHeapId(emh_heapId_t heapId)
{
    return ( ( size_t )( heapId & EMH_MALLOC_HEAP_ID_BITMASK ) ) << ( ( sizeof( size_t ) * EMH_MALLOC_BITS_PER_BYTE ) - 15 );
}

/**
 * @brief Unpacks heap id information from a size_t variable. By shifting it back to the original
 *        position and casting it to a emh_heapId_t
 * @param emh_blockSize 
 * @return emh_heapId_t 
 */
static emh_heapId_t emh_unpackHeapId(size_t emh_blockSize)
{
    return ( emh_heapId_t )( ( ( emh_blockSize ) >> ( ( sizeof( size_t ) * EMH_MALLOC_BITS_PER_BYTE ) - 15 ) ) & ( EMH_MALLOC_HEAP_ID_BITMASK ) );
}

/**
 * @brief Performs memory block coalescing by linking a free block to the rest of the heap.
 * @param emh_heap  Pointer to a heap link.
 * @param emh_block Pointer of a free block to be linked.
 */
static void emh_linkFreeBlock(emh_heapLink_t *emh_heap, emh_blockLink_t *emh_block)
{
    emh_blockLink_t *iterator;
    uint8_t *addr;

    /*
     * Iterate through the links until an address higher than the one of the 
     * given block is found. 
     */
    for(iterator = &emh_heap->start; iterator->nextFree < emh_block; iterator = iterator->nextFree );

    /* Is the block being linked and the block linked after contiguous? */
    addr = (uint8_t *) iterator;
    if( ( addr + iterator->blockSize ) == ( ( uint8_t*) emh_block ) )
    {
        iterator->blockSize += emh_block->blockSize;
        emh_block = iterator;
    }
    
    /* Is the block being linked and the one linked before contiguous? */
    addr = (uint8_t *) emh_block;
    if( ( addr + emh_block->blockSize ) == ( ( uint8_t* ) iterator->nextFree ) )
    {
        /* Are we at the end of the heap? */
        if( emh_heap->end == iterator->nextFree )
        {
            emh_block->nextFree = emh_heap->end;
        }
        else
        {
            emh_block->blockSize += iterator->nextFree->blockSize;
            emh_block->nextFree  = iterator->nextFree->nextFree;
        }
    }
    else
    {
        emh_block->nextFree = iterator->nextFree;
    }

    if( iterator != emh_block )
    {
        iterator->nextFree = emh_block;
    }
    return;
}

/**
 * @brief Initialises heap space and links it into static links array.
 * @param heapAddr First memory address from the heap region.
 * @param heapSize Size of the heap memory region.
 * @return emh_heapId_t heap identifier, which is an index from u_static_links array.
 */
emh_heapId_t emh_create(void* heapAddr, size_t heapSize)
{
    static int      emh_critZoneInit = 0;
    emh_heapId_t    emh_heapIdx = 0;
    emh_heapLink_t  *emh_stFreeLink = emh_heapLinks;
    emh_blockLink_t *emh_stFreeBlock;
    uint8_t         *alignedAddr;
    size_t          unsLongAddr;
    size_t          totHeapSize = heapSize;

    /* Initialises mutex */
    if( 0 == emh_critZoneInit )
    {
    	__emh_create_zone__();
    	emh_critZoneInit = 1;
    }
    
    __emh_lock_zone__();
    /* Search for available links */
    for(emh_heapIdx = 0; emh_heapIdx < EMH_MALLOC_N_HEAPS; emh_heapIdx++)
    {
        if( NULL == emh_stFreeLink[emh_heapIdx].end )
        {
            break;   
        }
    }
    
    /*
     * No links are available at the moment.
     * This means the maximum number of manageable heaps was reached.
     * Therefore return an invalid heap id.
     */
    if( emh_heapIdx >= EMH_MALLOC_N_HEAPS )
    {
        emh_heapIdx = -1;
        return emh_heapIdx;
    }

    /*
     * The address must be aligned. 
     * Perform necessary corrections.
     */
    unsLongAddr = (size_t)( heapAddr );
    if( 0 != ( unsLongAddr & EMH_MALLOC_BYTE_ALIGN_MASK ) )
    {
        unsLongAddr += (EMH_MALLOC_BYTE_ALIGNMENT - 1);
        unsLongAddr &= ~( (size_t) EMH_MALLOC_BYTE_ALIGN_MASK );
        totHeapSize -= unsLongAddr - ( (size_t) heapAddr ); 
    }
    alignedAddr = (uint8_t*) unsLongAddr;

    emh_stFreeLink[emh_heapIdx].start.nextFree   = (void*) alignedAddr;
    emh_stFreeLink[emh_heapIdx].start.blockSize  = (size_t) 0;

    unsLongAddr = ( (size_t) alignedAddr ) + totHeapSize;
    unsLongAddr -= emh_blockLinkSize;
    unsLongAddr &= ~( (size_t) EMH_MALLOC_BYTE_ALIGN_MASK );

    emh_stFreeLink[emh_heapIdx].end = (void*) unsLongAddr;
    emh_stFreeLink[emh_heapIdx].end->blockSize = 0;
    emh_stFreeLink[emh_heapIdx].end->nextFree = NULL;

    emh_stFreeBlock             = (void*) alignedAddr;
    emh_stFreeBlock->blockSize  = unsLongAddr - ( (size_t) emh_stFreeBlock );
    emh_stFreeBlock->nextFree   = emh_stFreeLink[emh_heapIdx].end;

    emh_stFreeLink[emh_heapIdx].freeBytes      = emh_stFreeBlock->blockSize;
    emh_stFreeLink[emh_heapIdx].remainBytes    = emh_stFreeBlock->blockSize;
    __emh_unlock_zone__();

    return emh_heapIdx;
}

/**
 * @brief Allocates memory region into designated heap space specified by heapId
 *        and returns a memory aligned pointer to allocated memory area.
 * 
 * @param heapId Id number of the heap memory region to be used.
 * @param size    Size of memory to be allocated from the heap.
 * @return void* 
 */
void* emh_malloc(emh_heapId_t heapId, size_t size)
{
    __emh_lock_zone__();
    void* addr = NULL;

    /* Is heap ID not valid? */
    if( ( 0 > heapId ) || ( EMH_MALLOC_MAX_HEAPS < heapId ) )
    {
        return addr;
    }

    emh_heapLink_t  *emh_link = &emh_heapLinks[heapId];
    emh_blockLink_t *block, *prevBlock, *newBlock;

    /*
     * Check if the requested size is valid and can fit the
     * allocation control bit.
     */
    if( ( 0 == ( size & emh_allocBit ) ) && ( 0 < size ) )
    {   
        /* Perform necessary corrections on requested size. */
        size += emh_blockLinkSize;
        if( 0 != ( size & EMH_MALLOC_BYTE_ALIGN_MASK ) )
        {
            size += ( EMH_MALLOC_BYTE_ALIGNMENT - ( size & EMH_MALLOC_BYTE_ALIGN_MASK ) );
        }

        /* Is requested size possible to fit in ? */
        if( size <= emh_link->freeBytes )
        {
            prevBlock = &emh_link->start;
            block     = emh_link->start.nextFree;

            while( ( block->blockSize < size ) && ( NULL != block->nextFree ) )
            {
                prevBlock    = block;
                block        = block->nextFree;
            }

            /* Have we cycled through the entire list? */
            if( emh_link->end != block )
            {
                /* If we are here it means a suitable block has been found. */
                addr = ( void* )( ( ( uint8_t* ) prevBlock->nextFree ) + emh_blockLinkSize );
                prevBlock->nextFree = block->nextFree;

                if(  EMH_MALLOC_MIN_BLOCK_SIZE < ( block->blockSize - size ) )
                {
                    newBlock = ( void* )( ( ( uint8_t* ) block ) + size );
                    newBlock->blockSize = block->blockSize - size;
                    block->blockSize = size;

                    /* Insert block into list of free links */
                    emh_linkFreeBlock(emh_link, newBlock);
                }

                emh_link->freeBytes -= block->blockSize;
                if ( emh_link->freeBytes < emh_link->remainBytes )
                {
                    emh_link->remainBytes = emh_link->freeBytes;
                }

                block->blockSize |= emh_allocBit;
                block->blockSize |= emh_packHeapId(heapId);
                block->nextFree = NULL;
            }
        }
    }
    __emh_unlock_zone__();
    return addr;
}

/**
 * @brief Frees allocated memory region from heap and updates metadata.
 * 
 * @param addr Address of the memory region to be freed.
 */
void emh_free(void* addr)
{
    uint8_t *emh_addr = (uint8_t *) addr;
    emh_blockLink_t *emh_block;
    emh_heapId_t heapId;
    emh_heapLink_t *emh_link;

    if( NULL != addr )
    {
        emh_addr -= emh_blockLinkSize;
        emh_block = (void *) emh_addr;
        heapId = emh_unpackHeapId(emh_block->blockSize);
      
        __emh_lock_zone__();
        /*
         * [1.] Is the block allocated?
         * [2.] Is the next block pointer NULL?
         * [3.] Is the heapId valid? 
         */
        if( ( 0 != ( emh_block->blockSize & emh_allocBit ) ) &&
            ( NULL == emh_block->nextFree ) &&
            ( EMH_MALLOC_N_HEAPS > heapId) &&
            ( 0 <= heapId ) )
        {
            emh_link = &emh_heapLinks[heapId];
            emh_block->blockSize &= ~(emh_allocBit | emh_heapIdMsk);
            emh_link->freeBytes += emh_block->blockSize;
            emh_linkFreeBlock(emh_link, emh_block);
        }
        __emh_unlock_zone__();
    }
    return;
}

/**
 * @brief   Allocates memory for an array of *n* elements with given
 *          *size* and initializes all bytes in the allocated storage to 
 *          zero. 
 * 
 * @param heapId   Id number of the heap memory region to be used.
 * @param n         Number of elements.
 * @param size      Individual element size.
 * @return void* 
 */
void* emh_calloc(emh_heapId_t heapId, size_t n, size_t size)
{
    void *addr = NULL;
    size_t totSize = (size_t)( n * size );

    totSize += emh_blockLinkSize;
    if( 0 != ( totSize & EMH_MALLOC_BYTE_ALIGN_MASK ) )
    {
        totSize += ( EMH_MALLOC_BYTE_ALIGNMENT - ( totSize & EMH_MALLOC_BYTE_ALIGN_MASK ) );
    }

    addr = emh_malloc(heapId, totSize);

    if( NULL != addr )
    {
        memset(addr, 0x00, totSize);
    }

    return addr;
}

/**
 * @brief   Reallocates given address memory to a different size.
 *          If there is no sufficient space on the heap for specified
 *          size u_realloc will return a NULL pointer. There is no con-
 *          -traction nor expansion of a memory region, instead u_realloc
 *          looks in the free block list the requested memory size and if
 *          successful it will copy the information from the given address
 *          to the newly allocated one, free the given address and then 
 *          return a pointer to the memory region.
 *          
 *          If the requested size is smaller than the given block information 
 *          will be truncated during copy. Otherwise the information will be 
 *          fully copied.
 * 
 * @param addr Address of the memory region to be reallocated.
 * @param size Requested size of new memory region.
 * @return void* 
 */
void* emh_realloc(void *addr, size_t size)
{
    void *emh_addr = NULL;
    uint8_t *byteAddr = (uint8_t *) addr;
    size_t blockSize = 0;
    emh_blockLink_t *block;
    emh_heapId_t heapId;   

    /* Is given pointer valid? */
    if( NULL != addr )
    {
        /* 
         * Are we reallocating to a size of 0 bytes? 
         * Might as well just free the block... 
         */
        if( 0 == size )
        {
            emh_free(addr);
            return emh_addr;
        }

        /* Extract heap block information */
        byteAddr -= emh_blockLinkSize;
        block = (void *) byteAddr;
        heapId = emh_unpackHeapId(block->blockSize);
        blockSize =  ( block->blockSize & ~(emh_allocBit | emh_heapIdMsk) );

        /* Align requested size before allocating */
        if( 0 != ( size & EMH_MALLOC_BYTE_ALIGN_MASK ) )
        {
            size += ( EMH_MALLOC_BYTE_ALIGNMENT - ( size & EMH_MALLOC_BYTE_ALIGN_MASK ) );
        }           

        /*
         * If requested size is the same as target block's 
         * No need to reallocate, just return given address.
         */
        if( size == blockSize )
        {
            emh_addr = addr;
            return emh_addr;
        }

        emh_addr = emh_malloc(heapId, size);

        /* Was allocation successful? */
        if( NULL != emh_addr )
        {
            /* is new allocated area greater or smaller than previous? */
            if( size > blockSize )
            {
                memcpy(emh_addr, addr, blockSize);
                emh_free(addr);
            }
            else
            {
                memcpy(emh_addr, addr, size);
                emh_free(addr);
            }
        }
    }
    return emh_addr;
}
