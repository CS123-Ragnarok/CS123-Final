#ifndef CYLINDERSHAPE_H
#define CYLINDERSHAPE_H

#include "Shape.h"

class CylinderShape : public Shape
{
public:
    CylinderShape();
    CylinderShape(int par1, int par2);
    virtual ~CylinderShape();

private:
    void make_data(int par1, int par2);
};

#endif // CYLINDERSHAPE_H
