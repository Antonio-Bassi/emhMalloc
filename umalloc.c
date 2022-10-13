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

static const size_t uHeapLinkSize = (sizeof(uHeapLink_t) + ((size_t)(U_ALIGN_BOUNDARY - 1))) & ~((size_t)U_BYTE_MASK);

uHeapLink_t uStaticLink =
{
    .id = (uHeapId_t)0,
    .size = 0,
    .next_block = NULL,
    .next_heap = &uStaticLink
};

uHeapId_t ucreate(void *addr, size_t size)
{
    uHeapLink_t uNewHeap = {0};

    uint8_t *uAligned;
    size_t uAddr;
    size_t uTotalSize = size;

    uAddr = (size_t)(addr);
    if ( ((uAddr)&U_BYTE_MASK) != 0)
    {
        uAddr += (U_ALIGN_BOUNDARY - 1);
        uAddr &= ~((size_t)(U_BYTE_MASK));
        uTotalSize -= uAddr - ((size_t)addr);
    }
    uAligned = (uint8_t*) uAddr;

    return 0;
}
