#include "pywise_distances.h"

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
    
    Further Information:
    
        This function receives a Python object which contains a two-dimensional
        sequence of sequences of numbers (representing a set of points in any-
        dimensional space) and passes it pywise_build_points_array() which
        copies its contents into an input array of form appropriate for passing
        to libpairwise's pairwise_distances(). Thereafter this function passes
        that input array to pairwise_distances(), and then returns the
        resulting output array which contains the calculated pairwise distances
        in the form of a NumPy array object.
        
*******************************************************************************/

PyObject*
pywise_distances
(

    PyObject* self,
    PyObject* values,
    PyObject* keys

)
{

    char* keywords[3] = {"points", "threads", NULL};
    
    size_t n_points;
    size_t n_coordinates;
    
    Py_ssize_t n_threads;
    
    PyObject* o_points;
    PyObject* o_distances;
    
    double* a_points;
    double* a_distances;
    
    size_t l_a_distances;
    size_t s_a_distances;
    
    npy_intp npy_l_a_distances[1];
    
    int n_return;
    
    /*
    *   Set the default number of threads to use if the user doesn't supply
    *   the threads argument.
    */
    
    n_threads = PYWISE_DEFAULT_THREADS;
    
    /*
    *   Attempt to parse aruguments with keywords "points" and "threads" as a
    *   Python object and a signed integer, respectively. Even though the
    *   number of threads should only ever be positive, overflow checking is
    *   not done when parsing unsigned integers, so an incorrectly specified
    *   negative number parsed in that way would be impossible to detect. Raise
    *   a Python exception if parsing fails. 
    */
    
    n_return = PyArg_ParseTupleAndKeywords(values, keys, "O|n:distances",
                                           keywords, &o_points, &n_threads);
    
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
    *   Build an an input array of points, a_points, from the caller-supplied
    *   Python object, o_points. pywise_build_points_array() sets by itself
    *   an appropriate Python exception on failure.
    */
    
    a_points = pywise_build_points_array(o_points, &n_points, &n_coordinates);
    
    if (!a_points) {
    
        return NULL;
    
    }
    
    /*
    *   Knowing now how many points across which we must calculate pairwise
    *   distances, allocate memory for the output distances array, a_distances.
    */
    
    l_a_distances = 0.5 * n_points * (n_points - 1);
    
    s_a_distances = l_a_distances * sizeof(double);
    
    a_distances = malloc(s_a_distances);
    
    if (!a_distances) {
    
        PyErr_Format(PyExc_MemoryError, "Failed to allocate memory for output "
                     "distances array; needed %zu bytes.", s_a_distances);
        
        free(a_points);
        
        return NULL;
    
    }
    
    /*
    *   Calculate pairwise distances across all points in a_points,
    *   distributing the calculations to be carried out over n_threads parallel
    *   threads, and store the calculated distances in a_distances.
    */
    
    n_return = pairwise_distances(n_points,
                                  n_coordinates,
                                  a_points,
                                  a_distances,
                                  n_threads);
    
    free(a_points);
    
    if (!n_return) {
    
        /*
        *   If pairwise_distances() succeeded, wrap a_distances in a NumPy
        *   array object o_distances, transfer ownership of the memory pointed
        *   to by a_distances to o_distances, and then return o_distances.
        */
    
        npy_l_a_distances[0] = l_a_distances;
        
        o_distances = PyArray_SimpleNewFromData(1,
                                                npy_l_a_distances,
                                                NPY_DOUBLE,
                                                a_distances);
        
        #if defined(NPY_ARRAY_OWNDATA)
        PyArray_ENABLEFLAGS((PyArrayObject*)o_distances, NPY_ARRAY_OWNDATA);
        #else
        PyArray_ENABLEFLAGS((PyArrayObject*)o_distances, NPY_OWNDATA);
        #endif
        
        return o_distances;
    
    }
    
    /*
    *   Otherwise if pairwise_distances() failed, free the memory pointed to by
    *   a_distances, and then raise a Python exception appropriate to the
    *   libpairwise return code from pairwise_distances().
    */
    
    free(a_distances);
    
    pywise_set_python_exception_from_pairwise_calculations_return_code(n_return);
    
    return NULL;

}
