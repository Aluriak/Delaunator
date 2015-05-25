#include "main.h"




/***************************************************
 * POINT CLASS
 ***************************************************/
class Point : public VirtualVertex {
        public:
        Point(int v) : value(v) { idpoint = ++classid; }

        bool operator==(const Point& othr) const { return this->idpoint == othr.idpoint; }
        bool haveSameValue(const Point& othr) const { return this->value == othr.value; }


        private:
        int value;
        int idpoint;
        static unsigned int classid;

};

unsigned int Point::classid = 0;





/***************************************************
 * CALLBACK
 ***************************************************/
float density(float x, float y, void* data) {
        return (x * y) / (((Coordinates*)data)->x() * ((Coordinates*)data)->y());
}



bool pointInCircle_methd1(Coordinates p1, Coordinates p2, Coordinates p3, Coordinates p0) {
        /* CURRENT IMPLEMENTATION */
        return geometry::pointInCircumcircleOf(p1, p2, p3, p0);
}



bool pointInCircle_methd2(Coordinates p1, Coordinates p2, Coordinates p3, Coordinates p0) {
        /* TESTED IMPLEMENTATION */
        // source here: http://mathworld.wolfram.com/Circumradius.html
        float a = p1.distanceTo(p2);
        float b = p2.distanceTo(p3);
        float c = p3.distanceTo(p1);
        // direct acces to coords, those are offseted with A at 0
        float Ax = 0, Ay = 0, Bx = (p2 - p1).x(), By = (p2 - p1).y(), Cx = (p3 - p1).x(), Cy = (p3 - p1).y();
        float D = 2 * (Bx*Cy - By*Cx);
        // get radius & center
        float crcm_radius = (a*b*c)  /  sqrt( (a+b+c) * (b + c -a) * (c + a -b) * (a + b - c) );
        Coordinates circumcenter(
                (Cy*(Bx*Bx + By*By) - By*(Cx*Cx + Cy*Cy)) / D,
                (Bx*(Cx*Cx + Cy*Cy) - Cx*(Bx*Bx + By*By)) / D
        );

        // simple distance test
        return p0.squareDistanceTo(circumcenter) < (crcm_radius*crcm_radius);
}


void vvverif(VirtualVertex* vv) { // TO DELETE
        Edge* cur_edge = vv->vertex()->getEdge();
        Edge* ref_edge = vv->vertex()->getEdge();
        logs("%u: ", vv->id());
        do {
                logs("%u  ", ref_edge->getID());
                ref_edge = ref_edge->rotLeftEdge();
        } while(cur_edge != ref_edge);
        logs("%u\n", ref_edge->getID());
}




float summerNonVariadic(const std::vector<double> values) {
        float sum = 0.;
        for(unsigned int summed = 0; summed < values.size(); summed++) {
                sum += values[summed];
        }
        return sum;
}




float summerVariadic(const unsigned int n, va_list args) {
        std::vector<double> values;

        for(unsigned int j = 0; j < n; j++) {
                values.push_back(va_arg(args, double));
        }

        return summerNonVariadic(values);
}




bool wrapperVariadic(const unsigned int n, ...) {
        va_list args;
        va_start(args, n);
        float summer_said = summerVariadic(n, args);
        va_end(args);
        return summer_said > 0;
}



#define LOGOK fprintf(stderr,"OK:%s:%u\n", __FILE__, __LINE__);
/***************************************************
 * MAIN
 ***************************************************/
int main(int argc, char* argv[]) {
#if DEBUG
        std::cerr << "[[[[ DEBUG MODE ]]]]" << std::endl;
#else
        std::cerr << "[[[[ FREE  MODE ]]]]" << std::endl;
#endif
        LOGOK
        Delaunator* dt = new Delaunator(0, 600, 0, 600);
        Point* vv1 = new Point(1);
        Point* vv2 = new Point(2);
        Point* vv3 = new Point(3);
        Point* vv4 = new Point(4);
        Point* vv5 = new Point(5);
        LOGOK
        assert(dt->addVirtualVertex(100,   0, vv1) != NULL);
        vvverif(vv1);
        LOGOK
        dt->unittests();
        LOGOK
        assert(dt->addVirtualVertex(  0, 100, vv2) != NULL);
        vvverif(vv2);
        LOGOK
        dt->unittests();
        LOGOK
        assert(dt->addVirtualVertex(100, 300, vv3) != NULL);
        LOGOK
        dt->unittests();
        LOGOK
        assert(dt->addVirtualVertex(200, 100, vv4) != NULL);
        LOGOK
        dt->representation();
        assert(dt->addVirtualVertex(100, 100, vv5) != NULL);
        LOGOK
        assert(dt->addVirtualVertex(300, 400) != NULL);
        LOGOK
        assert(dt->addVirtualVertex(300, 500) != NULL);
        LOGOK
        assert(dt->addVirtualVertex(400, 200) != NULL);
        LOGOK
        //assert(dt->addVirtualVertex(100, 500) != NULL);
        //LOGOK
        //assert(dt->addVirtualVertex(600, 500) != NULL);
        //LOGOK
        //assert(dt->addVirtualVertex(600, 300) != NULL);
        //LOGOK
        //assert(dt->addVirtualVertex(600, 100) != NULL);
        //LOGOK


        // Moving
        LOGOK
        dt->movVirtualVertex(vv1, 180, 180);
        LOGOK
        dt->movVirtualVertex(vv2, -180, 10);
        LOGOK
        // Deleting
        dt->delVirtualVertex(vv1);
        dt->delVirtualVertex(vv2);
        delete vv1;
        delete vv2;
        LOGOK

        // Reinitializing
        assert(dt->addVirtualVertex(100,   0, vv1 = new Point(1)) != NULL);
        assert(dt->addVirtualVertex(  0, 100, vv2 = new Point(2)) != NULL);
        LOGOK

        // Moving (limit case: pass by other point)
        dt->movVirtualVertex(vv5,  200, 0);
        dt->movVirtualVertex(vv5, -200, 0);
        assert(vv5->coordinates() == Coordinates(100, 100));
        LOGOK

        // Moving (limit case: arrive on a point)
        LOGOK
        dt->movVirtualVertex(vv5, 0, -100);
        LOGOK
        assert(vv5->coordinates() == vv1->coordinates());
        LOGOK

        // Deleting (limit case: point aligned with neighbors)
        dt->delVirtualVertex(vv5);
        delete vv5;
        assert(dt->addVirtualVertex(100, 100, vv5 = new Point(5)) != NULL);
        LOGOK


        delete dt;
        delete vv1;
        delete vv2;
        delete vv3;
        delete vv4;
        delete vv5;
        LOGOK



        // Ownership and memory management
        Coordinates size(25, 20);
        dt = MeshGenerator::fromDensityFunction(density, 0, size.x(), 0, size.y(), 4, (void*)&size);
        LOGOK

        for(int i = 0; i < 10; i++) {
                unsigned int counter = 0;
                for(auto vv : dt->virtualVertices())
                        counter++;
                assert(counter == 12);
        }
        LOGOK

        std::vector<VirtualVertex*> tmp_container;
        for(auto vv : dt->virtualVertices()) {
                tmp_container.push_back(vv);
        }
        dt->clear();
        LOGOK
        for(auto vv : tmp_container) {
                dt->addVirtualVertex(100, 100, vv);
        }

        dt->freeAll();
        delete dt;
        LOGOK


        // Usecase (limit case, but i don't know why)
        dt = new Delaunator(0, 1000, 0, 1000);
        vv1 = new Point(1);
        vv2 = new Point(2);
        vv3 = new Point(3);
        vv4 = new Point(4);
        assert(dt->addVirtualVertex(264, 306, vv1) != NULL);
        assert(dt->addVirtualVertex(221, 249, vv2) != NULL);
        assert(dt->addVirtualVertex(258, 305, vv3) != NULL);
        assert(dt->addVirtualVertex(317, 266, vv4) != NULL);
        //assert(dt->addVirtualVertex(318, 266, vv4) != NULL);
        // other case where error appeared:
        //assert(dt->addVirtualVertex(351,  38, vv1) != NULL);
        //assert(dt->addVirtualVertex(411,  97, vv2) != NULL);
        //assert(dt->addVirtualVertex(411,  18, vv3) != NULL);
        //assert(dt->addVirtualVertex(348,  69, vv4) != NULL);


        LOGOK
        Coordinates A = vv1->coordinates(), B = vv2->coordinates(), C = vv3->coordinates();
        Coordinates D = Coordinates(317, 266);
        //dt->movVirtualVertex(vv4, -1, 0);

        dt->freeAll();
        LOGOK
        delete dt;
        LOGOK





        // TESTS
        //std::cout << "limit case 1, method 1: " << pointInCircle_methd1(
                        //Coordinates(0, 0), Coordinates(100, 0), Coordinates(200, 0),
                        //Coordinates(10, 10)
        //) << std::endl;
        //std::cout << "limit case 1, method 2: " << pointInCircle_methd2(
                        //Coordinates(0, 0), Coordinates(100, 0), Coordinates(200, 0),
                        //Coordinates(10, 10)
        //) << std::endl;
        //bool methd2 = pointInCircle_methd2(A, B, C, D);
        //int nb_run_benchmark = 100000, nb_run_stats = 10000;
        //int max_size = 10, nb_conflict = 0, nb_methd1_false = 0;

        //for(int i = 0; i < nb_run_stats; i++) {
                //Coordinates A(randN(max_size), randN(max_size));
                //Coordinates B(randN(max_size), randN(max_size));
                //Coordinates C(randN(max_size), randN(max_size));
                //Coordinates D(randN(max_size), randN(max_size));
                //bool methd1 = pointInCircle_methd1(A, B, C, D);
                //bool methd2 = pointInCircle_methd2(A, B, C, D);
                //if(methd1 != methd2) {
                        //nb_conflict++;
                        //if(methd1 == false) nb_methd1_false++;
                        //std::cerr << methd1 << A << B << C << D << std::endl;
                //}
        //}
        //std::cerr << "Conflicts: " << nb_conflict << "(" << nb_methd1_false << " where method 1 said false)" << std::endl;
        //LOGOK


        // BENCHMARK
        //time_t t;
        //std::cout << "BENCHMARKS\n";

        //t = time(NULL);
        //for(int i = 0; i < nb_run_benchmark; i++) {
                //Coordinates A(randN(max_size), randN(max_size));
                //Coordinates B(randN(max_size), randN(max_size));
                //Coordinates C(randN(max_size), randN(max_size));
                //Coordinates D(randN(max_size), randN(max_size));
                //pointInCircle_methd1(A, B, C, D);
        //}
        //t = time(NULL) - t;
        //std::cout << "\tMethod 1: " << t << std::endl;
        //t = time(NULL);
        //for(int i = 0; i < nb_run_benchmark; i++) {
                //Coordinates A(randN(max_size), randN(max_size));
                //Coordinates B(randN(max_size), randN(max_size));
                //Coordinates C(randN(max_size), randN(max_size));
                //Coordinates D(randN(max_size), randN(max_size));
                //pointInCircle_methd2(A, B, C, D);
        //}
        //t = time(NULL) - t;
        //std::cout << "\tMethod 2: " << t << std::endl;

        return EXIT_SUCCESS;
}




