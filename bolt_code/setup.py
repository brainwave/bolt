import os
from distutils.core import setup, Extension


os.environ['CC'] = 'g++';
os.environ['CXX'] = 'g++';

setup (name = 'Slicer',
       version = '1.0',
       description = 'This is the slicer',
       ext_modules = [ 
                    Extension(
                        "slicer", 
                        libraries = ["boost_python", "boost_system", "pthread", "boost_thread"],
                        extra_compile_args = ["-O3", "-funroll-loops","-ftree-vectorize", "--std=c++11" ],
                        sources = ["logic.cpp"],
                        include_dirs = ["/home/brainwave/slicer/libs"],
                        library_dirs = ["/home/brainwave/slicer/libs"]
                        )

       ]) 
