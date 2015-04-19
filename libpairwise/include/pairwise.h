#ifndef PAIRWISE_H
#define PAIRWISE_H

/* Shared library definitions. */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>

/* libpairwise definitions. */

#define PAIRWISE_VERSION "1.0"

#define _PAIRWISE_SQUARE(x) ((x) * (x))

/* Public return codes for success and failures. */
#include "pairwise_error.h"

/* Private dependencies for any public function carrying out calculations. */
#include "pairwise_launch.h"

/* Public pairwise_distances() and private dependencies. */
#include "pairwise_distances.h"

/* Public pairwise_rmsds() and private dependencies. */
#include "pairwise_rmsds.h"

/* Public pairwise_index(). */
#include "pairwise_index.h"

#endif /* PAIRWISE_H */
