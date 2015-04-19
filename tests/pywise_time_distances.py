#!/usr/bin/env python

# pywise_time_distances.py
#
# A timer for comparing like calls to scipy.spatial.distance.pdist() and
# pywise.distances().
#
# Usage: python pywise_test_rmsds.py POINTS THREADS [PYWISE_SO_PATH]

import sys
import timeit

title = "pywise_time_single_distances.py"
usage = "Usage: python %s POINTS THREADS [PYWISE_SO_PATH]" % title

n_coords = 3


if __name__ == "__main__":

    if len(sys.argv) not in (3, 4) or "-h" in sys.argv or "--help" in sys.argv:
        
        print(usage)
        exit(1)
    
    try:
    
        n_points = int(sys.argv[1])
       
        if n_points < 2:
            raise Exception
    
    except:
        
        print("%s: Failed - POINTS must be a greater-than-one integer."
              % title)
        exit(1)
    
    try:
    
        n_threads = int(sys.argv[2])
       
        if n_threads < 1:
            raise Exception
    
    except:
        
        print("%s: Failed - THREADS must be a greater-than-zero integer."
              % title)
        exit(1)
    
    path_pywise = sys.argv[3] if len(sys.argv) == 4 else ""
    
    setup = list()
    
    setup.append("import sys")
    setup.append("sys.path.append(\"%s\")" % path_pywise)

    setup.append("import pywise")
    setup.append("import numpy")
    setup.append("import scipy.spatial")

    setup.append("p = numpy.random.rand(%d, %d)" % (n_points, n_coords))
    
    setup = "; ".join(setup)
    
    timer_pywise = timeit.Timer("pywise.distances(p, %d)" % n_threads, setup)
    result_pywise = timer_pywise.timeit(1)
    
    timer_scipy = timeit.Timer("scipy.spatial.distance.pdist(p)", setup)
    result_scipy = timer_scipy.timeit(1)
    
    print("# points, threads, pywise, scipy")
    print("%e,%d,%e,%e" % (n_points, n_threads, result_pywise, result_scipy))
    
