#include "pywise_rmsds.h"

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
    
    Further Information:
    
        This function receives a Python object which contains a three
        dimensional sequence of sequences of sequences of numbers (representing
        a set of collections of points in any-dimensional space) and passes it
        pywise_build_collections_array() which copies its contents into an
        input array of form appropriate for passing to libpairwise's
        pairwise_rmsds(). Thereafter this function passes that input array to
        pairwise_rmsds(), and then returns the resulting output array which
        contains the calculated pairwise RMSDs in the form of a NumPy array
        object.
        
*******************************************************************************/

PyObject*
pywise_rmsds
(

    PyObject* self,
    PyObject* values,
    PyObject* keys

)
{

    char* keywords[3] = {"collections", "threads", NULL};
    
    size_t n_collections;
    size_t n_points;
    size_t n_coordinates;
    
    Py_ssize_t n_threads;
    
    PyObject* o_collections;
    PyObject* o_rmsds;
    
    double* a_collections;
    double* a_rmsds;
    
    size_t l_a_rmsds;
    size_t s_a_rmsds;
    
    npy_intp npy_l_a_rmsds[1];
    
    int n_return;
    
    /*
    *   Set the default number of threads to use if the user doesn't supply
    *   the threads argument.
    */
    
    n_threads = PYWISE_DEFAULT_THREADS;
    
    /*
    *   Attempt to parse aruguments with keywords "collections" and "threads"
    *   as a Python object and a signed integer, respectively. Even though the
    *   number of threads should only ever be positive, overflow checking is
    *   not done when parsing unsigned integers, so an incorrectly specified
    *   negative number parsed in that way would be impossible to detect. Raise
    *   a Python exception if parsing fails. 
    */
    
    n_return = PyArg_ParseTupleAndKeywords(values, keys, "O|n:rmsds", keywords,
                                           &o_collections, &n_threads);
    
    if (!n_return) {
        
        return NULL;
    
    }
    
    /*
    *   Ensure that the requested number of threads is greater-than-zero.
    */
    
    if (n_threads < 0) {
    
        PyErr_Format(PyExc_ValueError, "Argument threads must be a positive "
                     "integer.");
        
        return NULL;
    
    }
    
    /*
    *   Future versions of pywise could dynamically detect the number of
    *   processor cores made available host, and then use that many threads
    *   given the special value of zero for the threads argument.
    */
    
    if (!n_threads) {
    
        PyErr_Format(PyExc_NotImplementedError, "Detection of number of "
                     "processors provided by host not yet implemented.");
        
        return NULL;
    
    }
    
    /*
    *   Build an an input array of collections, a_collections, from the
    *   caller-supplied Python object, o_collections.
    *   pywise_build_collections_array() sets by itself an appropriate Python
    *   exception on failure.
    */
    
    a_collections = pywise_build_collections_array(o_collections,
                                                   &n_collections,
                                                   &n_points,
                                                   &n_coordinates);
    
    if (!a_collections) {
    
        return NULL;
    
    }
    
    /*
    *   Knowing now how many collections across which we must calculate
    *   pairwise RMSDs, allocate memory for the output distances array,
    *   a_rmsds.
    */
    
    l_a_rmsds = 0.5 * n_collections * (n_collections - 1);
    
    s_a_rmsds = l_a_rmsds * sizeof(double);
    
    a_rmsds = malloc(s_a_rmsds);
    
    if (!a_rmsds) {
    
        PyErr_Format(PyExc_MemoryError, "Failed to allocate memory for output "
                     "RMSDs array; needed %zu bytes.", s_a_rmsds);
        
        free(a_collections);
        
        return NULL;
    
    }
    
    /*
    *   Calculate pairwise RMSDs across all collections in a_collections,
    *   distributing the calculations to be carried out over n_threads parallel
    *   threads, and store the calculated distances in a_rmsds.
    */
    
    n_return = pairwise_rmsds(n_collections,
                              n_points,
                              n_coordinates,
                              a_collections,
                              a_rmsds,
                              n_threads);
    
    free(a_collections);
    
    if (!n_return) {
        
        /*
        *   If pairwise_rmsds() succeeded, wrap a_rmsds in a NumPy array object
        *   o_rmsds, transfer ownership of the memory pointed to by a_rmsds to
        *   o_rmsds, and then return o_rmsds.
        */
        
        npy_l_a_rmsds[0] = l_a_rmsds;
        
        o_rmsds = PyArray_SimpleNewFromData(1,
                                            npy_l_a_rmsds,
                                            NPY_DOUBLE,
                                            a_rmsds);
        
        #if defined(NPY_ARRAY_OWNDATA)
        PyArray_ENABLEFLAGS((PyArrayObject*)o_rmsds, NPY_ARRAY_OWNDATA);
        #else
        PyArray_ENABLEFLAGS((PyArrayObject*)o_rmsds, NPY_OWNDATA);
        #endif
        
        return o_rmsds;
    
    }
    
    /*
    *   Otherwise if pairwise_rmsds() failed, free the memory pointed to by
    *   a_rmsds, and then raise a Python exception appropriate to the
    *   libpairwise return code from pairwise_rmsds().
    */
    
    free(a_rmsds);
    
    pywise_set_python_exception_from_pairwise_calculations_return_code(n_return);
    
    return NULL;

}
