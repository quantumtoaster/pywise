#ifndef PYWISE_BUILD_POINTS_ARRAY_H
#define PYWISE_BUILD_POINTS_ARRAY_H

#define NO_IMPORT_ARRAY

#include "pywise_common.h"

/*******************************************************************************

    Symbol: pywise_build_points_array
    
    Type: Function returning double*
    
    Intent: Private
    
    Description:
    
        Builds from a suitable Python object an array of collections of form
        appropriate for passing to a libpairwise calculation function.
        
        o_source is a pointer to the input Python object. On success stores the
        number of points in o_source and the number of coordinates per point in 
        n_points and n_coordinates respectively. Also returns a pointer to a
        new array of these points, the responsibility to free which is passed
        on to the caller. On failure sets a Python exception and returns a null
        pointer.
        
*******************************************************************************/

double*
pywise_build_points_array
(
    
    PyObject* o_source,
    
    size_t* n_points,
    size_t* n_coordinates

);

#endif /* PYWISE_BUILD_POINTS_ARRAY_H */
