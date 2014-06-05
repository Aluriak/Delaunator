# -*- coding: utf-8 -*-
#########################
#       SETUP.PY        #
#########################


#########################
# IMPORTS               #
#########################
from setuptools import setup, find_packages, Extension, distutils
from info import __version__, __name__
import os, re



# Delete unwanted flags for C compilation
d = distutils.sysconfig.get_config_vars()
for k, v in d.items():
    if str(v).find('-Wstrict-prototypes') != -1:
        #v = distutils.sysconfig.get_config_vars()[k] = str(v).replace('-Wstrict-prototypes', '')
        v = d[k] = str(v).replace('-Wstrict-prototypes', '')
    if str(v).find('-DNDEBUG') != -1:
        #v = distutils.sysconfig.get_config_vars()[k] = str(v).replace('-DNDEBUG', '')
        v = d[k] = str(v).replace('-DNDEBUG', '')



#########################
# EXTENSIONS            #
#########################
# MACROS IN C CODE
defines = [
    ('DEBUG', 1),
    ('DEULAUNAY_FINDER_INITIAL_RANDOM', None),
    #('DEULAUNAY_FINDER_INITIAL_FIRST', None),
    #('DEULAUNAY_FINDER_INITIAL_MIDDLE', None),
]
# SWIG COMPILATION
swig_extension = Extension(
    'delaunator.libdelaunator_src._libdelaunator',
    sources = [('delaunator/libdelaunator_src/'+_) for _ in os.listdir('delaunator/libdelaunator_src/') if os.path.splitext(_)[1] == '.cpp'] + 
                ['delaunator/libdelaunator_src/libdelaunator.i'],
    #swig_opts = ['-modern', '-c++'],
    swig_opts = ['-c++'],
    define_macros = defines,
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
        "Programming Language :: Allegro CL",
        "Programming Language :: C#",
        "Programming Language :: CFFI",
        "Programming Language :: CLISP",
        "Programming Language :: Chicken",
        "Programming Language :: D",
        "Programming Language :: Go",
        "Programming Language :: Guile",
        "Programming Language :: Java",
        "Programming Language :: Lua",
        "Programming Language :: Modula-3",
        "Programming Language :: Mzscheme",
        "Programming Language :: OCAML",
        "Programming Language :: Octave",
        "Programming Language :: Perl",
        "Programming Language :: PHP",
        "Programming Language :: Python",
        "Programming Language :: R",
        "Programming Language :: Ruby",
        "Programming Language :: Tcl",
        "Programming Language :: UFFI",
        "Topic :: Scientific/Engineering",
        "Topic :: Software Development :: Libraries :: Python Modules"
    ]
)



