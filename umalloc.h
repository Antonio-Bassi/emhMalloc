/**
 * @file    umalloc.h
 * @author  Antonio Vitor Grossi Bassi (antoniovitor.gb@gmail.com)
 * @brief   umalloc core function set.
 * @version 0.1
 * @date    2022-10-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

typedef unsigned char uHeapId_t;

typedef struct uBlock_link_t
{
    struct uBlock_link_t* next;
    size_t block_size;
}uBlock_link_t;

typedef struct uHeap_link_t
{
    uHeapId_t heap_id;
    struct uHeap_link_t* next;
    size_t heap_size;
}uHeap_link_t;

uHeapId_t   ucreate(void* addr, size_t size);
void        udestroy(uHeaoId_t heapId);
void*       umalloc(uHeapId_t heapId, size_t size);
void*       ucalloc(uHeapId_t heapId, size_t size);
void*       urealloc(uHeapId_t heapId, size_t size);
void        ufree(uHeapId_t heapId, void* addr);