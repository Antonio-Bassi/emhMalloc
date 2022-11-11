# umalloc
A multi-heap memory allocator based on FreeRTOS Kernel [**heap 4**](https://github.com/sifive/FreeRTOS-metal/blob/master/FreeRTOS-Kernel/portable/MemMang/heap_4.c) memory management algorithm. The prefix \"u\" is used to mimic the __mu__ letter from the greek alphabet, which in power of 10 notation is denoted as \"micro\" and thus making reference to  **micro**controllers. The main idea of umalloc or (**micro**malloc if you will) is to manage multiple heap regions in a multi-threaded (or single threaded) environment without coallescing them. This is interesting to keep track of the memory organization throughout the embedded system project and to create isolated memory allocations for important tasks. This way we guarantee that the given memory regions will not be tampered by other ongoing tasks.

## What's new?
u_realloc and u_calloc are now implemented and ready to use. A documentation is due to be written.

## How to use umalloc
coming soon...
