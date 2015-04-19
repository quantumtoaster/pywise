#include "pywise_build_collections_array.h"

/*******************************************************************************

    Symbol: pywise_build_collections_array
    
    Type: Function returning double*
    
    Intent: Private
    
    Description:
    
        Builds from a suitable Python object an array of collections of form
        appropriate for passing to a libpairwise calculation function.
        
        o_source is a pointer to the input Python object. On success stores the
        number of collections in o_source, the number of points per collection,
        and the number of coordinates per point in n_collections, n_points and
        n_coordinates respectively. Also returns a pointer to a new array of
        these collections, the responsibility to free which is passed on to the
        caller. On failure sets a Python exception and returns a null pointer.
    
    Further Information:
    
        This function allocates memory for a contiguous array of C doubles into
        which it copies the set of collections stored in o_source. During the
        course of this copy this function also validates the form and element
        types of o_source. It is expected that o_source will be a three-
        dimensional sequence of sequences of sequences of numbers having form,
        
            o_source <- [[COLLECTION_1], [COLLECTION_2], ..., [COLLECTION_A]]
        
        with,
        
            [COLLECTION_A] <- [[POINT_1], [POINT_2], ..., [POINT_B]]
        
        and,
        
            [POINT_B] <- [[COORDINATE_1], [COORDINATE_2], ..., [COORDINATE_C]]
        
        where [COORDINATE_C] is a Python number, and additionally B and C, the
        number of points per collection and the number of coordinates per point
        respectively, are constant across all A collections.
        
*******************************************************************************/

double*
pywise_build_collections_array
(
    
    PyObject* o_source,
    
    size_t* n_collections,
    size_t* n_points,
    size_t* n_coordinates

)
{

    PyObject** a_o_collections;
    PyObject** a_o_points;
    PyObject** a_o_coordinates;
    
    PyObject* o_collection;
    PyObject* o_point;
    PyObject* o_coordinate;
    
    Py_ssize_t n_collections_temp;
    Py_ssize_t n_points_temp;
    Py_ssize_t n_coordinates_temp;
    
    Py_ssize_t i_collection;
    Py_ssize_t i_point;
    Py_ssize_t i_coordinate;
    
    double* a_collections;
    double* a_collections_temp;
    
    size_t l_a_collections;
    size_t s_a_collections;
    
    char* b_error;
    
    size_t s_b_error;
    
    /*
    *   Initialise pointers to the output array (yet to be allocated) to null.
    *   This suppresses compiler warnings about potential uninitialisations
    *   which come from having the memory allocation for a_collections nested
    *   in a conditional block. (The compiler doesn't know that this
    *   conditional block will always be executed exactly once in a successful
    *   call - so those warnings are actually unfounded!)
    */
    
    a_collections = NULL;
    a_collections_temp = NULL;
    
    /*
    *   Allocate memory for an error buffer. This allows us to report error
    *   strings which contain runtime information via Python functions which
    *   don't support the usual variadic sprintf() syntax, but which do insist
    *   on setting the Python exception buffer for us.
    */
    
    s_b_error = PYWISE_ERROR_BUFFER_LENGTH * sizeof(char);
    
    b_error = malloc(s_b_error);
    
    if (!b_error) {
    
        PyErr_Format(PyExc_MemoryError, "Failed to allocate memory for error "
                     "string buffer; needed %zu bytes.", s_b_error);
        
        goto exception;
    
    }
    
    /*
    *   Ensure that the first dimension of o_source supports the sequence
    *   protocol, return a reference to it appropriate for fast access, and
    *   then ensure that it has non-zero length.
    */
    
    o_source = PySequence_Fast(o_source, "Input array of collections must be a "
                               "sequence.");
        
    if (!o_source) {
        
        PyErr_Format(PyExc_RuntimeError, "Failed to convert the input array of "
                     "collections into a list for fast access.");
        
        goto exception;
    
    }
    
    n_collections_temp = PySequence_Length(o_source);
    
    if (n_collections_temp < 0) {
    
        PyErr_Format(PyExc_RuntimeError, "Failed to retrieve the length of the "
                     "input array of collections.");
        
        goto exception;
    
    } else if (!n_collections_temp) {
    
        PyErr_Format(PyExc_IndexError, "Input array of collections must be a "
                     "non-empty sequence.");
        
        goto exception;
    
    }
    
    /*
    *   Having checked n_collections_temp, the length of the first dimension of
    *   o_collections, for less-than-zero error codes, we can now safely store
    *   this value in the unsigned n_collections.
    */
    
    *n_collections = n_collections_temp;
    
    /*
    *   Return a reference, a_o_collections, to the underlying element buffer
    *   of the first dimension of o_source for fast access. This will be an
    *   array of Python objects which represents the first dimension of
    *   o_source - that is, one object per collection.
    */
    
    a_o_collections = PySequence_Fast_ITEMS(o_source);
    
    /*
    *   Loop over each individual collection in the array a_o_collections.
    */
    
    for (i_collection = 0; i_collection < n_collections_temp; i_collection ++) {
        
        /*
        *   Get a reference, o_collection, to the Python object which
        *   represents the single collection presently under consideration.
        */
        
        o_collection = *(a_o_collections + i_collection);
        
        /*
        *   Ensure that, o_collection, the element of the first dimension of
        *   o_source presently under consideration, supports the sequence
        *   protocol, return a reference to it appropriate for fast access, and
        *   then ensure that it has non-zero length.
        */
        
        sprintf(b_error, "Collection %zu must be a sequence.", i_collection);
        
        o_collection = PySequence_Fast(o_collection, b_error);
        
        if (!o_collection) {
            
            PyErr_Format(PyExc_RuntimeError, "Failed to convert collection %zu "
                         "into a list for fast access.", i_collection);
            
            goto exception;
        
        }
        
        n_points_temp = PySequence_Length(o_collection);
        
        if (n_points_temp < 0) {
        
            PyErr_Format(PyExc_RuntimeError, "Failed to retrieve the length of "
                         "the sequence which is expected to represent "
                         "collection %zu.", i_collection);
            
            goto exception;
        
        } else if (!n_points_temp) {
        
            PyErr_Format(PyExc_IndexError, "Collection %zu must be a non-empty "
                         "sequence.", i_collection);
            
            goto exception;
        
        }
        
        if (!i_collection) {
            
            /*
            *   If this is the first element of the first dimension of
            *   o_source - that is, the first single collection - we've
            *   encountered, store its length (having already been checked for
            *   less-than-zero error codes) in the unsigned n_points.
            */
            
            *n_points = n_points_temp;
        
        } else if (n_points_temp != *n_points) {
        
            /*
            *   Otherwise if this is any other element of the first dimension
            *   of o_source, check that its length is the same as that of
            *   the first such element, and raise an exception if it's not.
            */
        
            PyErr_Format(PyExc_IndexError, "Collection %zu must have the same "
                         "number of points as all other collections; "
                         "collection %zu has %zu point(s), and all others have "
                         "%zu.", i_collection, i_collection, n_points_temp,
                         *n_points);
            
            goto exception;
        
        }
        
        /*
        *   Return a reference, a_o_points, to the underlying element buffer
        *   of the second dimension of o_collections for fast access. This will
        *   be an array of Python objects which represents the second dimension
        *   of o_collections - that is, one object per point.
        */
        
        a_o_points = PySequence_Fast_ITEMS(o_collection);
        
        /*
        *   Loop over each individual point in the array a_o_points.
        */
        
        for (i_point = 0; i_point < n_points_temp; i_point ++) {
        
            /*
            *   Get a reference, o_point, to the Python object which represents
            *   the single point presently under consideration.
            */
            
            o_point = *(a_o_points + i_point);
            
            /*
            *   Ensure that, o_point, the element of the second dimension of
            *   o_collections presently under consideration, supports the
            *   sequence protocol, return a reference to it appropriate for
            *   fast access, and then ensure that it has non-zero length.
            */
            
            sprintf(b_error, "Point %zu of collection %zu must be a sequence.",
                    i_collection, i_point);
            
            o_point = PySequence_Fast(o_point, b_error);
            
            if (!o_point) {
                
                PyErr_Format(PyExc_RuntimeError, "Failed to convert point %zu "
                             "into a list for fast access.", i_point);
                
                goto exception;
            
            }
            
            n_coordinates_temp = PySequence_Length(o_point);
            
            if (n_coordinates_temp < 0) {
            
                PyErr_Format(PyExc_RuntimeError, "Failed to retrieve the "
                             "length of the sequence which is expected to "
                             "represent point %zu.", i_point);
                
                goto exception;
            
            } else if (!n_coordinates_temp) {
            
                PyErr_Format(PyExc_IndexError, "Point %zu must be a non-empty "
                             "sequence.", i_point);
                
                goto exception;
            
            }
            
            if (!i_point) {
            
                /*
                *   If this is the first element of the second dimension of
                *   o_collections - that is, the first single point - we've
                *   encountered, store its length (having already been checked
                *   for less-than-zero error codes) in the unsigned
                *   n_coordinates.
                */
            
                *n_coordinates = n_coordinates_temp;
                
                if (!i_collection) {
                
                    /*
                    *   If this is the first point of the first collection, we
                    *   now have sufficient length information to allocate
                    *   memory for the output array (insodoing enforcing that
                    *   all subsequent collections have identical form).
                    *   Attempt to allocate the required memory, and raise an
                    *   exception on failure.
                    */
                
                    l_a_collections = n_collections_temp * n_points_temp * n_coordinates_temp;
                    
                    s_a_collections = l_a_collections * sizeof(double);
                    
                    a_collections = malloc(s_a_collections);
                    
                    if (!a_collections) {
                    
                        PyErr_Format(PyExc_MemoryError, "Failed to allocate "
                                     "memory for input collections array; "
                                     "needed %zu bytes.", s_a_collections);
                        
                        goto exception;
                    
                    }
                    
                    /*
                    *   Store a copy of the pointer to the output array in
                    *   a_collections_temp for returning to the caller later;
                    *   the pointer a_collections will be incremented as the
                    *   output array is populated by the remainder of this
                    *   function.
                    */
                    
                    a_collections_temp = a_collections;
                
                }
            
            } else if (n_coordinates_temp != *n_coordinates) {
                
                /*
                *   Otherwise if this is any other element of the second
                *   dimension of o_source, check that its length is the same as
                *   that of the first such element, and raise an exception if
                *   it's not.
                */
                
                PyErr_Format(PyExc_IndexError, "Point %zu must have the same "
                             "number of coordinates as all other points; point "
                             "%zu has %zu coordinate(s), and all others have "
                             "%zu.", i_point, i_point, n_coordinates_temp,
                             *n_coordinates);
                
                goto exception;
            
            }
            
            /*
            *   Return a reference, a_o_coordinates, to the underlying element
            *   buffer of the third dimension of o_source for fast access. This
            *   will be an array of Python objects which represents the third
            *   dimension of o_source - that is, one object per coordinate.
            */
            
            a_o_coordinates = PySequence_Fast_ITEMS(o_point);
            
            /*
            *   Loop over each individual coordinate in the array
            *   a_o_coordinates.
            */
            
            for (i_coordinate = 0;
                 i_coordinate < n_coordinates_temp;
                 i_coordinate ++) {
                
                /*
                *   Get a reference, o_coordinate, to the Python object which
                *   represents the single coordinate presently under
                *   consideration.
                */
                
                o_coordinate = *(a_o_coordinates + i_coordinate);
                
                /*
                *   Ensure that, o_coordinate, the element of the third
                *   dimension of o_source presently under consideration, is
                *   either a Python float object or a Python integer object.
                *   If so, perform the appropriate conversion to a C double,
                *   and then store the value in the next element in nascent
                *   output array. Otherwise raise a Python exception.
                */
                
                if (PyFloat_Check(o_coordinate)) {
                    
                    *(a_collections_temp ++) = PyFloat_AS_DOUBLE(o_coordinate);
                
                } else if (PyInt_Check(o_coordinate)) {
                
                    *(a_collections_temp ++) = (double)PyInt_AS_LONG(o_coordinate);
                
                } else {
                    
                    PyErr_Format(PyExc_TypeError, "Coordinate %zu of point %zu "
                                 "of collection %zu must be either an integer "
                                 "or a float.", i_coordinate, i_point,
                                 i_collection);
                    
                    goto exception;
                
                }
            
            }
        
        }
    
    }
    
    /*
    *   Free only memory allocated for the error buffer, and then return a
    *   pointer to the successfully populated output array.
    */
    
    free(b_error);
    
    return a_collections;
    
exception:
    
    /*
    *   Free all allocated memory, and then signal that the caller raise a
    *   Python exception by returning a null pointer.
    */
    
    free(b_error);
    
    free(a_collections);
    
    return NULL;

}
