/**
 * @file    umalloc.c
 * @author  Antonio Vitor Grossi Bassi (antoniovitor.gb@gmail.com)
 * @brief   umalloc source code.
 * @version 0.1
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

#include "umalloc.h"

#define UBYTE_ALIGN      4
#define UBYTE_ALIGN_MASK 0x0003

static const uHeapStructSize = ( sizeof(uHeap_link_t) + ( UBYTE_ALIGN - 1 ) ) & ~( (size_t) UBYTE_ALIGN_MASK );



uHeapId_t ucreate(void* addr, size_t size)
{
    uHeapId_t   id = 0;
    uint8_t*    aligned_heap = NULL;
    uint32_t    uaddr = 0;
    size_t      total_size = size;

    uaddr = ( uint32_t ) addr;

    // A misaligned address was provided, fix it for access consistency.
    if( ( UBYTE_ALIGN_MASK & uaddr ) != 0 )
    {
        uaddr += ( UBYTE_ALIGN - 1 );
        uaddr &= ~( UBYTE_ALIGN_MASK );
        total_size -= uaddr - ( (size_t) addr );
    }

    aligned_heap = (uint8_t*) uaddr;

    return id;
}






