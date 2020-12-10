#include "particle.h"
#include "gl/datatype/VAO.h"
#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr


using namespace CS123::GL;


void Particle::buildVAO()
{

    const int numFloatsPerVertex = 2;
    const int numVertices = m_vertexData.size() / numFloatsPerVertex;

    std::vector<VBOAttribMarker> markers;
    markers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
    VBO vbo = VBO(m_vertexData.data(), m_vertexData.size(), markers);
    m_VAO = std::make_unique<VAO>(vbo, numVertices);
    glDepthMask(false);

}

bool Particle::update(){
    velocity.y += GRAVITY * gravityEffect * dt;
    pos += velocity * (float)dt;
    elapsedTime += dt;
    return elapsedTime < lifeLength;
}


void Particle::draw(){
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}

glm::mat4x4 Particle::updateModelViewMatrix(float rotation, float scale, glm::mat4x4 viewMatrix)
{
    glm::mat4x4 modelMatrix = glm::translate(pos);
    modelMatrix[0][0] = viewMatrix[0][0];
    modelMatrix[0][1] = viewMatrix[1][0];
    modelMatrix[0][2] = viewMatrix[2][0];
    modelMatrix[1][0] = viewMatrix[0][1];
    modelMatrix[2][0] = viewMatrix[0][2];
    modelMatrix[3][0] = viewMatrix[0][3];
    float rotation_radian = rotation * M_PI / 180.0f;
    modelMatrix = glm::rotate(rotation_radian, glm::vec3(0,0,1)) * modelMatrix;
    modelMatrix *= scale;



}
/*

void Particle::add_triangle_face(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
    glm::vec3 v1 = p1-p3;
    glm::vec3 v2 = p2-p3;
    glm::vec3 normal = glm::normalize(glm::cross(v1,v2));
    add_single_vertex(p1, normal);
    add_single_vertex(p2, normal);
    add_single_vertex(p3, normal);

}
void Particle::add_single_vertex(glm::vec3 point, glm::vec3 normal){
    m_vertexData.push_back(point.x);
    m_vertexData.push_back(point.y);
    m_vertexData.push_back(point.z);
    m_vertexData.push_back(normal[0]);
    m_vertexData.push_back(normal[1]);
    m_vertexData.push_back(normal[2]);

}
*/
