#include "pairwise_launch.h"

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
        
    Further Information:
    
        This function determines the fairest division of the pairwise
        calculations to be done across the requested number of threads to be
        run in parallel, n_threads. For each such subset of these calculations,
        this function determines i_collection_lower and i_collection_upper,
        the lower- and upper-bound indices, respectively, into a_collections,
        an input array containing a set of collections over which to do these
        calculations. It also determines i_results_offset, the corresponding
        lower-bound index into a_results, the output array in which the results
        of these calculations will be stored.
    
        In this way, each instance of _pairwise_launch_bounded() parameterised
        by a different _pairwise_as_t in an initialised a_argument_sets
        works on a different subset of pairs of collections in a_collections,
        and then populates with the results of the corresponding subset of the
        pairwise calculations to be done the appropriate section of a_results.
        Sections of a_results are chosen such that they are contiguous and non-
        overlapping. Thus, in making all calls to _pairwise_launch_bounded() as
        determined by a_argument_sets after initialisation by this function, it
        is guaranteed that a_results will be fully and correctly populated with
        the results of all the pairwise calculations to be done.
        
*******************************************************************************/

void
_pairwise_populate_argument_sets
(

    /*
    *   A pointer to the function representing the calculation to be carried
    *   out across all pairs of collections in a_collections. n_points is the
    *   number of points per collection, n_coordinates is the number of
    *   coordinates per point, and collection_a and collection_b are pointers
    *   to the pair of collections involved in this calculation.
    */
    
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
    
)
{
    
    size_t i_argument_set;
    
    size_t i_collection_lower;
    size_t i_collection_upper;
    size_t i_collection_delta;
    
    size_t i_results_offset;
    
    double n_calculations_per_argument_set;
    double n_collections_remaining;
    double n_collections_used;
    
    /*
    *   Calculate the ideal number of calculations to be carried out by each
    *   call to _pairwise_launch_bounded().
    */
    
    n_calculations_per_argument_set = (0.5 * n_collections * (n_collections - 1)) / n_argument_sets;
    
    i_collection_upper = 0;
    
    n_collections_remaining = n_collections;
    
    /*
    *   Populate each _pairwise_as_t in a_argument_sets appropriately.
    */
    
    for (i_argument_set = 0; i_argument_set < n_argument_sets; i_argument_set ++) {
    
        if (!i_argument_set) {
        
            /*
            *   For the first _pairwise_as_t read from the beginning of the
            *   input data array and correspondingly write to the beginning of
            *   the output data array.
            */
        
            i_results_offset = 0;
            
            i_collection_lower = 0;
        
        } else {
        
            /*
            *   For subsequent _pairwise_as_t read from where in the input data
            *   array the last _pairwise_as_t left off. Write to the output
            *   data array at the corresponding offset, given the expression
            *   appropriate to a set of pairwise calculations carried out in
            *   the order,
            *   
            *       1_to_2, 1_to_3, ..., 1_to_N,
            *               2_to_3, ..., 2_to_N,
            *                       ..., ...,
            *                       ..., (N - 1)_to_N
            *   
            *   where i_to_j represents the calculation done on the pair of
            *   collections i and j.
            *   
            *   Throughout libpairwise, sets of pairwise calculations are
            *   indexed according to the index of the first collection - that
            *   is, collection i in i_to_j notation, considered by any one
            *   given pairwise calculation. Thus, for example, lower- and
            *   upper-bounds of i = 5 and i = 7 correspond to the set of
            *   pairwise calculations,
            *   
            *       5_to_6, 5_to_7, 5_to_8, ..., 5_to_N,
            *               6_to_7, 6_to_8, ..., 6_to_N,
            *                       7_to_8, ..., 7_to_N
            *  
            *   such that any given pairwise calculation i_to_j, assumed to
            *   yield an equivalent result to that j_to_i, is carried out
            *   exactly once.
            *   
            *   In this way, for fair distribution of pairwise calculations
            *   across calls to _pairwise_launch_bounded(), the difference
            *   between the upper- and lower-indices into the input data array,
            *   i_collection_delta, grows quadratically with i_argument_set,
            *   the index into the array of _pairwise_as_t.
            */
        
            i_collection_delta = i_collection_upper - i_collection_lower;
        
            i_results_offset += 0.5 * i_collection_delta * ((2 * (n_collections - i_collection_lower)) - i_collection_delta - 1);
            
            i_collection_lower = i_collection_upper;
        
        }
        
        /*
        *   Calculate the number of first collections - that is, collections i
        *   in i_to_j notation as follows the pairwise calculation series
        *   defined above - that have already been assigned to _pairwise_as_t,
        *   n_collections_used, and thus those that remain to be assigned,
        *   n_collections_remaining.
        *   
        *   Note that the result of n_collections_used is not likely to be an
        *   integer; given that this result is found in terms of first
        *   collection index - that is, in terms of i in i_to_j notation -, it
        *   is not guaranteed that there exists an integer solution for an
        *   arbitrary number of divisions of pairwise calculations - that is,
        *   for arbitrary n_argument_sets. Yet we require integer solutions so
        *   that they may be used to form indices into the input data array.
        *   
        *   The strategy here is to round down n_collections_used to an integer
        *   for all _pairwise_as_t except the last. (Rounding down is implicit
        *   in the assignment of a double to a size_t.) For the last
        *   _pairwise_as_t we instead round up to n_collections, the number of
        *   input collections across which to carry out pairwise calculations.
        *   This ensures that all pairwise calculations are assigned to one of
        *   the available _pairwise_as_t.
        *   
        *   The practical consequence of this strategy is that the call to
        *   _pairwise_launch_bounded() parameterised by the last _pairwise_as_t
        *   in a_argument_sets will usually have to carry out a slightly larger
        *   number of pairwise calculations than the other calls. However, this
        *   excess will be very small compared with the number of pairwise
        *   calculations to the carried out by the other calls for any total
        *   number of pairwise calculations for which this parallelisation
        *   strategy provides a speed-up, and only becomes comparatively
        *   smaller with increasing total number of pairwise calculations.
        *   
        *   We can go some way to compensating for this small imbalance by
        *   launching threads which call _pairwise_launch_bounded() in an order
        *   reversed compared to that of _pairwise_as_t in a_argument_sets.
        *   (Note that, while libpairwise threads can indeed run in parallel,
        *   they must be launched sequentially by the parent thread.)
        */
        
        n_collections_used = 0.5 * (-sqrt((-8 * n_calculations_per_argument_set) + (4 * _PAIRWISE_SQUARE(n_collections_remaining))
                             - (4 * n_collections_remaining) +  1) + (2 * n_collections_remaining) - 1);
        
        n_collections_remaining -= n_collections_used;
        
        if (i_argument_set == (n_argument_sets - 1)) {
        
            i_collection_upper = n_collections;
        
        } else {
        
            i_collection_upper += n_collections_used;
        
        }
        
        /*
        *   Populate this _pairwise_as_t with the calculated parameters. Note
        *   that a number of parameters are constant across all _pairwise_as_t
        *   belonging to the same a_argument_sets; these are f_calculation,
        *   a_collections, n_collections, n_points and n_coordinates.
        */
        
        (a_argument_sets + i_argument_set)->f_calculation = f_calculation;
        
        (a_argument_sets + i_argument_set)->a_collections = a_collections;
        (a_argument_sets + i_argument_set)->a_results = a_results + i_results_offset;
        
        (a_argument_sets + i_argument_set)->n_collections = n_collections;
        (a_argument_sets + i_argument_set)->n_points = n_points;
        (a_argument_sets + i_argument_set)->n_coordinates = n_coordinates;
        
        (a_argument_sets + i_argument_set)->i_collection_lower = i_collection_lower;
        (a_argument_sets + i_argument_set)->i_collection_upper = i_collection_upper;
    
    }

}

/*******************************************************************************

    Symbol: _pairwise_launch_bounded
    
    Type: Function returning void
    
    Intent: Private
    
    Description:
    
        Carries out a subset of all the pairwise calculations to be done which
        is parameterised by an initialised _pairwise_as_t, argument_set.
        
        On success returns nothing. Not expected to fail.
        
    Further Information:
    
        It is expected that multiple calls to this function will be made by
        multiple threads running is parallel, as per the libpairwise
        parallelisation strategy.
        
*******************************************************************************/

void
_pairwise_launch_bounded
(
    
    _pairwise_as_t* argument_set

)
{

    register double (*f_calculation)(size_t n_points,
                                     size_t n_coordinates,
                                     double* collection_a,
                                     double* collection_b);
    
    register double* a_collections;
    register double* a_results;

    register size_t n_collections;
    register size_t n_points;
    register size_t n_coordinates;
    
    register size_t i_collection_lower;
    register size_t i_collection_upper;
    
    register size_t i_collection_a;
    register size_t i_collection_b;
    
    register double* collection_a;
    register double* collection_b;
    
    /*
    *   Extract parameters from argument_set on the heap and place them on the
    *   stack for faster access.
    */
    
    f_calculation = argument_set->f_calculation;
    
    a_collections = argument_set->a_collections;
    a_results = argument_set->a_results;
    
    n_collections = argument_set->n_collections;
    n_points = argument_set->n_points;
    n_coordinates = argument_set->n_coordinates;
    
    i_collection_lower = argument_set->i_collection_lower;
    i_collection_upper = argument_set->i_collection_upper;
    
    /*
    *   Iterate over a subset of pairs of collections, stored in a_collections
    *   and delimited by i_collection_lower and i_collection_upper. Indices for
    *   any given collection pair are i_collection_a and i_collection_b.
    */
    
    for (i_collection_a = i_collection_lower;
         i_collection_a < i_collection_upper;
         i_collection_a ++) {
        
        for (i_collection_b = i_collection_a + 1;
             i_collection_b < n_collections;
             i_collection_b ++) {
            
            /*
            *   Calculate pointers, collection_a and collection_b, to the
            *   elements in a_collections at which the present pair of
            *   collections begin. Note that a_collections has form,
            *   
            *   a_collections = [COLLECTION_1], ..., [COLLECTION_P]
            *   
            *   [COLLECTION_P] = [POINT_1], ..., [POINT_Q]
            *   
            *   [POINT_Q] = [COORDINATE_1], ..., [COORDINATE_R]
            *   
            *   where Q and R, given by n_points and n_coordinates
            *   respectively, are constant across all P collections in
            *   a_collections.
            */
            
            collection_a = a_collections + (i_collection_a * n_points * n_coordinates);
            collection_b = a_collections + (i_collection_b * n_points * n_coordinates);
            
            /*
            *   Pass n_points, n_coordinates, collection_a and collection_b to
            *   the function pointed to by f_calculation and store the result
            *   in the element to which a_results points. Then advance the
            *   a_results pointer by one element in preparation for the next
            *   pairwise calculation.
            */
            
            *(a_results ++) = f_calculation(n_points,
                                            n_coordinates,
                                            collection_a,
                                            collection_b);
        
        }
    
    }

}

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
        
    Further Information:
    
        It is expected that this function will be indirectly called by a public
        wrapper function that binds it to a specific calculation function.
        
        This function allocates memory for an array of pthread_t, a_threads,
        and for an array of _pairwise_as_t, a_argument_sets. It then calls
        _pairwise_populate_argument_sets() to initialise a_argument_sets
        according to its arguments. Thereafter it launches n_threads threads by
        making sequential calls to pthread_create(), each such thread
        instructed to call _pairwise_launch_bounded() with parameters stored in
        one of the initialised _pairwise_as_t in a_argument_sets. This function
        then waits for all child threads to join by making sequential calls to
        pthread_join(), by which time all pairwise calculations have been done.
    
        This function checks the return codes of all functions it calls for
        which it makes sense to do so, and will itself return early with an
        appropriate libpairwise non-zero error code if it detects an error.
        Before any such early return, all allocated memory is first freed.
        
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

)
{

    int n_return;
    
    size_t i_thread;
    
    pthread_t* a_threads;
    
    _pairwise_as_t* a_argument_sets;
    
    /*
    *   If the caller specifies fewer than two collections over which to carry
    *   out pairwise calculations, or that each collection contains no points,
    *   or that each point contains no coordinates, then the total number of
    *   pairwise calculations to be carried out is zero, and we can return
    *   with success without doing anything else.
    */
    
    if (n_collections < 2 || !n_points || !n_coordinates) {
    
        return PAIRWISE_RETURN_SUCCESS;
    
    }
    
    /*
    *   If the caller specifies that zero threads should be used (and there are
    *   pairwise calculations to be carried out) then return with an error.
    */
    
    if (!n_threads) {
    
        return PAIRWISE_RETURN_ERROR_NTHREADS;
    
    }
    
    a_argument_sets = malloc(n_threads * sizeof(_pairwise_as_t));
    
    if (!a_argument_sets) {
        
        return PAIRWISE_RETURN_MALLOC_FAIL;
    
    }
    
    /*
    *   For the special case in which only one thread is requested, forego all
    *   parallelisation overhead. Manually initialise the single _pairwise_as_t
    *   in a_argument_sets to parameterise a call to _pairwise_launch_bounded(
    *   which will carry out all of the pairwise calculationsto be done, and
    *   then make that call.
    */
    
    if (n_threads == 1) {
    
        a_argument_sets->f_calculation = f_calculation;
        
        a_argument_sets->a_collections = a_collections;
        a_argument_sets->a_results = a_results;
        
        a_argument_sets->n_collections = n_collections;
        a_argument_sets->n_points = n_points;
        a_argument_sets->n_coordinates = n_coordinates;
        
        a_argument_sets->i_collection_lower = 0;
        a_argument_sets->i_collection_upper = n_collections;
        
        _pairwise_launch_bounded(a_argument_sets);
        
        free(a_argument_sets);
        
        return PAIRWISE_RETURN_SUCCESS;
    
    }
    
    _pairwise_populate_argument_sets(f_calculation,
                                     n_collections,
                                     n_points,
                                     n_coordinates,
                                     a_collections,
                                     a_results,
                                     n_threads,
                                     a_argument_sets);
    
    a_threads = malloc(n_threads * sizeof(pthread_t));
    
    if (!a_threads) {
    
        free(a_argument_sets);
        
        return PAIRWISE_RETURN_MALLOC_FAIL;
        
    }
    
    /*
    *   Launch a thread for each _pairwise_as_t in a_argument_sets. These
    *   threads are deliberately launched in reverse order (that is, the thread
    *   whose call to _pairwise_launch_bounded is parameterised by the last
    *   element of a_argument_sets is launched first), because distribution of
    *   pairwise calculations across threads is not usually perfectly fair, and
    *   the thread parameterised by the last element of a_argument_sets has to
    *   do a slightly larger number of calculations than the rest.
    */
    
    for (i_thread = n_threads; i_thread; i_thread --) {
        
        n_return = pthread_create(a_threads + i_thread - 1,
                                  NULL,
                                  (void*)_pairwise_launch_bounded,
                                  a_argument_sets + i_thread - 1);
        
        if (n_return) {
        
            /*
            *   If any pthread_create() call fails, return early with an
            *   appropriate error code. Before returning, send cancellation
            *   requests to all other child threads that may already have been
            *   launched successfully. We don't want to return leaving behind
            *   orphaned threads whose references we've freed. Note that
            *   pthread_cancel() only fails if the requested thread ID is
            *   invalid. Since we've necessarily already checked that any
            *   threads whose IDs we're passing to pthread_cancel() were indeed
            *   successfully launched, there's no need to test the return code
            *   here.
            */
        
            for (; i_thread < n_threads; i_thread ++) {
            
                pthread_cancel(*(a_threads + i_thread));
            
            }
        
            free(a_argument_sets);
            free(a_threads);
            
            switch (n_return) {
            
                case EAGAIN: return PAIRWISE_RETURN_PTHREAD_CREATE_EAGAIN;
                
                case EINVAL: return PAIRWISE_RETURN_PTHREAD_CREATE_EINVAL;
                
                case EPERM: return PAIRWISE_RETURN_PTHREAD_CREATE_EPERM;
                
                default: return PAIRWISE_RETURN_PTHREAD_CREATE_UNKNOWN;
            
            }
            
        }
    
    }
    
    /*
    *   Wait for each launched thread to finish, also in reverse order to
    *   complement the order in which they were launched.
    */
    
    for (i_thread = n_threads; i_thread; i_thread --) {
    
        n_return = pthread_join(*(a_threads + i_thread - 1), NULL);
        
        /*
        *   If any pthread_join() call fails, apply the same clean up procedure
        *   as for failed pthread_create() calls above.
        */
        
        if (n_return) {
        
            for (; i_thread < n_threads; i_thread ++) {
            
                pthread_cancel(*(a_threads + i_thread));
            
            }
        
            free(a_argument_sets);
            free(a_threads);
            
            switch (n_return) {
            
                case EDEADLK: return PAIRWISE_RETURN_PTHREAD_JOIN_EDEADLK;
                
                case EINVAL: return PAIRWISE_RETURN_PTHREAD_JOIN_EINVAL;
                
                case ESRCH: return PAIRWISE_RETURN_PTHREAD_JOIN_ESRCH;
                
                default: return PAIRWISE_RETURN_PTHREAD_JOIN_UNKNOWN;
            
            }
            
        }
    
    }
    
    free(a_argument_sets);
    free(a_threads);
    
    return PAIRWISE_RETURN_SUCCESS;
    
}
