#ifndef PAIRWISE_DISTANCES_H
#define PAIRWISE_DISTANCES_H

#include "pairwise.h"

/*******************************************************************************

    Symbol: pairwise_distances
    
    Type: Function returning int
    
    Intent: Public
    
    Description:
    
        Calculates all pairwise Euclidean distances across a set of points in
        any-dimensional space. Fairly distributes the total number of pairwise
        calculations to be done over the requested number of threads which are
        launched in parallel.
        
        n_points is the number of points in a_points, n_coordinates is the
        number of coordinates per point, and n_threads is the number of threads
        across which to distribute the pairwise calculations to be done.
        a_points is a pointer to an array containing the input set of points
        over which to do pairwise calculations. a_distances is a pointer to an
        array of sufficient size to be populated with the distances which are
        results of those pairwise calculations. The caller is responsible for
        ensuring that a_points has the expected form, and that a_distances is
        the expected size.
        
        a_points should have form,
        
            a_points = [POINT_1], [POINT_2], ..., [POINT_A]
        
        with,
        
            [POINT_A] = [COORDINATE_1], [COORDINATE_2], ..., [COORDINATE_B]
        
        where [COORDINATE_B] is a double, and the number of coordinates per
        point, B, is constant across all A points. n_points should equal the
        number of points, A, n_coordinates should equal the number of
        coordinates per point, B, and a_distances should be of sufficient size
        to store 0.5 * A * (A - 1) doubles.
        
        On success returns PAIRWISE_RETURN_SUCCESS which is always equivalent
        to integer zero. On failure returns a non-zero libpairwise error code.
               
*******************************************************************************/

int
pairwise_distances
(
    
    size_t n_points,
    size_t n_coordinates,
    
    double* a_points,
    double* a_distances,
    
    size_t n_threads

);

/*******************************************************************************

    Symbol: _pairwise_single_distance
    
    Type: Inline function returning double
    
    Intent: Private
    
    Description:
    
        Calculates the single Euclidean distance between two collections of
        points in the special case where each collection contains exactly one
        point only.
        
        n_points is the number of points per collection; conceptually this must
        always be one, but its value is actually ignored. This redundancy is
        necessary to ensure a function prototype which is compatible with
        _pairwise_launch(). n_coordinates is the number of coordinates per
        point. collection_a and collection_b are pointers to arrays containing
        the coordinates of the two single-point collections involved in this
        calculation.
        
        On success returns the calculated distance. Not expected to fail.
        
*******************************************************************************/

inline double
_pairwise_single_distance
(
    
    size_t n_points,
    size_t n_coordinates,
    
    double* collection_a,
    double* collection_b

);

#endif /* PAIRWISE_DISTANCES_H */
