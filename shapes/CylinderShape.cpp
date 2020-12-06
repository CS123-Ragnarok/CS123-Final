#include "CylinderShape.h"
#include "math.h"

CylinderShape::CylinderShape()
{
    tot = 1076400;
    for(int i = 0; i < tot; i++) {
        m_vertexData.push_back(0.0f);
    }
    make_data(1, 3);
}

CylinderShape::CylinderShape(int par1, int par2){
    par1 = std::max(1, par1);
    par1 = std::min(100, par1);

    par2 = std::max(3, par2);
    par2 = std::min(100, par2);

    tot = (1 + (par1 - 1) * 2) * par2 * 6 * 3 * 2 + par1 * par2 * 36;
    for(int i = 0; i < tot; i ++) {
        m_vertexData.push_back(0.0f);
    }
    make_data(par1, par2);
}

CylinderShape::~CylinderShape(){

}

void CylinderShape::make_data(int par1, int par2) {
    par1 = std::max(1, par1);
    par1 = std::min(100, par1);

    par2 = std::max(3, par2);
    par2 = std::min(100, par2);
    center_tri(0, -0.5f, -0.5f, M_PI, par1, par2, -1);
    center_tri((1 + (par1 - 1) * 2) * par2 * 6 * 3, 0.5f, 0.5f, 0.0f, par1, par2, 1);

    std::vector<std::vector<int>> step = {
        {1, 0, 0},
        {1, 1, 0},
        {0, 0, 1},
        {1, 1, 0},
        {0, 1, 1},
        {0, 0, 1},
    };

    float angleInc = 2.0 * M_PI / (float) par2;

    int bigbase = (1 + (par1 - 1) * 2) * par2 * 6 * 3 * 2;

    for(int i = 0; i < par2; i++) {
        float ang = angleInc * i;
        float lang = angleInc * i + angleInc / 2.0f;
        float rang = angleInc * i - angleInc / 2.0f;
        std::vector<float> normal = {cos(ang), 0.0f, sin(ang)};
        float leftx = 0.5f * cos(lang);
        float rightx = 0.5f * cos(rang);
        float leftz = 0.5f * sin(lang);
        float rightz = 0.5f * sin(rang);
        float seg = 1.0 / (float) par1;
        int base = bigbase + i * par1 * 36;
        std::vector<float> leftnormal = {cos(lang), 0.0f, sin(lang)};
        std::vector<float> rightnormal = {cos(rang), 0.0f, sin(rang)};
        for(int j = 0; j < par1; j++){
            for(int k = 0; k < 6; k++) {
                m_vertexData[base + j * 36 + k * 6 + 0] = (float) step[k][0] * leftx + (float) step[k][2] * rightx;
                m_vertexData[base + j * 36 + k * 6 + 1] = 0.5 - seg * j - (float) step[k][1] * seg;
                m_vertexData[base + j * 36 + k * 6 + 2] = (float) step[k][0] * leftz + (float) step[k][2] * rightz;
                m_vertexData[base + j * 36 + k * 6 + 3] = leftnormal[0] * (float) step[k][0] + rightnormal[0] * (float) step[k][2];
                m_vertexData[base + j * 36 + k * 6 + 4] = leftnormal[1] * (float) step[k][0] + rightnormal[1] * (float) step[k][2];
                m_vertexData[base + j * 36 + k * 6 + 5] = leftnormal[2] * (float) step[k][0] + rightnormal[2] * (float) step[k][2];
            }
        }
    }

    tot = (1 + (par1 - 1) * 2) * par2 * 6 * 3 * 2 + par1 * par2 * 36;

    buildVAO();
}

