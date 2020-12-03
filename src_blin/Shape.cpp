#include "Shape.h"
#include "datatype/VAO.h"
#include "datatype/VBO.h"
#include "datatype/VBOAttribMarker.h"
#include "shaders/ShaderAttribLocations.h"

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
    const int numVertices = m_vertexData.size() / numFloatsPerVertex;

    std::vector<VBOAttribMarker> markers;
    markers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
    markers.push_back(VBOAttribMarker(ShaderAttrib::NORMAL, 3, 3*sizeof(float)));
    VBO vbo = VBO(m_vertexData.data(), m_vertexData.size(), markers);
    m_VAO = std::make_unique<VAO>(vbo, numVertices);
}

void Shape::update_m_vtx(float x, float y, float z, glm::vec3 normal){
    m_vertexData.push_back(x);
    m_vertexData.push_back(y);
    m_vertexData.push_back(z);
    m_vertexData.push_back(normal[0]);
    m_vertexData.push_back(normal[1]);
    m_vertexData.push_back(normal[2]);
}

void Shape::update_m_vtx(float x, float y, float z, std::vector<float> normal){
    m_vertexData.push_back(x);
    m_vertexData.push_back(y);
    m_vertexData.push_back(z);
    m_vertexData.insert(m_vertexData.end(), normal.begin(), normal.end());
}

glm::vec3 Shape::crossProduct(glm::vec3 v_A, glm::vec3 v_B)
{

   float token_0 = v_A[1] * v_B[2] - v_A[2] * v_B[1];
   float token_1 = -(v_A[0] * v_B[2] - v_A[2] * v_B[0]);
   float token_2 = v_A[0] * v_B[1] - v_A[1] * v_B[0];
   glm::vec3 v3 = {token_0, token_1, token_2};
   return v3;
}
