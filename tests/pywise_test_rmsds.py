#!/usr/bin/env python

# pywise_test_rmsds.py
#
# A unit test for both single- and multi-threaded calls to pywise.rmsds().
#
# Usage: python pywise_test_rmsds.py

import sys
import os

n_colls = 100
n_points = 50
n_coords = 3
n_threads = 8

test_name = "pywise_test_rmsds.py"


def native_single_rmsd(collection_a, collection_b):

    """Return the RMSD between collection_a and collection_b."""

    delta = collection_a - collection_b
    sum_squared_dists = float(0)

    for point in delta:
        squared_dist = numpy.sum(point**2)
        sum_squared_dists += squared_dist
    
    mean_squared_dist = sum_squared_dists / len(delta)
    
    return numpy.sqrt(mean_squared_dist)


def native_rmsds(collections):

    """Return a one-dimensional array containing all pairwise RMSDs calculated
    across collections."""

    rmsds = numpy.zeros((0.5 * len(collections) * (len(collections) - 1)))
    count = 0
    
    for i in xrange(len(collections)):
        for j in xrange(i + 1, len(collections)):
            rmsds[count] = native_single_rmsd(collections[i], collections[j])
            count += 1
    
    return rmsds


if __name__ == "__main__":

    # Add the distribution root to the system path in case pywise has been
    # built in place, rather than installed permanently.
    
    test_directory = os.path.dirname(os.path.realpath(__file__))
    dist_directory = os.path.join(test_directory, os.path.pardir)
    
    sys.path.append(dist_directory)
    
    # Try to import the required modules, including pywise.
    
    try:
    
        import pywise
    
    except:
        
        print("%s: Failed - couldn't import pywise." % test_name)
        exit(1)
    
    try:
    
        import numpy
    
    except:
    
        printf("%s: Failed - couldn't import NumPy." % test_name)
        exit(1)
    
    # Calculate all pairwise RMSDs across a set of randomly-generated
    # collections three times: using pywise in single-threaded mode, using
    # pywise in multi-threaded mode, and using a dumb native implementation.
    
    colls = numpy.random.rand(n_colls, n_points, n_coords)
    
    rmsds_pywise_singlethread = pywise.rmsds(colls, 1)
    rmsds_pywise_multithread = pywise.rmsds(colls, n_threads)
    
    rmsds_native = native_rmsds(colls)
    
    # Check that the results of all sets of RMSD calculations are identical.
    # Depending on the libm implementation linked to pywise, the result of
    # sqrt() may differ very slightly at high precision between pywise and the
    # native implementation. In this case numpy.allclose() is a more
    # appropriate test for equality.
    
    if not numpy.array_equal(rmsds_pywise_singlethread, rmsds_native):
            
        if not numpy.allclose(rmsds_pywise_singlethread, rmsds_native):
    
            print("%s: Failed - pairwise RMSDs from single-threaded pywise and "
                  "native are different." % test_name)
            exit(1)
        
        print("%s: Note - pairwise RMSDs from single-threaded pywise and "
              "native are not exactly the same, but are within tolerance."
              % test_name)
    
    if not numpy.array_equal(rmsds_pywise_multithread, rmsds_native):
            
        if not numpy.allclose(rmsds_pywise_multithread, rmsds_native):
    
            print("%s: Failed - pairwise RMSDs from multi-threaded pywise and "
                  "native are different." % test_name)
            exit(1)
        
        print("%s: Note - pairwise RMSDs from multi-threaded pywise and native "
              "are not exactly the same, but are within tolerance." % test_name)
    
    print("%s: Passed!" % test_name)
    
