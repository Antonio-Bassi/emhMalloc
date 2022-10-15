/**
 * @file    ualign.h
 * @author  Antonio Vitor Grossi Bassi (antoniovitor.gb@gmail.com)
 * 
 * @brief   Contains memory alignment directive macros used on umalloc code. In order 
 *          to align the heap segment, a correction is made at ucreate() function. The 
 *          macros below define the type of alignment to be used. This parameters are 
 *          architecture specific and should be chosen with care.
 * 
 * @version alpha - 0.1
 * @date    2022-10-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "uportmacro.h"
 
#define UMALLOC_BYTE_ALIGNMENT 4

#if UMALLOC_BYTE_ALIGNMENT == 32
#define UMALLOC_BYTE_ALIGN_MASK 0x001F
#endif /* UMALLOC_BYTE_ALIGNMENT == 32 */

#if UMALLOC_BYTE_ALIGNMENT == 16
#define UMALLOC_BYTE_ALIGN_MASK 0x000F
#endif /* UMALLOC_BYTE_ALIGNMENT == 16 */

#if UMALLOC_BYTE_ALIGNMENT == 8
#define UMALLOC_BYTE_ALIGN_MASK 0x0007
#endif /* UMALLOC_BYTE_ALIGNMENT == 8 */

#if UMALLOC_BYTE_ALIGNMENT == 4
#define UMALLOC_BYTE_ALIGN_MASK 0x0003
#endif /* UMALLOC_BYTE_ALIGNMENT == 4 */

#if UMALLOC_BYTE_ALIGNMENT == 2
#define UMALLOC_BYTE_ALIGN_MASK 0x0001
#endif /* UMALLOC_BYTE_ALIGNMENT == 2 */

#if UMALLOC_BYTE_ALIGNMENT == 1
#define UMALLOC_BYTE_ALIGN_MASK 0x0000
#endif /* UMALLOC_BYTE_ALIGNMENT == 1 */

#ifndef UMALLOC_BYTE_ALIGNMENT
#error "Invalid alignment boundary or no alignment boundary defined!"
#endif