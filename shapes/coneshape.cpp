#include <iostream>

#include "ConeShape.h"
#include "math.h"

ConeShape::ConeShape()
{
    tot = 716400;
    for(int i = 0; i < tot; i ++) {
        m_vertexData.push_back(0.0f);
    }
    make_data(1, 3);
}

ConeShape::ConeShape(int par1, int par2){
    par1 = std::max(1, par1);
    par1 = std::min(100, par1);

    par2 = std::max(3, par2);
    par2 = std::min(100, par2);
    tot = (1 + (par1 - 1) * 2) * par2 * 6 * 3 * 2;
    for(int i = 0; i < tot; i ++) {
        m_vertexData.push_back(0.0f);
    }
    make_data(par1, par2);
}


ConeShape::~ConeShape(){

}

void ConeShape::make_data(int par1, int par2){
    par1 = std::max(1, par1);
    par1 = std::min(100, par1);

    par2 = std::max(3, par2);
    par2 = std::min(100, par2);
    center_tri(0, 0.5f, -0.5f, acos(1.0f / sqrt(5)), par1, par2, 1);
    center_tri((1 + (par1 - 1) * 2) * par2 * 6 * 3, -0.5f, -0.5f, M_PI, par1, par2, -1);
    tot = (1 + (par1 - 1) * 2) * par2 * 6 * 3 * 2;

    buildVAO();
}
