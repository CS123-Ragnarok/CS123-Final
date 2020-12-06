#include "Shape.h"
#include "gl/datatype/VAO.h"
#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include "math.h"


using namespace CS123::GL;

Shape::Shape() :
    m_VAO(nullptr)
{

}

Shape::~Shape()
{
}

void Shape::draw() {
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}

void Shape::buildVAO() {
    const int numFloatsPerVertex = 6;
    const int numVertices = tot / numFloatsPerVertex;

    std::vector<VBOAttribMarker> markers;
    markers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
    markers.push_back(VBOAttribMarker(ShaderAttrib::NORMAL, 3, 3*sizeof(float)));
    VBO vbo = VBO(m_vertexData.data(), tot, markers);
    m_VAO = std::make_unique<VAO>(vbo, numVertices);
}

void Shape::center_tri(int base, float y, float basey, float phi, int par1, int par2, int control) {
    std::vector<float> tip = {0.0f, y, 0.0f};
    float angleInc = 2.0 * M_PI / (float) par2;
    int tempbase = base;

    std::vector<std::vector<int>> step = {
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 1, 1},
        {0, 0, 1, 0},
    };
    std::vector<std::vector<int>> normalstep = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
    };

    for(int i = 0; i < par2; i++) {
        float ang = angleInc * i;
        float lang = angleInc * i + (float)(control) * angleInc / 2.0f;
        float rang = angleInc * i - (float)(control) * angleInc / 2.0f;
        std::vector<float> normal = {sin(phi) * cos(ang), cos(phi), sin(phi) * sin(ang)};
        std::vector<float> left = {0.5f * cos(lang), basey, 0.5f * sin(lang)};
        std::vector<float> right = {0.5f * cos(rang), basey, 0.5f * sin(rang)};
        std::vector<float> lseg = {(left[0] - tip[0]) / (float) par1, (left[1] - tip[1]) / (float) par1, (left[2] - tip[2]) / (float) par1};
        std::vector<float> rseg = {(right[0] - tip[0]) / (float) par1, (right[1] - tip[1]) / (float) par1, (right[2] - tip[2]) / (float) par1};
        base = i * (1 + (par1 - 1) * 2) * 18 + tempbase;
        std::vector<float> leftnormal = {sin(phi) * cos(lang), cos(phi), sin(phi) * sin(lang)};
        std::vector<float> rightnormal = {sin(phi) * cos(rang), cos(phi), sin(phi) * sin(rang)};
        m_vertexData[base + 0] = tip[0];
        m_vertexData[base + 1] = tip[1];
        m_vertexData[base + 2] = tip[2];
        m_vertexData[base + 6] = tip[0] + lseg[0];
        m_vertexData[base + 7] = tip[1] + lseg[1];
        m_vertexData[base + 8] = tip[2] + lseg[2];
        m_vertexData[base + 12] = tip[0] + rseg[0];
        m_vertexData[base + 13] = tip[1] + rseg[1];
        m_vertexData[base + 14] = tip[2] + rseg[2];
        for(int j = 0; j < 3; j++){
            m_vertexData[base + 3 + j * 6 + 0] = normal[0] * (float) normalstep[j][0] + leftnormal[0] * (float) normalstep[j][1] + rightnormal[0] * (float) normalstep[j][2];
            m_vertexData[base + 3 + j * 6 + 1] = normal[1] * (float) normalstep[j][0] + leftnormal[1] * (float) normalstep[j][1] + rightnormal[1] * (float) normalstep[j][2];
            m_vertexData[base + 3 + j * 6 + 2] = normal[2] * (float) normalstep[j][0] + leftnormal[2] * (float) normalstep[j][1] + rightnormal[2] * (float) normalstep[j][2];
        }
        for(int j = 1; j < par1; j++) {
            for(int k = 0; k < 6; k++) {
                m_vertexData[base + j * 36 - 18 + k * 6 + 0] = tip[0] + lseg[0] * j * step[k][0] + lseg[0] * step[k][1] + rseg[0] * j * step[k][2] + rseg[0] * step[k][3];
                m_vertexData[base + j * 36 - 18 + k * 6 + 1] = tip[1] + lseg[1] * j * step[k][0] + lseg[1] * step[k][1] + rseg[1] * j * step[k][2] + rseg[1] * step[k][3];
                m_vertexData[base + j * 36 - 18 + k * 6 + 2] = tip[2] + lseg[2] * j * step[k][0] + lseg[2] * step[k][1] + rseg[2] * j * step[k][2] + rseg[2] * step[k][3];
                m_vertexData[base + j * 36 - 18 + k * 6 + 3] = leftnormal[0] * (float) step[k][0] + rightnormal[0] * (float) step[k][2];
                m_vertexData[base + j * 36 - 18 + k * 6 + 4] = leftnormal[1] * (float) step[k][0] + rightnormal[1] * (float) step[k][2];
                m_vertexData[base + j * 36 - 18 + k * 6 + 5] = leftnormal[2] * (float) step[k][0] + rightnormal[2] * (float) step[k][2];
            }
        }
    }
}
