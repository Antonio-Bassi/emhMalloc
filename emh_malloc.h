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
 * @file    emh_malloc.h
 * @author  Antonio Vitor Grossi Bassi (antoniovitor.gb@gmail.com)
 * @brief   emh_malloc core function set.
 * @version 1.6
 * @date    2022-10-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef EMH_MALLOC_H
#define EMH_MALLOC_H

#define EMH_MALLOC_HEAP_ID_BITMASK 0x7F
#define EMH_MALLOC_BITS_PER_BYTE   8
#define EMH_MALLOC_MAX_HEAPS       127
#define EMH_MALLOC_N_HEAPS         4

#if !defined(EMH_MALLOC_N_HEAPS)
#define EMH_MALLOC_N_HEAPS 2
#elif   EMH_MALLOC_N_HEAPS > EMH_MALLOC_MAX_HEAPS
#undef  EMH_MALLOC_N_HEAPS
#define EMH_MALLOC_N_HEAPS 2
#warning "emh_malloc: EMH_MALLOC_N_HEAPS exceeds the limit, macro redefined to 2"
#endif /* !defined(EMH_MALLOC_N_HEAPS) */

typedef signed char emh_heapId_t;

typedef struct emh_blockLink_t
{
    size_t blockSize;
    struct emh_blockLink_t* nextFree;
}emh_blockLink_t;

typedef struct emh_heapLink_t
{
    emh_blockLink_t  start;
    emh_blockLink_t* end;
    size_t           freeBytes;
    size_t           remainBytes;   
}emh_heapLink_t;

extern emh_heapId_t emh_create(void *heapAddr, size_t heapSize);
extern void*        emh_malloc(emh_heapId_t heapId, size_t size);
extern void         emh_free(void *addr);
extern void*        emh_calloc(emh_heapId_t heapId, size_t n, size_t size);
extern void*        emh_realloc(void *addr, size_t size);

#endif /* EMH_MALLOC_H */
