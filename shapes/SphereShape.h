#ifndef SPHERESHAPE_H
#define SPHERESHAPE_H

#include "math.h"
#include "Shape.h"

class SphereShape : public Shape
{
public:
    SphereShape();
    SphereShape(int par1, int par2);
    virtual ~SphereShape();

    void make_data(int par1, int par2);
};

#endif // SPHERESHAPE_H
