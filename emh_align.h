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
 * @file    emh_align.h
 * @author  Antonio Vitor Grossi Bassi (antoniovitor.gb@gmail.com)
 * 
 * @brief   Contains memory alignment directive macros used on emh_malloc code. In order 
 *          to align the heap segment, a correction is made at emh_create() function. The 
 *          macros below define the type of alignment to be used. This parameters are 
 *          architecture specific and should be chosen with care. A proper alignment 
 *          can result in time improvements during memory accessing.
 * 
 * @version 1.6
 * @date    2022-10-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef EMH_ALIGN_H
#define EMH_ALIGN_H

/* This header file must be provided by the user! */
#include <emh_portenv.h>

#if !defined(EMH_MALLOC_BYTE_ALIGNMENT)
#error "emh_malloc: ERROR -> No memory alignment was specified! Check emh_malloc/emh_align.h"
#endif /* EMH_MALLOC_BYTE_ALIGNMENT */

#if ((EMH_MALLOC_BYTE_ALIGNMENT > 32) | (EMH_MALLOC_BYTE_ALIGNMENT < 1))
#error "emh_malloc: ERROR -> An invalid memory alignment directive was specified! Check emh_malloc/emh_align.h"
#endif

/* 256-bits alignment */
#if ( EMH_MALLOC_BYTE_ALIGNMENT == 32 )
#define EMH_MALLOC_BYTE_ALIGN_MASK 0x001F
#endif /* EMH_MALLOC_BYTE_ALIGNMENT == 32 */

/* 128-bits alignment */
#if ( EMH_MALLOC_BYTE_ALIGNMENT == 16 )
#define EMH_MALLOC_BYTE_ALIGN_MASK 0x000F
#endif /* EMH_MALLOC_BYTE_ALIGNMENT == 16 */

/* 64-bits alignment */
#if ( EMH_MALLOC_BYTE_ALIGNMENT == 8 )
#define EMH_MALLOC_BYTE_ALIGN_MASK 0x0007
#endif /* EMH_MALLOC_BYTE_ALIGNMENT == 8 */

/* 32-bits alignment */
#if ( EMH_MALLOC_BYTE_ALIGNMENT == 4 )
#define EMH_MALLOC_BYTE_ALIGN_MASK 0x0003
#endif /* EMH_MALLOC_BYTE_ALIGNMENT == 4 */

/* 16-bits alignment */
#if ( EMH_MALLOC_BYTE_ALIGNMENT == 2 )
#define EMH_MALLOC_BYTE_ALIGN_MASK 0x0001
#endif /* EMH_MALLOC_BYTE_ALIGNMENT == 2 */

/* 8-bits alignment */
#if ( EMH_MALLOC_BYTE_ALIGNMENT == 1 )
#define EMH_MALLOC_BYTE_ALIGN_MASK 0x0000
#endif /* EMH_MALLOC_BYTE_ALIGNMENT == 1 */

#endif /* EMH_ALIGN_H */
