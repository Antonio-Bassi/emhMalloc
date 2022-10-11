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

#define UMEM_ALIGN      4
#define UMEM_ALIGN_MASK 0x0003

uHeapId_t ucreate(void* addr, size_t size)
{
    uHeapId_t   id = 0;
    uint8_t*    aligned_addr = NULL;
    uint32_t    base_addr = 0;

    if( ( UMEM_ALIGN_MASK & base_addr ) != 0 )
    {

    }

    return id;
}






