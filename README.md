# emh_malloc
A multi-heap memory allocator for embedded systems based on FreeRTOS Kernel [**heap 4**](https://github.com/sifive/FreeRTOS-metal/blob/master/FreeRTOS-Kernel/portable/MemMang/heap_4.c) memory management algorithm. The prefix \"emh\" stands for Embedded Multi-Heap. The main proposition of emh_malloc is to manage multiple heap regions in a multi-threaded (or single threaded) environment without coalescing them. This is interesting to keep track of the memory organization throughout an embedded system project and to create isolated memory allocations for each important tasks. This way we guarantee that the given memory regions will not be tampered by other ongoing tasks.

## What's new?
Project was renamed to a more appropriate name.

## How does emh_malloc work?
coming soon...

## Integrating emh_malloc to your project
coming soon...

## How to use emh_malloc
coming soon...
