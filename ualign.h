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
 
#define uBYTE_ALIGNMENT 4

#if uBYTE_ALIGNMENT == 32
#define uBYTE_ALIGN_MASK 0x001F
#endif /* uBYTE_ALIGNMENT == 32 */

#if uBYTE_ALIGNMENT == 16
#define uBYTE_ALIGN_MASK 0x000F
#endif /* uBYTE_ALIGNMENT == 16 */

#if uBYTE_ALIGNMENT == 8
#define uBYTE_ALIGN_MASK 0x0007
#endif /* uBYTE_ALIGNMENT == 8 */

#if uBYTE_ALIGNMENT == 4
#define uBYTE_ALIGN_MASK 0x0003
#endif /* uBYTE_ALIGNMENT == 4 */

#if uBYTE_ALIGNMENT == 2
#define uBYTE_ALIGN_MASK 0x0001
#endif /* uBYTE_ALIGNMENT == 2 */

#if uBYTE_ALIGNMENT == 1
#define uBYTE_ALIGN_MASK 0x0000
#endif /* uBYTE_ALIGNMENT == 1 */

#ifndef uBYTE_ALIGNMENT
#error "Invalid alignment boundary or no alignment boundary defined!"
#endif