#include "SphereShape.h"

SphereShape::SphereShape()
{
    tot = 360000;
    for(int i = 0; i < tot; i++){
        m_vertexData.push_back(1.0f);
    }
    make_data(2, 3);
}

SphereShape::SphereShape(int par1, int par2){
    par1 = std::min(par1, 100);
    par1 = std::max(par1, 2);

    par2 = std::min(par2, 100);
    par2 = std::max(par2, 3);

    tot = 36 * (par1 - 1) * par2;
    m_vertexData.reserve(tot);
    for(int i = 0; i < tot; i++){
        m_vertexData.push_back(1.0f);
    }
    make_data(par1, par2);
}

SphereShape::~SphereShape(){

}

void SphereShape::make_data(int par1, int par2){
    par1 = std::min(par1, 100);
    par1 = std::max(par1, 2);

    par2 = std::min(par2, 100);
    par2 = std::max(par2, 3);

    tot = 36 * (par1 - 1) * par2;

    std::vector<std::vector<int>> step = {
        {1, 0},
        {0, 1},
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1},
    };

    float angleInc = 2.0 * M_PI / (float) par2;
    float phiInc = M_PI / (float) par1;

    for(int i = 0; i < par2; i++) {
        std::vector<std::vector<float>> top = {
            {0.0f, 1.0f, 0.0f},
            {sin(phiInc) * cos(angleInc * (i + 1)), cos(phiInc), sin(phiInc) * sin(angleInc * (i + 1))},
            {sin(phiInc) * cos(angleInc * i), cos(phiInc), sin(phiInc) * sin(angleInc * i)}
        };
        std::vector<std::vector<float>> down = {
            {0.0f, -1.0f, 0.0f},
            {sin((float)M_PI - phiInc) * cos(angleInc * i), cos((float)M_PI - phiInc), sin((float)M_PI - phiInc) * sin(angleInc * i)},
            {sin((float)M_PI - phiInc) * cos(angleInc * (i + 1)), cos((float)M_PI - phiInc), sin((float)M_PI - phiInc) * sin(angleInc * (i + 1))}
        };
        for(int r = 0; r < 3; r++) {
            for(int c = 0; c < 3; c++){
                m_vertexData[i * (par1 - 1) * 36 + r * 2 * 3 + c] = 0.5f * top[r][c];
                m_vertexData[i * (par1 - 1) * 36 + r * 2 * 3 + c + 3] = top[r][c];
                m_vertexData[(i + 1) * (par1 - 1) * 36 - 18 + r * 2 * 3 + c] = 0.5f * down[r][c];
                m_vertexData[(i + 1) * (par1 - 1) * 36 - 18 + r * 2 * 3 + c + 3] = down[r][c];
            }
        }
        for(int j = 1; j < par1 - 1; j++) {
            int base = i * (par1 - 1) * 36 + j * 36 - 18;
            for(int k = 0; k < 6; k++) {
                float phi = phiInc * (step[k][1] + j);
                float ang = angleInc * (step[k][0] + i);
                std::vector<float> dir = {sin(phi) * cos(ang), cos(phi), sin(phi) * sin(ang)};
                m_vertexData[base + k * 6 + 0] = 0.5f * dir[0];
                m_vertexData[base + k * 6 + 1] = 0.5f * dir[1];
                m_vertexData[base + k * 6 + 2] = 0.5f * dir[2];
                m_vertexData[base + k * 6 + 3] = dir[0];
                m_vertexData[base + k * 6 + 4] = dir[1];
                m_vertexData[base + k * 6 + 5] = dir[2];
            }
        }

    }
    buildVAO();
}
