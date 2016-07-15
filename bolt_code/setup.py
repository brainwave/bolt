from distutils.core import setup, Extension


setup (name = 'Slicer',
       version = '1.0',
       description = 'This is the slicer',
       ext_modules = [ 
                    Extension(

                        "slicer", 
                        libraries = ["boost_python", "boost_system", "pthread", "boost_thread"],
                        sources = ["logic.cpp"],
                        include_dirs = ["/home/brainwave/slicer/libs"],
                        library_dirs = ["/home/brainwave/slicer/libs"]
                        )

       ])
