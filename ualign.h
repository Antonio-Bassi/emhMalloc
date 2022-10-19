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
 * @file    ualign.h
 * @author  Antonio Vitor Grossi Bassi (antoniovitor.gb@gmail.com)
 * 
 * @brief   Contains memory alignment directive macros used on umalloc code. In order 
 *          to align the heap segment, a correction is made at ucreate() function. The 
 *          macros below define the type of alignment to be used. This parameters are 
 *          architecture specific and should be chosen with care. A proper alignment 
 *          can result in time improvements during memory accessing.
 * 
 * @version alpha
 * @date    2022-10-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
 
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