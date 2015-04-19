#include "pywise_index.h"

/*******************************************************************************

    Symbol: pywise_index
    
    Type: Function returning PyObject*
    
    Intent: Public exposed in Python as pywise.index()
    
    Python Signature:
    
        pywise.index(n_collections, i_collection_a, i_collection_b) 
            -> numpy.ndarray
    
    Description:
    
        Calculates an index into the NumPy array object returned by one of
        pywise's calculation functions. This index corresponds to the result of
        the calculation between the collection in the set of input collections
        with index i_collection_a, and that with index i_collection_b, given
        the total number of coollections in the set, n_collections.
        
        On success returns a Python integer object. On failure raises a Python
        exception.
    
    Further Information:
    
        This function expects three integer arguments, the first of which must
        be positive. These arguments are passed to libpairwise's
        pairwise_index() which validates them and then stores the sought index
        in i_result. Thereafter i_result is stored in a Python integer object,
        o_result, which is returned to the caller.
        
*******************************************************************************/

PyObject*
pywise_index
(

    PyObject* self,
    PyObject* values,
    PyObject* keys

)
{

    char* keywords[4] = {"n_collections", "i_collection_a", "i_collection_b",
                         NULL};
    
    Py_ssize_t n_collections;
    
    Py_ssize_t i_collection_a;
    Py_ssize_t i_collection_b;
    
    size_t i_result;
    
    PyObject* o_result;
    
    int n_return;
    
    /*
    *   Attempt to parse all arguments as signed integers - even n_collections
    *   which should only ever be positive. This is because overflow checking
    *   is not done when parsing Python numbers as unsigned integers, so an
    *   incorrectly specified negative number would be impossible to detect.
    *   Raise a Python exception if parsing fails.
    */
    
    n_return = PyArg_ParseTupleAndKeywords(values, keys, "nnn:index", keywords,
                                           &n_collections, &i_collection_a,
                                           &i_collection_b);
    
    if (!n_return) {
    
        return NULL;
    
    }
    
    if (n_collections < 0) {
    
        PyErr_Format(PyExc_ValueError, "Argument n_collections must be a "
                     "positive integer.");
        
        return NULL;
    
    }
    
    /*
    *   libpairwise's pairwise_index() does its own internal checks on
    *   n_collections, i_collection_a and i_collection_b before storing the
    *   sought result in i_result. If these checks fail it will return a
    *   non-zero libpairwise error code which in turn directs the appropriate
    *   Python exception to raise.
    */
    
    n_return = pairwise_index(n_collections, i_collection_a, i_collection_b, 
                              &i_result);
    
    if (!n_return) {
    
        o_result = Py_BuildValue("n", i_result);
        
        return o_result;
    
    }
    
    pywise_set_python_exception_from_pairwise_index_return_code(n_return);
    
    return NULL;

}
