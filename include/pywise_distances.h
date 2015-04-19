#ifndef PYWISE_DISTANCES_H
#define PYWISE_DISTANCES_H

#define NO_IMPORT_ARRAY

#include "pywise_common.h"

/*******************************************************************************

    Symbol: pywise_distances
    
    Type: Function returning PyObject*
    
    Intent: Public exposed in Python as pywise.distances()
    
    Python Signature:
    
        pywise.distances(points, threads) -> numpy.ndarray
    
    Description:
    
        Calculates all pairwise Euclidean distances across a set of points in
        any-dimensional space. Binds libpairwise to fairly distribute the total
        number of pairwise calculations to be done over the requested number of
        threads which are launched in parallel.
        
        On success pywise_distances() returns a one-dimensional NumPy array
        object which contains the results of all pairwise calculations. On
        failure it raises a Python exception.
        
*******************************************************************************/

PyObject*
pywise_distances
(

    PyObject* self,
    PyObject* values,
    PyObject* keys

);

#endif /* PYWISE_DISTANCES_H */
