#include "main.h"




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
 * MAIN
 ***************************************************/
int main(int argc, char* argv[]) {
#if DEBUG
    std::cerr << "[[[[ DEBUG MODE ]]]]" << std::endl;
#else
    std::cerr << "[[[[ FREE  MODE ]]]]" << std::endl;
#endif
    Delaunator* dt = new Delaunator(0,600,0,600);
    
    Point* vv1 = new Point(1);
    Point* vv2 = new Point(2);
    Point* vv3 = new Point(3);
    Point* vv4 = new Point(4);
    Point* vv5 = new Point(5);
    assert(dt->addVirtualVertex(100, 100, vv1) != NULL);
    assert(dt->addVirtualVertex(200, 100, vv2) != NULL);
    assert(dt->addVirtualVertex(150, 200, vv3) != NULL);
    assert(dt->addVirtualVertex(150,   0, vv4) != NULL);
    assert(dt->addVirtualVertex(150, 150, vv5) != NULL);


    // Moving
    dt->movVirtualVertex(vv1, 180, 180);
    dt->movVirtualVertex(vv2, -180, 10);
    // Deleting
    dt->delVirtualVertex(vv1);
    dt->delVirtualVertex(vv2);
    delete vv1;
    delete vv2;

    // Reinitializing
    assert(dt->addVirtualVertex(100, 100, vv1 = new Point(1)) != NULL);
    assert(dt->addVirtualVertex(200, 100, vv2 = new Point(2)) != NULL);

    // Moving (limit case: pass by other point)
    dt->movVirtualVertex(vv5, -150, 0);
    dt->movVirtualVertex(vv5,  300, 0);
    dt->movVirtualVertex(vv5, -150, 0);

    // Deleting (limit case: point aligned with neighbors)
    dt->delVirtualVertex(vv5);
    delete vv5;
    assert(dt->addVirtualVertex(150, 150, vv5 = new Point(5)) != NULL);


    delete dt;
    delete vv1;
    delete vv2;
    delete vv3;
    delete vv4;
    delete vv5;
    return EXIT_SUCCESS;
}




