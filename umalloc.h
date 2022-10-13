/**
 * @file    umalloc.h
 * @author  Antonio Vitor Grossi Bassi (antoniovitor.gb@gmail.com)
 * @brief   umalloc core function set.
 * @version alpha - 0.1
 * @date    2022-10-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

typedef unsigned char uHeapId_t;

typedef struct uBlockLink_t
{
    size_t size;
    struct uBlockLink_t* next;
}uBlockLink_t;

typedef struct uHeapLink_t
{
    uHeapId_t               id;
    size_t                  size;
    struct uBlockLink_t*    next_block;
    struct uHeapLink_t*     next_heap;
}uHeapLink_t;

uHeapId_t   ucreate(void* addr, size_t size);
void        udestroy(uHeapId_t heapId);
void*       umalloc(uHeapId_t heapId, size_t size);
void*       ucalloc(uHeapId_t heapId, size_t size);
void*       urealloc(uHeapId_t heapId, size_t size);
void        ufree(uHeapId_t heapId, void* addr);