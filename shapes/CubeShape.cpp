#include "CubeShape.h"

CubeShape::CubeShape()
{
    tot = 100 * 100 * 6 * 6 * 6;
    for(int i = 0; i < tot; i++) {
        m_vertexData.push_back(0.0f);
    }
    make_data(1, 1);
}

CubeShape::CubeShape(int par1, int par2){
    par1 = std::max(par1, 1);
    par1 = std::min(100, par1);
    tot = par1 * par1 * 6 * 6 * 6;
    m_vertexData.reserve(tot);
    for(int i = 0; i < tot; i++) {
        m_vertexData.push_back(0.0f);
    }
    make_data(par1, par2);
}

CubeShape::~CubeShape() {

}

void CubeShape::make_data(int par1, int par2) {
    par1 = std::max(par1, 1);
    par1 = std::min(100, par1);
    tot = par1 * par1 * 6 * 6 * 6;
    //int times = tot - m_vertexData.size();

    std::vector<std::vector<int>> normals {
        { 1,  0,  0},
        {-1,  0,  0},
        { 0,  1,  0},
        { 0, -1,  0},
        { 0,  0,  1},
        { 0,  0, -1},
    };

    std::vector<std::vector<int>> steps {
        {0, 0},
        {1, 1},
        {0, 1},
        {1, 1},
        {0, 0},
        {1, 0},
    };

    for(int k = 0; k < 6; k++) {
        int a = -1;
        int b = -1;
        int c = -1;
        for(int i = 0; i < 3; i++) {
            if (normals[k][i] != 0) {
                c = i;
                if(c == 0) {
                    if(normals[k][i] == 1) {
                        a = 1;
                        b = 2;
                    } else {
                        a = 2;
                        b = 1;
                    }
                } else if(c == 1) {
                    if(normals[k][i] == 1) {
                        a = 2;
                        b = 0;
                    } else {
                        a = 0;
                        b = 2;
                    }
                } else {
                    if(normals[k][i] == 1) {
                        a = 0;
                        b = 1;
                    } else {
                        a = 1;
                        b = 0;
                    }
                }
                break;
            }
        }
        for(int i = 0; i < par1; i++){
            float cord1base = -0.5 + 1.0 / par1 * (float)i;
            for(int j = 0; j < par1; j++) {
                int base = par1 * par1 * k * 6 * 6 + i * par1 * 6 * 6 + j * 6 * 6;
                float cord2base = -0.5 + 1.0 / par1 * (float)j;
                float step = 1.0 / par1;
                for(int t = 0; t < 6; t++) {
                    int astep = steps[t][0];
                    int bstep = steps[t][1];
                    m_vertexData[base + a + t * 6] = cord1base + (float)astep * step;
                    m_vertexData[base + b + t * 6] = cord2base + (float)bstep * step;
                    m_vertexData[base + c + t * 6] = (float)normals[k][c] * 0.5;
                    m_vertexData[base + a + 3 + t * 6] = 0.0f;
                    m_vertexData[base + b + 3 + t * 6] = 0.0f;
                    m_vertexData[base + c + 3 + t * 6] = (float)normals[k][c];
                }
            }
        }
    }

    buildVAO();
}
