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


#define uHEAPID_BITMASK 0x80
#define uMAX_NUM_OF_HEAPS 0x7F
#define uNUM_OF_HEAPS 10

typedef unsigned char uHeapId_t;

typedef struct uBlockLink_t
{
    size_t size;
    struct uBlockLink_t* next;
}uBlockLink_t;

typedef struct uHeapLink_t
{
    uHeapId_t       id;
    uBlockLink_t*   uStart;
    uBlockLink_t*   uEnd;   
}uHeapLink_t;

uHeapId_t   ucreate(void* addr, size_t size);
uHeapId_t   udestroy(uHeapId_t heapId);
void*       umalloc(uHeapId_t heapId, size_t size);
void*       ucalloc(uHeapId_t heapId, size_t size);
void*       urealloc(uHeapId_t heapId, size_t size);
void        ufree(uHeapId_t heapId, void* addr);