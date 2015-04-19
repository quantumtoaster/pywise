#ifndef PYWISE_EXCEPTION_H
#define PYWISE_EXCEPTION_H

#define NO_IMPORT_ARRAY

#include "pywise_common.h"

/*******************************************************************************

    Symbol: pywise_set_python_exception_from_pairwise_calculations_return_code
    
    Type: Function returning void
    
    Intent: Private
    
    Description:
    
        Sets an appropriate Python exception given a failure return code from a
        public libpairwise calculations function.
        
        Ignores unrecognised error codes, returns nothing, and will not fail.
        
*******************************************************************************/

void
pywise_set_python_exception_from_pairwise_calculations_return_code
(
    
    int n_return

);

/*******************************************************************************

    Symbol: pywise_set_python_exception_from_pairwise_index_return_code
    
    Type: Function returning void
    
    Intent: Private
    
    Description:
    
        Sets an appropriate Python exception given a failure return code from
        libpairwise's public pairwise_index().
        
        Ignores unrecognised error codes, returns nothing, and will not fail.
        
*******************************************************************************/

void
pywise_set_python_exception_from_pairwise_index_return_code
(
    
    int n_return

);

#endif /* PYWISE_EXCEPTION_H */
