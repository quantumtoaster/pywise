#ifndef PYWISE_COMMON_H
#define PYWISE_COMMON_H

/* libpairwise definitions. */

#include "pairwise.h"

/* Python and NumPy C-API definitions. */

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#define PY_ARRAY_UNIQUE_SYMBOL PYWISE_ARRAY_API

#include <Python.h>
#include <numpy/arrayobject.h>

/* pywise definitions. */

#define PYWISE_VERSION "1.0"

#define PYWISE_DEFAULT_THREADS 1

#define PYWISE_ERROR_BUFFER_LENGTH 500

#include "pywise_exception.h"

#include "pywise_build_points_array.h"
#include "pywise_build_collections_array.h"

#include "pywise_distances.h"
#include "pywise_rmsds.h"
#include "pywise_index.h"

#endif
