#ifndef PAIRWISE_RMSDS_H
#define PAIRWISE_RMSDS_H

#include "pairwise.h"

/*******************************************************************************

    Symbol: pairwise_rmsds
    
    Type: Function returning int
    
    Intent: Public
    
    Description:
    
        Calculates all pairwise RMSDs across a set of collections of points in
        any-dimensional space. Fairly distributes the total number of pairwise
        calculations to be done over the requested number of threads which are
        launched in parallel.
        
        n_collections is the number of collections in a_collections, n_points
        is the number of points per collection, and n_coordinates is the number
        of coordinates per point. n_threads is the number of threads across
        which to distribute the pairwise calculations to be done. a_collections
        is a pointer to an array containing the input set of collections over
        which to do pairwise calculations. a_rmsds is a pointer to an array of
        sufficient size to be populated with the RMSDs which are results of
        those pairwise calculations. The caller is responsible for ensuring
        that a_collections has the expected form, and that a_rmsds is the
        expected size.
        
        a_collections should have form,
        
            a_collections = [COLLECTION_1], [COLLECTION_2], ..., [COLLECTION_A]
        
        with,
        
            [COLLECTION_A] = [POINT_1], [POINT_2], ..., [POINT_B]
        
        and,
        
            [POINT_B] = [COORDINATE_1], [COORDINATE_2], ..., [COORDINATE_C]
        
        where [COORDINATE_C] is a double, and the numbers of points per
        collection and of coordinates per point, B and C respectively, are
        constant across all A collections. n_collections should equal the
        number of collections, A, n_points should equal the number of points
        per collections, B, and n_coordinates should equal the number of
        coordinates per point, C. a_distances should be of sufficient size to
        store 0.5 * A * (A - 1) doubles.
        
        On success returns PAIRWISE_RETURN_SUCCESS which is always equivalent
        to integer zero. On failure returns a non-zero libpairwise error code.
        
*******************************************************************************/

int
pairwise_rmsds
(
    
    size_t n_collections,
    size_t n_points,
    size_t n_coordinates,
    
    double* a_collections,
    double* a_rmsds,
    
    size_t n_threads

);

/*******************************************************************************

    Symbol: _pairwise_single_rmsd
    
    Type: Inline function returning double
    
    Intent: Private
    
    Description:
    
        Calculates the single RMSD between two collections of points.
        
        n_points is the number of points per collection, and n_coordinates is
        the number of coordinates per point. collection_a and collection_b are
        pointers to arrays containing the coordinates of the two collections
        involved in this calculation.
        
        On success returns the calculated RMSD. Not expected to fail.
        
*******************************************************************************/

inline double
_pairwise_single_rmsd
(
    
    size_t n_points,
    size_t n_coordinates,
    
    double* collection_a,
    double* collection_b

);

#endif /* PAIRWISE_RMSDS_H */
