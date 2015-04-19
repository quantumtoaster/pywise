#!/usr/bin/env python

# pywise_test_rmsds.py
#
# A unit test for both single- and multi-threaded calls to pywise.distances().
#
# Usage: python pywise_test_distances.py

import sys
import os

n_points = 10000
n_coords = 3
n_threads = 8

test_name = "pywise_test_distances.py"


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
    
    try:
    
        import scipy.spatial
    
    except:
    
        print("%s: Failed - couldn't import scipy.spatial." % test_name)
        exit(1)
    
    # Calculate all pairwise distances across a set of randomly-generated
    # points three times: using pywise in single-threaded mode, using pywise in
    # multi-threaded mode, and using scipy.spatial.pdist().
    
    points = numpy.random.rand(n_points, n_coords)
    
    dists_pywise_singlethread = pywise.distances(points, 1)
    dists_pywise_multithread = pywise.distances(points, n_threads)
    
    dists_scipy = scipy.spatial.distance.pdist(points)
    
    # Check that the results of all sets of distance calculations are
    # identical. Depending on the libm implementation linked to pywise, the
    # result of sqrt() may differ very slightly at high precision between
    # pywise and scipy. In this case numpy.allclose() is a more appropriate
    # test for equality.
    
    if not numpy.array_equal(dists_pywise_singlethread, dists_scipy):
            
        if not numpy.allclose(dists_pywise_singlethread, dists_scipy):
    
            print("%s: Failed - pairwise distances from single-threaded pywise "
                  "and scipy.spatial are different." % test_name)
            exit(1)
        
        print("%s: Note - pairwise distances from single-threaded pywise and "
              "scipy.spatial are not exactly the same, but are within "
              "tolerance." % test_name)
    
    if not numpy.array_equal(dists_pywise_multithread, dists_scipy):
            
        if not numpy.allclose(dists_pywise_multithread, dists_scipy):
    
            print("%s: Failed - pairwise distances from multi-threaded pywise "
                  "and scipy.spatial are different." % test_name)
            exit(1)
        
        print("%s: Note - pairwise distances from multi-threaded pywise and "
              "scipy.spatial are not exactly the same, but are within "
              "tolerance." % test_name)
    
    print("%s: Passed!" % test_name)
    
