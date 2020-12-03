#ifndef CONESHAPE_H
#define CONESHAPE_H

#include "Shape.h"

class ConeShape : public Shape
{
public:
    ConeShape();
    ConeShape(int par1, int par2);
    virtual ~ConeShape();

    void make_data(int par1, int par2);
};

#endif // CONESHAPE_H
