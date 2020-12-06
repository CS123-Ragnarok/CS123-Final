#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"
class Cylinder : public Shape
{
public :
    Cylinder();
    Cylinder(int p1, int p2);
    ~Cylinder();
private:
    void create_vertex_data();
    void create_cycle(float y_axis, bool face_top);
    int mesh_size;
    int side_size;
};


#endif
