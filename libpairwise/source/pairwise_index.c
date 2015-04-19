#include "pairwise_index.h"

/*******************************************************************************

    Symbol: pairwise_index
    
    Type: Function returning int
    
    Intent: Public
    
    Description:
    
        Calculates an index into an array of pairwise calculation results so
        populated by a successful call _pairwise_launch(), usually via a
        libpairwise public calculations function.
        
        n_collections is the total number of collections in the input set over
        which pairwise calculations were done. i_collection_a and
        i_collection_b are indices of the collections an index to the result of
        whose pairwise calculation we seek. i_result is a pointer to the size_t
        in which the sought index will be stored.
        
        Negative values for i_collection_a and i_collection_b are permitted;
        these index backwards from the end of the input set of collections
        where, for example, an index of -1 indicates the last collection in
        that set.
        
        Fails if either i_collection_a or i_collection_b indicates a collection
        which is out of bounds for an input set of n_collections collections,
        if i_collection_a and i_collection_b are indices for the same
        collection, or if n_collections is less than two.
        
        On success returns PAIRWISE_RETURN_SUCCESS which is always equivalent
        to integer zero. On failure returns a non-zero libpairwise error code.
        
*******************************************************************************/

int
pairwise_index
(
    
    size_t n_collections,
    
    ssize_t i_collection_a,
    ssize_t i_collection_b,
    
    size_t* i_result

)
{

    size_t i_collection_min;
    size_t i_collection_max;
    
    size_t i_working;
    
    if (n_collections < 2) {
    
        return PAIRWISE_RETURN_ERROR_NCOLLECTIONS;
    
    }
    
    if (i_collection_a < 0) {
    
        i_collection_a += n_collections;
    
    }
    
    if (i_collection_a < 0 || i_collection_a >= n_collections) {
    
        return PAIRWISE_RETURN_ERROR_ICOLLECTIONA;
    
    }
    
    if (i_collection_b < 0) {
    
        i_collection_b += n_collections;
    
    }
    
    if (i_collection_b < 0 || i_collection_b >= n_collections) {
    
        return PAIRWISE_RETURN_ERROR_ICOLLECTIONB;
    
    }
    
    if (i_collection_a == i_collection_b) {
    
        return PAIRWISE_RETURN_ERROR_ICOLLECTIONSAME;
    
    }
    
    if (i_collection_a < i_collection_b) {
    
        i_collection_min = i_collection_a;
        i_collection_max = i_collection_b;
    
    } else {
    
        i_collection_min = i_collection_b;
        i_collection_max = i_collection_a;
    
    }
    
    i_working = -i_collection_min * (i_collection_min - (2 * n_collections) + 1);
    
    i_working >>= 1;
    
    i_working += i_collection_max - i_collection_min - 1;
    
    *i_result = i_working;
    
    return PAIRWISE_RETURN_SUCCESS;

}
