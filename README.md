# emh_malloc
A memory allocator designed for embedded systems. The prefix \"emh\" stands for Embedded Multi-Heap. The main proposition of emh_malloc is to manage multiple heap regions in a multi-threaded (or single threaded) environment without coalescing them. This is interesting to keep track of the memory organization throughout an embedded system project and to create isolated memory allocations for each important tasks. This way we guarantee that the given memory regions will not be tampered by other ongoing tasks.

## What's new?
Project was renamed to a more appropriate name.

## How does emh_malloc work?
Since our memory allocator performs operations on multiple heaps that are kept apart from each other, an identification means must be provided for each heap region and also for the allocated units within each region. For that we define 2 different objects:
* **emh_blockLink_t** : Data structure present in **every** memory block. Retains the size of the block (either free or allocated) and a pointer to the next block.  
* **emh_heapLink_t** : Data structure that is statically allocated during compile time. Retains the information from the heap region such as the struct for the starting block, a pointer to the last block of the heap and the amount of space available for allocation.

Before memory blocks can be allocated, the heap region must be specified and then initialised. This can be done by calling `emh_create` and passing the memory region pointer and the size (in bytes) of the respective region, this will initialise an available heap link structure and return the heap ID, which is an 8-bit signed integer (`signed char`), if `emh_create` fails a negative heap ID number is returned. The heap links are organized inside an array of **emh_heapLink_t** elements where each index of this array is the actual heap ID number.

<figure>
  <img align="center" height="285" width="601" src="https://github.com/Antonio-Bassi/emh_malloc/blob/main/mkdown_pics/how_emh_works.jpg">
</figure>

At first, our memory region is considered empty, i.e no allocated blocks whatsoever. Therefore the heap starts with a single **free** memory block pointed by **\*next** in the **start** block link struct, present in the heap link data structure. When `emh_malloc` is called, the allocation size and heap ID number must be passed as arguments. During the allocation process, `emh_malloc` performs memory alignment procedures on the requested allocation size and then traverse the entire free block list to find a block in the specified heap that will fit the requested size. If successful, the list of free blocks is updated and a pointer to the allocated block is returned, otherwise a `NULL` pointer is returned. Everytime `emh_malloc` successfully allocates a block, the respective block is unlinked from the **free** block list. Observe the illustrations below.

<figure>
  <img align="center" height="281" width="442" src="https://github.com/Antonio-Bassi/emh_malloc/blob/main/mkdown_pics/emh_malloc_step1.jpg">
</figure>

<figure>
  <img align="center" height="281" width="442" src="https://github.com/Antonio-Bassi/emh_malloc/blob/main/mkdown_pics/emh_malloc_step2.jpg">
</figure>

When we are done with our dynamic allocation needs, a call to `emh_free` may be placed. Only the block pointer needs to be provided as an argument since the heap ID number is packed within the block size field and extracted with a bit mask. After freeing the block, the newly available space is now linked to the free block list, if the **freed** memory block is contiguous in memory with another **free** block then it is agglutinated into a single free block. 

Other allocation API such as `emh_calloc` and `emh_realloc` are also available. Regarding `emh_realloc` it is important to stress that the reallocation algorithm does not perform contraction nor stretching of the allocated region. Instead it searches for a new memory block with the desired memory size and allocates it, copies the content of the previous block and then frees it.

## Integrating emh_malloc to your project
If `emh_malloc` operates in a multi-threaded environment, concurrent access to memory allocation is a problem we want to avoid. For that a mutual exclusion semaphore (mutex) must be provided, the function macros `__emh_create_zone__`, `__emh_lock_zone__` and `__emh_unlock_zone__` provide a hook to the mutex creation, locking and release functions. We must define the memory alignment options as well by defining the macro `EMH_MALLOC_BYTE_ALIGNMENT` with any of the options available on the file **emh_align.h**. Finally, the number of heaps managed by `emh_malloc` can be specified by defining `EMH_MALLOC_N_HEAPS`.

All the definitions listed above should be placed in a header file named **emh_portenv.h** that must be provided by the user. Below is an example of how to integrate `emh_malloc` to `FreeRTOS`.

```C
#include "FreeRTOS.h"
#include "semphr.h"

extern SemaphoreHandle_t emh_malloc_mtx;

#define EMH_MALLOC_N_HEAPS        4
#define EMH_MALLOC_BYTE_ALIGNMENT 4

#define __emh_create_zone__()                 \
{                                             \
  emh_malloc_mtx = xSemaphoreCreateMutex();   \
}                                             \

#define __emh_lock_zone__()                      \
{                                                \
  xSemaphoreTake(emh_malloc_mtx, portMAX_DELAY); \
}                                                \

#define __emh_unlock_zone__()     \
{                                 \
  xSemaphoreGive(emh_malloc_mtx); \
}                                 \
```

If we are operating in a single threaded environment, you may define as below.

```C
#define EMH_MALLOC_N_HEAPS        4
#define EMH_MALLOC_BYTE_ALIGNMENT 4

#define __emh_create_zone__()  \
{                              \
  do{}while(0);                \
}                              \

#define __emh_lock_zone__()    \
{                              \
  do{}while(0);                \
}                              \

#define __emh_unlock_zone__()  \
{                              \
  do{}while(0);                \
}                              \
```

## How to use emh_malloc
After the integration step is concluded, you can start using `emh_malloc` with the following API.

```C
extern emh_heapId_t emh_create(void *heapAddr, size_t heapSize);
extern void*        emh_malloc(emh_heapId_t heapId, size_t size);
extern void         emh_free(void *addr);
extern void*        emh_calloc(emh_heapId_t heapId, size_t n, size_t size);
extern void*        emh_realloc(void *addr, size_t size);
```

The `emh_create` function must be called to initialise your heap and adding it to the list of heap links. The return value `emh_heapId_t` is the identification number of the heap. You must store this value in a variable in order to perform allocations with `emh_malloc`, as this value identifies from which heap we wish to allocate. 