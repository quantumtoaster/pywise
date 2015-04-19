#ifndef PYWISE_INDEX_H
#define PYWISE_INDEX_H

#define NO_IMPORT_ARRAY

#include "pywise_common.h"

/*******************************************************************************

    Symbol: pywise_index
    
    Type: Function returning PyObject*
    
    Intent: Public exposed in Python as pywise.index()
    
    Python Signature:
    
        pywise.index(n_collections, i_collection_a, i_collection_b) 
            -> numpy.ndarray
    
    Description:
    
        Calculates an index into the NumPy array object returned by one of
        pywise's calculation functions. This index corresponds to the result of
        the calculation between the collection in the set of input collections
        with index i_collection_a, and that with index i_collection_b, given
        the total number of coollections in the set, n_collections.
        
        On success returns a Python integer object. On failure raises a Python
        exception.
        
*******************************************************************************/

PyObject*
pywise_index
(

    PyObject* self,
    PyObject* values,
    PyObject* keys

);

#endif /* PYWISE_INDEX_H */
