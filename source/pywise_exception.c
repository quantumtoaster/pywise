#include "pywise_exception.h"

/*******************************************************************************

    Symbol: pywise_set_python_exception_from_pairwise_calculations_return_code
    
    Type: Function returning void
    
    Intent: Private
    
    Description:
    
        Sets an appropriate Python exception given a failure return code from a
        public libpairwise calculations function.
        
        Ignores unrecognised error codes, returns nothing, and will not fail.
    
    Further Information:
    
        This function is intended to translate a failure return code from any
        libpairwise wrapper function which makes an internal call to the
        private _pairwise_launch() to a Python exception and error string. The
        caller should then return NULL to the Python interpreter to raise that
        exception.
        
*******************************************************************************/

void
pywise_set_python_exception_from_pairwise_calculations_return_code
(
    
    int n_return

)
{

    switch (n_return) {
    
        case PAIRWISE_RETURN_MALLOC_FAIL:
        
            PyErr_Format(PyExc_MemoryError, "Failed to allocate memory for "
                         "parallelisation parameter arrays.");
            
            return;
        
        case PAIRWISE_RETURN_PTHREAD_CREATE_EAGAIN:
        
            PyErr_Format(PyExc_RuntimeError, "Failed to create a thread: "
                         "either insufficient resources, or a system limit on "
                         "the number of threads would be breached.");
            
            return;
        
        case PAIRWISE_RETURN_PTHREAD_CREATE_EINVAL:
        
            PyErr_Format(PyExc_RuntimeError, "Failed to create a thread: "
                         "requested settings for the thread are invalid.");
            
            return;
        
        case PAIRWISE_RETURN_PTHREAD_CREATE_EPERM:
        
            PyErr_Format(PyExc_RuntimeError, "Failed to create a thread: "
                         "insufficient permissions to set the requested "
                         "scheduling policy and parameters for the thread.");
            
            return;
        
        case PAIRWISE_RETURN_PTHREAD_CREATE_UNKNOWN:
        
            PyErr_Format(PyExc_RuntimeError, "Failed to create a thread: "
                         "unknown reason.");
            
            return;
        
        case PAIRWISE_RETURN_PTHREAD_JOIN_EDEADLK:
        
            PyErr_Format(PyExc_RuntimeError, "Failed to join a thread: "
                         "detected a thread joining deadlock; either two "
                         "threads tried to join with one another, or one "
                         "thread tried to join with itself.");
            
            return;
        
        case PAIRWISE_RETURN_PTHREAD_JOIN_EINVAL:
        
            PyErr_Format(PyExc_RuntimeError, "Failed to join a thread: either "
                         "the thread is not joinable, or another thread is "
                         "already waiting to join this thread.");
            
            return;
        
        case PAIRWISE_RETURN_PTHREAD_JOIN_ESRCH:
        
            PyErr_Format(PyExc_RuntimeError, "Failed to join a thread: no "
                         "thread with the requested ID could be found.");
            
            return;
        
        case PAIRWISE_RETURN_PTHREAD_JOIN_UNKNOWN:
        
            PyErr_Format(PyExc_RuntimeError, "Failed to join a thread: "
                         "unknown reason.");
            
            return;
        
        
        case PAIRWISE_RETURN_ERROR_NTHREADS:
        
            PyErr_Format(PyExc_ValueError, "Argument threads must be a "
                         "positive integer.");
            
            return;
        
    }

}

/*******************************************************************************

    Symbol: pywise_set_python_exception_from_pairwise_index_return_code
    
    Type: Function returning void
    
    Intent: Private
    
    Description:
    
        Sets an appropriate Python exception given a failure return code from
        libpairwise's public pairwise_index().
        
        Ignores unrecognised error codes, returns nothing, and will not fail.
        
    Further Information:
    
        This function is intended to translate a failure return code from
        pairwise_index() to a Python exception and error string. The caller
        should then return NULL to the Python interpreter to raise that
        exception.
        
*******************************************************************************/

void
pywise_set_python_exception_from_pairwise_index_return_code
(
    
    int n_return

)
{

    switch (n_return) {
    
        case PAIRWISE_RETURN_ERROR_NCOLLECTIONS:
        
            PyErr_Format(PyExc_ValueError, "Argument n_collections must be a "
                         "greater-than-one integer.");
            
            return;
        
        case PAIRWISE_RETURN_ERROR_ICOLLECTIONA:
        
            PyErr_Format(PyExc_IndexError, "Argument i_collections_a must be "
                         "a valid index into an array of n_collections "
                         "collections.");
            
            return;
        
        case PAIRWISE_RETURN_ERROR_ICOLLECTIONB:
        
            PyErr_Format(PyExc_IndexError, "Argument i_collections_b must be "
                         "a valid index into an array of n_collections "
                         "collections.");
            
            return;
        
        case PAIRWISE_RETURN_ERROR_ICOLLECTIONSAME:
        
            PyErr_Format(PyExc_IndexError, "Arguments i_collections_a and "
                         "i_collections_b must not both refer to the same "
                         "collection.");
            
            return;
    
    }

}
