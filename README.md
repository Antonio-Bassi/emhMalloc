# umalloc
A multi-heap memory allocator based on FreeRTOS Kernel [**heap 4**](https://github.com/sifive/FreeRTOS-metal/blob/master/FreeRTOS-Kernel/portable/MemMang/heap_4.c) memory management algorithm. The prefix \"u\" is used to mimic the __mu__ letter from the greek alphabet, which in power of 10 notation is denoted as \"micro\" and thus making reference to  **micro**controllers. The main idea of umalloc or (**micro**malloc if you will) is to manage multiple heap regions without coallescing them in a multi-threaded (or single threaded) environment. This is interesting to keep track of the memory organization throughout the embedded system project and to isolate critical memory allocations from important tasks. This way we guarantee that the given memory regions will not be tampered by other ongoing tasks.

## What's new?
Currently umalloc only performs memory allocation, deallocation and coallescing operations on a given heap. In upcoming updates, functions such as calloc and realloc shall be implemented as well. 

## How to use umalloc

