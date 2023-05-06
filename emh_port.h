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
 * @file    emh_port.h
 * @author  Antonio Vitor Grossi Bassi (antoniovitor.gb@gmail.com)
 * @brief   Port environment header file.
 *          This file is dedicated for the user to port u_malloc to
 *          their application. You may add necessary header files and
 *          definitions and must define the critical section function
 *          macros.
 * 
 * @version 1.6
 * @date    2022-10-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef EMH_PORT_H
#define EMH_PORT_H


/*
 * This macros are user defined and strictly necessary 
 * for multi-threaded environment in order to create 
 * the critical section (or critical zone if you will) 
 * thus protecting u_malloc from concurrent accesses.
 */
#ifndef __emh_create_zone__
#define __emh_create_zone__(){    \
	(void*) NULL;	            \
}
#endif /* __emh_create_zone__ */

#ifndef __emh_lock_zone__
#define __emh_lock_zone__(){   \
    (void*) NULL;            \
}                               
#endif /* __emh_lock_zone__ */

#ifndef __emh_unlock_zone__
#define __emh_unlock_zone__(){ \
    (void*) NULL;            \
}
#endif /* __emh_unlock_zone__ */ 

#endif /* EMH_PORT_H */
