#ifndef PAIRWISE_LAUNCH_H
#define PAIRWISE_LAUNCH_H

#include "pairwise.h"

/*******************************************************************************

    Symbol: _pairwise_as_t
    
    Type: Structure
    
    Intent: Private
    
    Description:
    
        Parameterises a call to _pairwise_launch_bounded(). Initialised by
        _pairwise_populate_argument_sets().
        
*******************************************************************************/

typedef struct
_pairwise_argument_set
{
    
    double (*f_calculation)(size_t n_points,
                            size_t n_coordinates,
                            double* collection_a,
                            double* collection_b);
    
    double* a_collections;
    double* a_results;
    
    size_t n_collections;
    size_t n_points;
    size_t n_coordinates;
    
    size_t i_collection_lower;
    size_t i_collection_upper;
    
} _pairwise_as_t;

/*******************************************************************************

    Symbol: _pairwise_populate_argument_sets
    
    Type: Function returning void
    
    Intent: Private
    
    Description:
    
        Initialises an array of _pairwise_as_t, a_argument_sets, according to
        parameters specified by the caller. Each initialised _pairwise_as_t
        can then be passed to _pairwise_launch_bounded() to carry out a
        specific subset of all pairwise calculations to be done. Multiple such
        calls to _pairwise_launch_bounded(), each parameterised by a different
        _pairwise_as_t in an initialised a_argument_sets, can be made by
        multiple threads running in parallel; this is the libpairwise
        parallelisation strategy.
        
        f_calculation is a pointer to a function representing a single round of
        the pairwise calculations to be done. n_collections is the number of
        collections in a_collections, n_points is the number of points per
        collection, and n_coordinates is the number of coordinates per point.
        a_collections is a pointer to an input array containing a set of
        collections, a_results is a pointer to an output array of sufficient
        size to store the results of all pairwise calculations. n_argument_sets
        is the number of _pairwise_as_t in a_argument_sets, and a_argument_sets
        is a pointer to the array of _pairwise_as_t to be initialised.
        
        Changes only a_argument_sets. The caller is responsible for ensuring
        that a_collections has the expected form (see the prologue comment for
        pairwise_rmsds()), and that a_results is large enough to store
        0.5 * n_collections * (n_collections - 1) doubles.
        
        On success returns nothing. Not expected to fail.
        
*******************************************************************************/

void
_pairwise_populate_argument_sets
(

    double (*f_calculation)(size_t n_points,
                            size_t n_coordinates,
                            double* collection_a,
                            double* collection_b),
    
    size_t n_collections,
    size_t n_points,
    size_t n_coordinates,
    
    double* a_collections,
    double* a_results,
    
    size_t n_argument_sets,
    _pairwise_as_t* a_argument_sets
    
);

/*******************************************************************************

    Symbol: _pairwise_launch_bounded
    
    Type: Function returning void
    
    Intent: Private
    
    Description:
    
        Carries out a subset of all the pairwise calculations to be done which
        is parameterised by an initialised _pairwise_as_t, argument_set.
        
        On success returns nothing. Not expected to fail.
        
*******************************************************************************/

void
_pairwise_launch_bounded
(
    
    _pairwise_as_t* argument_set

);

/*******************************************************************************

    Symbol: _pairwise_launch
    
    Type: Function returning int
    
    Intent: Private
    
    Description:
    
        Coordinates the parallel execution of pairwise calculations to be done
        distributed fairly across multiple threads.
        
        f_calculation is a pointer to a function representing a single round of
        the pairwise calculations to be done. n_collections is the number of
        collections in a_collections, n_points is the number of points per
        collection, and n_coordinates is the number of coordinates per point.
        a_collections is a pointer to an input array containing a set of
        collections, a_results is a pointer to an output array of sufficient
        size to store the results of all pairwise calculations. n_argument_sets
        is the number of _pairwise_as_t in a_argument_sets, and a_argument_sets
        is a pointer to the array of _pairwise_as_t to be initialised.
        
        The caller is responsible for ensuring that a_collections has the
        expected form (see the prologue comment for pairwise_rmsds()), and that
        a_results is large enough to store
        0.5 * n_collections * (n_collections - 1) doubles.
        
        On success returns PAIRWISE_RETURN_SUCCESS which is always equivalent
        to integer zero. On failure returns a non-zero libpairwise error code,
        and makes no guarantee about the state a_results, which may or may not
        have been changed.
        
*******************************************************************************/

int
_pairwise_launch
(

    double (*f_calculation)(size_t n_points,
                            size_t n_coordinates,
                            double* collection_a,
                            double* collection_b),
    
    size_t n_collections,
    size_t n_points,
    size_t n_coordinates,
    
    double* a_collections,
    double* a_results,
    
    size_t n_threads

);

#endif /* PAIRWISE_LAUNCH_H */
