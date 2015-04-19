/*
    pywise Version 1.0
    
    A Python module to provide bindings to libpairwise for multi-threaded
    pairwise calculations across collections of points in any-dimensional
    space.
    
    Copyright (C) 2015 Adam J. Marsden
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    
*/

#ifndef PYWISE_H
#define PYWISE_H

#include "pywise_common.h"

/*******************************************************************************

    Symbol: pywise_methods
    
    Type: Array of PyMethodDef
    
    Intent: Private
    
    Description:
    
        A manifest of all public methods to be exposed by the pywise module.
        
*******************************************************************************/

PyMethodDef
pywise_methods[];

/*******************************************************************************

    Symbol: initpywise
    
    Type: Function returning PyMODINIT_FUNC
    
    Intent: Private
    
    Description:
    
        Registers the methods listed in pywise_methods, registers module
        constants, and then initialises the C-API for NumPy arrays.
        Automatically called by the Python interpreter when the pywise module
        is imported.
        
*******************************************************************************/

PyMODINIT_FUNC
initpywise
(void);

#endif /* PYWISE_H */
