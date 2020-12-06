#ifndef CUBESHAPE_H
#define CUBESHAPE_H

#include "Shape.h"


class CubeShape : public Shape
{
public:
    CubeShape();
    CubeShape(int par1, int par2);
    virtual ~CubeShape();

    void make_data(int par1, int par2);

};

#endif // CUBESHAPE_H
