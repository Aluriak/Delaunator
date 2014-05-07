#include "main.h"



/***************************************************
 * MAIN
 ***************************************************/
int main(int argc, char* argv[]) {
#if DEBUG
        std::cout << "[[[[ DEBUG MODE ]]]]" << std::endl;
#else
        std::cout << "[[[[ RELEASE MODE ]]]]" << std::endl;
#endif
        Delaunator* dt = new Delaunator(0,600,0,600);
#if DEBUG
        dt->DEBUG_tests();
        std::cout << "All tests succeed !" << std::endl;
#endif
        

        for(IteratorOnAllEdges it = dt->allEdges(); it != it.end(); it++) {
                std::cout << **it << std::endl;
        }
        for(IteratorOnAllFaces it = dt->allFaces(); it != it.end(); it++) {
                std::cout << **it << std::endl;
        }
        for(IteratorOnAllVertices it = dt->allVertices(); it != it.end(); it++) {
                std::cout << **it << std::endl;
        }

        delete dt;
        return EXIT_SUCCESS;
}




