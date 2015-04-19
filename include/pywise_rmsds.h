#ifndef PYWISE_RMSDS_H
#define PYWISE_RMSDS_H

#define NO_IMPORT_ARRAY

#include "pywise_common.h"

/*******************************************************************************

    Symbol: pywise_rmsds
    
    Type: Function returning PyObject*
    
    Intent: Public exposed in Python as pywise.rmsds()
    
    Python Signature:
    
        pywise.rmsds(collections, threads) -> numpy.ndarray
    
    Description:
    
        Calculates all pairwise RMSDs across a set of collections of points in
        any-dimensional space. Binds libpairwise to fairly distribute the total
        number of pairwise calculations to be done over the requested number of
        threads which are launched in parallel.
        
        On success pywise_rmsds() returns a one-dimensional NumPy array object
        which contains the results of all pairwise calculations. On failure it
        raises a Python exception.
        
*******************************************************************************/

PyObject*
pywise_rmsds
(

    PyObject* self,
    PyObject* values,
    PyObject* keys

);

#endif /* PYWISE_RMSDS_H */
