#!/usr/bin/env python

import os
import sys

import subprocess
import distutils.core

if __name__ == "__main__":

    # Clean libpairwise and remove pywise.so for each new build and each clean.
    
    if sys.argv[1].startswith("build") or sys.argv[1].startswith("clean"):
        
        make_clean = subprocess.Popen(
            
            args = ["make", "clean"],
            cwd = "libpairwise"
        
        )
        
        make_clean.wait()
        
        if make_clean.returncode:
        
            sys.stderr.write("Call to \"make clean\" in libpairwise failed.\n")
            exit(make_clean.returncode)
        
        try: os.unlink("pywise.so")
        
        except: pass
    
    
    # Build libpairwise if the static library archive libpairwise.a doesn't
    # exist.
    
    if sys.argv[1].startswith("build") \
    and not os.path.isfile(os.path.join("libpairwise", "libpairwise.a")):
        
        make = subprocess.Popen(
            
            args = ["make"],
            cwd = "libpairwise"
        
        )
        
        make.wait()
        
        if make.returncode:
        
            sys.stderr.write("Call to \"make\" in libpairwise failed.\n")
            exit(make.returncode)
    
    
    # Usual distutils setup procedure from here onwards.
    
    pywise_extension = distutils.core.Extension(
        
        name = "pywise",
        
        sources = [
            
            os.path.join("source", "pywise_exception.c"),
            os.path.join("source", "pywise_build_collections_array.c"),
            os.path.join("source", "pywise_build_points_array.c"),
            os.path.join("source", "pywise_rmsds.c"),
            os.path.join("source", "pywise_distances.c"),
            os.path.join("source", "pywise_index.c"),
            os.path.join("source", "pywise.c")
        
        ],
        
        include_dirs = [
            
            "include",
            os.path.join("libpairwise", "include")
        
        ],
        
        library_dirs = [
            
            "libpairwise"
        
        ],
        
        libraries = [
            
            "pairwise"
        
        ],
        
        extra_compile_args = [
            
            "-Ofast"
        
        ]

    )

    distutils.core.setup(
        
        name = "pywise",
        
        version = "1.0",
        
        license = "GNU General Public License Version 2",
        
        description = "A Python module to provide bindings to libpairwise.",
        
        author = "Adam J. Marsden",
        
        ext_modules = [pywise_extension]
        
    )

