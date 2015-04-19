#include "pywise.h"

/*******************************************************************************

    Symbol: pywise_methods
    
    Type: Array of PyMethodDef
    
    Intent: Private
    
    Description:
    
        A manifest of all public methods to be exposed by the pywise module.
        
*******************************************************************************/

PyMethodDef
pywise_methods[] = {

	{
	
	    "distances",
	    (PyCFunction)pywise_distances,
	    METH_VARARGS | METH_KEYWORDS,
	    NULL
	
	},
	
	{
	
	    "rmsds",
	    (PyCFunction)pywise_rmsds,
	    METH_VARARGS | METH_KEYWORDS,
	    NULL
	
	},
	
	{
	
	    "index",
	    (PyCFunction)pywise_index,
	    METH_VARARGS | METH_KEYWORDS,
	    NULL
	
	},
	
	{
	    
	    NULL,
	    NULL,
	    0,
	    NULL
	
	}
	
};

/*******************************************************************************

    Symbol: initpywise
    
    Type: Function returning PyMODINIT_FUNC
    
    Intent: Private
    
    Description:
    
        Registers the methods listed in pywise_methods, registers module
        constants, and then initialises the C-API for NumPy arrays.
        Automatically called by the Python interpreter when the pywise module
        is imported.
        
*******************************************************************************/

PyMODINIT_FUNC
initpywise
(void)
{

    PyObject* o_module;
    
    o_module = Py_InitModule("pywise", pywise_methods);
    
    PyModule_AddStringConstant(o_module, "__version__", PYWISE_VERSION);
    
    import_array();

}
