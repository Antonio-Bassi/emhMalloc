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


static const size_t uBlockLinkSize = ( sizeof(uBlockLink_t) + ( ( size_t ) ( uBYTE_ALIGNMENT - 1 ) ) ) & ~( ( size_t ) uBYTE_ALIGN_MASK );

static uHeapLink_t uStaticLinks[uNUM_OF_HEAPS] = {0};

static int uHeapIdExists(uHeapId_t heapId)
{
    uHeapId_t uHeapIdx = heapId;
    uHeapIdx &= ~(uHEAPID_BITMASK);
    return ( heapId == uStaticLinks[uHeapIdx].id );   
}

uHeapId_t ucreate(void* addr, size_t size)
{
    uHeapId_t       uHeapIdx        =   0;
    uBlockLink_t*   uNewBlock       =   {0};
    size_t          uAddr           =   (size_t)( addr );
    size_t          uTotalHeapSize  =   size;
    uint8_t*        uAlignedAddr    =   NULL; 
    
    // Search for available slots
    while ( uNUM_OF_HEAPS > uHeapIdx )
    {
        if( !( ( uStaticLinks[uHeapIdx].id ) & ( (uHeapId_t)uHEAPID_BITMASK ) ) )
        {
            uStaticLinks[uHeapIdx].id = ( uHeapIdx | ( (uHeapId_t) uHEAPID_BITMASK ) );
            break;
        }
        uHeapIdx++;
    }

    if( uHeapIdx >= uNUM_OF_HEAPS )
    {
        /*
         * If we are here, it means we ran out of heap slots.
         * Return an invalid heap id.
         * 
         * obs: Heap IDs must be greater than 127 due to the allocation bit marker.
         */
        uHeapIdx = 0;
        return uHeapIdx;
    }

    // Perform heap address alignment 
    if( (uAddr & uBYTE_ALIGN_MASK) != 0 )
    {
        uAddr += uBYTE_ALIGNMENT - 1;
        uAddr &= ~( (size_t) uBYTE_ALIGN_MASK );
        uTotalHeapSize -= uAddr - ((size_t)addr);
    }
    uAlignedAddr = (uint8_t*) uAddr;

    uStaticLinks[uHeapIdx].uStart       = (void*)(uAlignedAddr);
    uStaticLinks[uHeapIdx].uStart->size = (size_t) 0;

    uAddr = ((size_t) uAlignedAddr ) + uTotalHeapSize;
    uAddr -= uBlockLinkSize;
    uAddr &= ~( (size_t) uBYTE_ALIGN_MASK );

    uStaticLinks[uHeapIdx].uEnd         = (void*)(uAddr);
    uStaticLinks[uHeapIdx].uEnd->size   = (size_t) 0;
    uStaticLinks[uHeapIdx].uEnd->next   = NULL;

    uNewBlock       = (void*) uAlignedAddr;
    uNewBlock->size = uAddr - ((size_t)uNewBlock);
    uNewBlock->next = uStaticLinks[uHeapIdx].uEnd;
    
    uHeapIdx = uStaticLinks[uHeapIdx].id;

    return uHeapIdx;
}

void* umalloc(uHeapId_t heapId, size_t size)
{
    void* uPtr = NULL;
    if( 0 == uHeapIdExists(heapId) )
    {
        return uPtr;
    }

    uHeapId_t       uHeapIdx = uHeapIdx & ~(uHEAPID_BITMASK);
    uBlockLink_t*   uHeap = uStaticLinks[uHeapIdx].uStart;

    if( size > uHeap->size )
    {
        /*
         * Requesting a size greater than the available.
         * umalloc tragedy, no memory left.
         */
        return uPtr;
    }

    



}

