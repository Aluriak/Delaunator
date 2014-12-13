#include "meshGenerator.h"





/******************************************************************
 * FROM DENSITY FUNCTION
 ******************************************************************/
/**
 * Creat a Delaunator instance with many points, placed between given bounds at given step from each other, 
 * according to given density function. The density function must return a value in [0;1]
 * @param fDensity a function pointer for a float, float -> float function
 * @param bound_min the minimal coordinate in all dimension
 * @param bound_max the maximal coordinate in all dimension
 * @param step the minimal distance between two points
 * @return address of a new and initialized Delaunator object
 */
Delaunator* MeshGenerator::fromDensityFunction(float (*fDensity)(float, float, void*), 
                unsigned int bound_minx, unsigned int bound_maxx, 
                unsigned int bound_miny, unsigned int bound_maxy, float step, void* data) {
// INIT
        Delaunator* dt = new Delaunator(bound_minx, bound_maxx, bound_miny, bound_maxy);
        float ci = bound_minx, cj = bound_miny;
// TREATMENT
        for(; ci <= bound_maxx; ci += step) {
                for(cj = 0; cj <= bound_maxy; cj += step) {
                        // get random number in [0;1]
                        if(((float)randN(101)) <= (fDensity(ci, cj, data) * 100)) {
                                dt->addVirtualVertex(Coordinates(ci, cj));
                        }
                }
        }
// END
        return dt;
}





