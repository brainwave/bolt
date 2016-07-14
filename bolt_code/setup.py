from distutils.core import setup, Extension

module = Extension('slicer',
                    sources = ['logic.cpp'],
include_dirs = ['/home/brainwave/slicer/libs'],
)
                    
setup (name = 'Slicer',
       version = '1.0',
       description = 'This is the slicer',
       ext_modules = [module])
