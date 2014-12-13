#ifndef MESH_GENERATOR_H_INCLUDED
#define MESH_GENERATOR_H_INCLUDED



/*
 * LIBRARIES
 */
// LOCAL MODULES
#include "commons.h"
#include "delaunator.h"



/*
 * DEFINES
 */



/*
 * PREDECLARATIONS
 */






/**
 * Methods for automatic generation of Delaunator intances
 */
namespace MeshGenerator {
    Delaunator* fromDensityFunction(float (*)(float, float, void*), unsigned int, unsigned int, 
                                    unsigned int, unsigned int, float, void*);

}



#endif
