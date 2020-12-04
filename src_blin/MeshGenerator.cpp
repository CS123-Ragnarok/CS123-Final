#include "MeshGenerator.h"
#include "datatype/VAO.h"
#include "datatype/VBO.h"
#include "datatype/VBOAttribMarker.h"
#include "shaders/ShaderAttribLocations.h"


using namespace CS123::GL;

MeshGenerator::MeshGenerator(){

}
MeshGenerator::~MeshGenerator(){

}


void MeshGenerator::draw(){
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}

void MeshGenerator::buildVAO(){
    const int numFloatsPerVertex = 6;
    const int numVertices = m_vertexData.size() / numFloatsPerVertex;

    std::vector<VBOAttribMarker> markers;
    markers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
    markers.push_back(VBOAttribMarker(ShaderAttrib::NORMAL, 3, 3*sizeof(float)));
    VBO vbo = VBO(m_vertexData.data(), m_vertexData.size(), markers);
    m_VAO = std::make_unique<VAO>(vbo, numVertices);
}

glm::vec3 MeshGenerator::crossProduct(glm::vec3 v_A, glm::vec3 v_B)
{

   float token_0 = v_A[1] * v_B[2] - v_A[2] * v_B[1];
   float token_1 = -(v_A[0] * v_B[2] - v_A[2] * v_B[0]);
   float token_2 = v_A[0] * v_B[1] - v_A[1] * v_B[0];
   glm::vec3 v3 = {token_0, token_1, token_2};
   return v3;
}


void DOL::interpretation()
{
    glm::vec3 pos(0,0,0);
    m_primitive.clear();
    m_transform.clear();
    for(int i = 0; i < m_base.size(); i++)
    {
        char instru = m_base.at(i);
        switch(instru)
        {
        case 'f':
            pos += heading * dist;
            break;
        case 'F':
            Cylinder token(60, 60);
            m_shapes.push_back(token);
            glm::vec3 old_pos = pos;
            pos += heading * dist;
            glm::vec3 mid = (pos + old_pos)/2.0f;
            glm::mat4 translate = glm::translate(glm::vec3(mid));
            glm::mat4 rotate = RotationMatrix(heading, left, up);
            glm::mat4 scale = glm::scale(glm::vec3(1,1,dist));
            glm::mat4 move = rotate * translate * scale;
            m_transform.push_back(move);
            break;
        case '+':
             glm::mat3 R;
             R = rotate_up(m_rad);
             heading = heading * R;
             left = left * R;
             up = up * R;
             break;
        else
        {
            glm::mat3 R;
            if(instru == '+')
                R = rotate_up(m_rad);
            else if(instru == '-')
                 R = rotate_up(-m_rad);

            else if(instru == '&')
                 R = rotate_left(m_rad);
            else if(instru == '^')
                 R = rotate_left(-m_rad);

            else if(instru == '\\')
                 R = rotate_head(m_rad);
            else if(instru == '/')
                 R = rotate_head(-m_rad);

            else if(instru == '|')
                R = rotate_up(M_PI);

            heading = heading * R;
            left = left * R;
            up = up * R;
        }
        }

    }

}






