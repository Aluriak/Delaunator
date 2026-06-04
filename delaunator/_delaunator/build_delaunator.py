# build_delaunator.py
from cffi import FFI
ffibuilder = FFI()

ffibuilder.set_source(
    '_delaunator',
    '#include "triangulation.h"',
    sources=['triangulation.c', 'geometry.c', 'edge.c', 'vertex.c', 'face.c', 'test.c'],
    extra_compile_args=['-O2', '-flto'],
)
ffibuilder.cdef(open('triangulation.h').read())  # le vrai header, pas une copie

if __name__ == '__main__':
    ffibuilder.compile(verbose=True)
