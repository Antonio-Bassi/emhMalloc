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
 
#define U_ALIGN_BOUNDARY 4

#if U_ALIGN_BOUNDARY == 32
#define U_BYTE_MASK 0x001F
#endif /* U_ALIGN_BOUNDARY == 32 */

#if U_ALIGN_BOUNDARY == 16
#define U_BYTE_MASK 0x000F
#endif /* U_ALIGN_BOUNDARY == 16 */

#if U_ALIGN_BOUNDARY == 8
#define U_BYTE_MASK 0x0007
#endif /* U_ALIGN_BOUNDARY == 8 */

#if U_ALIGN_BOUNDARY == 4
#define U_BYTE_MASK 0x0003
#endif /* U_ALIGN_BOUNDARY == 4 */

#if U_ALIGN_BOUNDARY == 2
#define U_BYTE_MASK 0x0001
#endif /* U_ALIGN_BOUNDARY == 2 */

#if U_ALIGN_BOUNDARY == 1
#define U_BYTE_MASK 0x0000
#endif /* U_ALIGN_BOUNDARY == 1 */

#ifndef U_ALIGN_BOUNDARY
#error "Invalid alignment boundary or no alignment boundary defined!"
#endif