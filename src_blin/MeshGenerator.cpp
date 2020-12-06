#include "MeshGenerator.h"
#include "gl/datatype/VAO.h"
#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include <iostream>

using namespace CS123::GL;

MeshGenerator::MeshGenerator():
    m_VAO(nullptr)
{
    m_lsystem = std::make_unique<LSystem>();
    m_lsystem->add_rules('F', "F[Fz[zFZXFZYF]Z[ZFxzFyzF]C+]");

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


void MeshGenerator::GenerateMesh(std::string L_base, int iterations,
                                 glm::vec3 start_pos, float radius)
{

    std::vector<std::tuple<glm::vec3, glm::vec3, float>> stack;     //first vec3 is pos, second vec3 is direction, third float is radius
    std::tuple<glm::vec3, glm::vec3, float> cur;

    std::vector<std::pair<glm::vec3, float>> points_list;           //first is pos, second is raduis
    std::vector<int> close_index;
    m_lsystem->change_base(L_base);
    std::string system = m_lsystem->derivation(iterations);
    std::cout<<"generated system: "<<system<<std::endl;
    glm::vec3 direction = glm::vec3(0, 1, 0);
    glm::vec3 pos = start_pos;
    for(int i = 0; i < system.size(); i++)
    {
        char key = system.at(i);
        glm::mat3 R;
        switch(key)
        {
        case 't':
            pos -= direction;
            break;
        case 'T':
            pos += direction;
            break;
        case 's':  //scale smaller
            radius /= scaleOffset;
            break;
        case 'S':
            radius *= scaleOffset;
            break;
        case 'x':
            direction = direction * rotate_up(rotationOffset);
            break;
        case 'X':
            direction = direction * rotate_up(-rotationOffset);
            break;
        case 'y':
            direction = direction * rotate_head(rotationOffset);
            break;
        case 'Y':
            direction = direction * rotate_head(-rotationOffset);
             break;
        case 'z':
            direction = direction * rotate_left(rotationOffset);
            break;
        case 'Z':
            direction = direction * rotate_left(-rotationOffset);
            break;
        case '[':
            cur = std::make_tuple(pos, direction, radius);
            stack.push_back(cur);
            break;
        case ']':
            cur = stack.back();
            stack.pop_back();
            pos = std::get<0>(cur);
            direction = std::get<1>(cur);
            radius = std::get<2>(cur);
            break;
        case 'c':
        case 'C':
            close_index.push_back(points_list.size()-1);
            break;
        case '+': //pushes the current point to be used on the geometry
            points_list.push_back(std::make_pair(pos, radius));
            break;
        case 'F':
            pos += direction;
            points_list.push_back(std::make_pair(pos, radius));
            break;
        default:
            break;
        }

    }
    std::cout<<"MeshGenerator::GenerateMesh()  finish parse the system"<<std::endl;
    //generate vertices for mesh
    std::vector<std::vector<glm::vec3>> mesh_list = MeshGenerator::generate_vertice(points_list);
    //generate mesh
    MeshGenerator::create_mesh(mesh_list, points_list, close_index);
    buildVAO();
}


std::vector<std::vector<glm::vec3>> MeshGenerator::generate_vertice(std::vector<std::pair<glm::vec3, float>> points_list){

    std::vector<std::vector<glm::vec3>> mesh_list;
    for (int i = 0; i < points_list.size(); i++)
    {
        std::vector<glm::vec3> points_per_level;
        for (int j = 0; j < points_per_lvl; j++)
        {
            glm::vec3 vertex;
            vertex = points_list[i].first;
            float radius =  points_list[i].second;
            glm::vec3 radVec = glm::vec3(radius, 0, 0);
            glm::vec3 rotAxis = vertex;
            if (i > 1)
               rotAxis = rotAxis - points_list[i - 1].first;
            if (glm::length(vertex) > 0)
               rotAxis = glm::normalize(rotAxis);
            else if (i + 1 < points_list.size())
               rotAxis = points_list[i + 1].first - rotAxis;
            else //Assume 0,1,0
               rotAxis = glm::vec3(0, 1, 0);
               float theta = 2 * M_PI / points_per_lvl * j;
               radVec = (float) cos(theta) * radVec + (glm::cross(rotAxis, radVec)) * (float)sin(theta) + rotAxis * (glm::dot(rotAxis, radVec))* (1.0f - (float)cos(theta));
               points_per_level.push_back(vertex + radVec);
         }
            mesh_list.push_back(points_per_level);
    }
    return mesh_list;
}

void MeshGenerator::create_mesh(std::vector<std::vector<glm::vec3>> mesh_list,
                                std::vector<std::pair<glm::vec3, float>> points_list,
                                 std::vector<int> close_index)
{
    for(int i = 0; i < mesh_list.size(); i++)
    {

            int currentSize = mesh_list[i].size();
            int nextRowSize = mesh_list[(i + 1) % mesh_list.size()].size();
            bool closedOff = false;
            for (int k = 0; k < currentSize; k++)
            {
                //Draw to the next layer of points, unless this is the first or last layer.
                //In those situations the shape needs to close, so connect those to the center points.
                int k_next = (k + 1) % currentSize;
                if (i == 0 || i == mesh_list.size() - 1)
                {
                    add_triangle_face(mesh_list[i][k], mesh_list[i][k_next], points_list[i].first);
                }

                //Don't connect points to the next layer of the index is a close off point
                if (closedOff || (close_index.size() > 0 && i == close_index.front()))
                {

                    add_triangle_face(mesh_list[i][k], mesh_list[i][k_next], points_list[i].first);
                    closedOff = true;
                    continue;
                }
                //Need to draw the face

                //Two triangles per face
                //Draw the first triangle of the face
                if( i < mesh_list.size() -1){
                    add_triangle_face(mesh_list[i][k], mesh_list[i][k_next], mesh_list[i + 1][k]);
                    add_triangle_face(mesh_list[i][k_next], mesh_list[i+1][k_next], mesh_list[i+1][k]);
                }
            }
            if(closedOff)
                close_index.erase(close_index.begin());
        }
}






glm::mat3 MeshGenerator::rotate_up(float rad)
{
   glm::mat3 token = glm::mat3( cos(rad),   sin(rad),  0,
                               -sin(rad),   cos(rad),  0,
                               0,           0,         1);
   return glm::transpose(token);

}

glm::mat3 MeshGenerator::rotate_left(float rad)
{
    glm::mat3 token = glm::mat3(cos(rad),   0,     -sin(rad),
                                  0,        1,         0,
                              sin(rad),     0,      cos(rad));
    return glm::transpose(token);
}

glm::mat3 MeshGenerator::rotate_head(float rad)
{
    glm::mat3 token = glm::mat3(   1,       0,          0,
                                   0,    cos(rad),   -sin(rad),
                                   0,    sin(rad),    cos(rad));
     return glm::transpose(token);
}

void MeshGenerator::add_triangle_face(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
    glm::vec3 v1 = p1-p3;
    glm::vec3 v2 = p2-p3;
    glm::vec3 normal = glm::normalize(glm::cross(v1,v2));
    add_single_vertex(p1, normal);
    add_single_vertex(p2, normal);
    add_single_vertex(p3, normal);

}
void MeshGenerator::add_single_vertex(glm::vec3 point, glm::vec3 normal){
    m_vertexData.push_back(point.x);
    m_vertexData.push_back(point.y);
    m_vertexData.push_back(point.z);
    m_vertexData.push_back(normal[0]);
    m_vertexData.push_back(normal[1]);
    m_vertexData.push_back(normal[2]);
}
