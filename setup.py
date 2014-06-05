# -*- coding: utf-8 -*-
#########################
#       SETUP.PY        #
#########################


#########################
# IMPORTS               #
#########################
from setuptools import setup, find_packages, Extension
from distutils import sysconfig 
from info import __version__, __name__
import os, re



# Delete unwanted flags for C compilation
# find a better way to do that is necessary
d = sysconfig.get_config_vars()
for k, v in d.items():
    if str(v).find('-Wstrict-prototypes') != -1:
        v = d[k] = str(v).replace('-Wstrict-prototypes', '')
    if str(v).find('-DNDEBUG') != -1:
        v = d[k] = str(v).replace('-DNDEBUG', '')



#########################
# EXTENSIONS            #
#########################
# SWIG COMPILATION
swig_extension = Extension(
    'delaunator.libdelaunator_src._libdelaunator',
    sources = [('delaunator/libdelaunator_src/'+_) for _ in os.listdir('delaunator/libdelaunator_src/') if os.path.splitext(_)[1] == '.cpp'] + 
                ['delaunator/libdelaunator_src/libdelaunator.i'],
    swig_opts = ['-c++'],
    define_macros = [
        ('DEBUG', 1),
        # choose your finder mode here !
        ('DEULAUNAY_FINDER_INITIAL_RANDOM', None),
        #('DEULAUNAY_FINDER_INITIAL_FIRST', None),
        #('DEULAUNAY_FINDER_INITIAL_MIDDLE', None),
    ],
    optional = False,
    extra_compile_args = [
        '-std=c++11',
    ],

)



#########################
# SETUP                 #
#########################
setup(
    name = __name__,
    version = __version__,
    ext_modules = [swig_extension],
    py_modules = ['delaunator', 'delaunator.libdelaunator_src', 'delaunator.libdelaunator_src._libdelaunator'],
    packages = find_packages(exclude=['pydelaunator/', 't/']), 
    include_package_data = True,

    author = "aluriak",
    author_email = "lucas.bourneuf@laposte.net",
    description = "2D Delaunay Triangulation in C++ with Python wrapper",
    long_description = open('README.md').read(),
    keywords = "triangulation geometry delaunay wrapper",
    url = "https://github.com/Aluriak/Delaunator",

    classifiers = [
        "License :: OSI Approved :: GNU General Public License (GPL)",
        "Natural Language :: English",
        "Operating System :: Unix",
        "Programming Language :: C",
        "Programming Language :: C++",
        "Programming Language :: Python :: 3",
        "Programming Language :: C#",
        "Programming Language :: Java",
        "Programming Language :: OCaml",
        "Programming Language :: Other",
        "Programming Language :: Perl",
        "Programming Language :: PHP",
        "Programming Language :: Python",
        "Programming Language :: Tcl",
        "Topic :: Scientific/Engineering",
        "Topic :: Software Development :: Libraries :: Python Modules"
    ]
)



