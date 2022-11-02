/**
 *  u_malloc
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
 * @file    umalloc.h
 * @author  Antonio Vitor Grossi Bassi (antoniovitor.gb@gmail.com)
 * @brief   umalloc core function set.
 * @version alpha
 * @date    2022-10-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#define UMALLOC_HEAP_ID_BITMASK 0x7F
#define UMALLOC_BITS_PER_BYTE   8
#define UMALLOC_MAX_HEAPS       127
#define UMALLOC_N_HEAPS         4

#if !defined(UMALLOC_N_HEAPS)
#define uNUM_OF_HEAPS 2
#elif   UMALLOC_N_HEAPS > UMALLOC_MAX_HEAPS
#undef  UMALLOC_N_HEAPS
#define UMALLOC_N_HEAPS 2
#warning "umalloc error, number of heaps macro exceeds the maximum, macro redefined to 2"
#endif /* uNUM_OF_HEAPS */

typedef signed char u_heap_id_t;

typedef struct u_block_link_t
{
    size_t u_block_size;
    struct u_block_link_t* u_next_free;
}u_block_link_t;

typedef struct u_heap_link_t
{
    u_block_link_t  u_start;
    u_block_link_t* u_end;
    size_t          u_free_bytes;
    size_t          u_remain_bytes;   
}u_heap_link_t;

extern u_heap_id_t u_create(void *heap_addr, size_t heap_size);
extern void*       u_malloc(u_heap_id_t heap_id, size_t size);
extern void*       u_calloc(u_heap_id_t heap_id, size_t num, size_t size);
extern void*       u_realloc(void *addr, size_t size);
extern void        u_free(void *addr);