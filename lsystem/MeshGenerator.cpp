#include "MeshGenerator.h"
#include "gl/datatype/VAO.h"
#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include <iostream>
#include <unordered_map>

using namespace CS123::GL;

MeshGenerator::MeshGenerator(int seed):
    m_VAO(nullptr)
{
    m_lsystem = std::make_unique<LSystem>(seed);
//    m_lsystem->add_rules('F', "F[Fz[zFZXFZYF]Z[ZFxzFyzF]C+]");
//    m_lsystem->add_rules('R', "FFF[FXYZ[FxRxF[zFRzXFC]R[ZFZyFC]]yFRyF]");
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

void MeshGenerator::drawLeave(){
    if (m_leave_VAO) {
        m_leave_VAO->bind();
        m_leave_VAO->draw();
        m_leave_VAO->unbind();
    }
}

void MeshGenerator::drawBranch(){
    if (m_branch_VAO) {
        m_branch_VAO->bind();
        m_branch_VAO->draw();
        m_branch_VAO->unbind();
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

/*
    const int num_leave_vertices = m_leaves.size() / numFloatsPerVertex;
    std::vector<VBOAttribMarker> leave_markers;
    leave_markers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
    leave_markers.push_back(VBOAttribMarker(ShaderAttrib::NORMAL, 3, 3*sizeof(float)));
    VBO leave_vbo = VBO(m_leaves.data(), m_leaves.size(), leave_markers);
    m_leave_VAO = std::make_unique<VAO>(leave_vbo, num_leave_vertices);

    const int num_branch_vertices = m_leaves.size() / numFloatsPerVertex;

    std::vector<VBOAttribMarker> branch_markers;
    branch_markers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
    branch_markers.push_back(VBOAttribMarker(ShaderAttrib::NORMAL, 3, 3*sizeof(float)));
    VBO branch_vbo = VBO(m_branches.data(), m_branches.size(), branch_markers);
    m_branch_VAO = std::make_unique<VAO>(branch_vbo, num_branch_vertices);
*/
}


void MeshGenerator::GenerateMesh(std::string L_base, int iterations,
                                 glm::vec3 start_pos, float radius)
{

    std::vector<std::tuple<glm::vec3, glm::vec3, float, tree_node*>> stack;     //first vec3 is pos, second vec3 is direction, third float is radius, 4th is the last node
    std::tuple<glm::vec3, glm::vec3, float, tree_node*> cur;                //first vec3 is pos, second vec3 is direction, third float is radius, 4th is the last node
    tree_node* last_node = NULL;
    tree_node* token;

    std::vector<tree_node*> points_list;            //first is pos, second is raduis
    std::vector<int> close_index;
    m_lsystem->change_base(L_base);
    std::string system = m_lsystem->derivation(iterations);
    //std::cout<<"generated system: "<<system<<std::endl;

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
            cur = std::make_tuple(pos, direction, radius, last_node);
            stack.push_back(cur);
            break;
        case ']':
            cur = stack.back();
            stack.pop_back();
            pos = std::get<0>(cur);
            direction = std::get<1>(cur);
            radius = std::get<2>(cur);
            last_node = std::get<3>(cur);
            break;
        case 'c':
        case 'C':
            close_index.push_back(points_list.size()-1);
            break;
        case '+': //pushes the current point to be used on the geometry
            token = new tree_node;
            token->pos = pos;
            token->parent = last_node;
            token->radius = radius;
            last_node = token;
            points_list.push_back(token);
            break;
        case 'F':
            pos += direction;
            token = new tree_node;
            token->pos = pos;
            token->parent = last_node;
            token->radius = radius;
            last_node = token;
            points_list.push_back(token);
            break;
        default:
            break;
        }


    }
    //std::cout<<"MeshGenerator::GenerateMesh()  finish parse the system"<<std::endl;
    //generate vertices for mesh
    std::vector<std::pair<int, std::vector<glm::vec3>>> mesh_list = MeshGenerator::generate_vertice(points_list);
    //generate mesh
    MeshGenerator::create_mesh(mesh_list, points_list, close_index);
    buildVAO();
    for(auto node : points_list)
         delete node;

}


std::vector<std::pair<int, std::vector<glm::vec3>>> MeshGenerator::generate_vertice(std::vector<tree_node*> points_list)
{

    glm::vec3 non_existed = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);
    myMap pos2index;
    pos2index[non_existed] = -1;
    std::vector<std::pair<int, std::vector<glm::vec3>>> mesh_list;
    for(int i = 0; i < points_list.size(); i++)
    {
        tree_node* cur_node = points_list[i];
        glm::vec3 cur_pos = cur_node->pos;
        tree_node* parent_node = cur_node->parent;
        glm::vec3 parent_pos = non_existed;
        if(parent_node != NULL)
            parent_pos = parent_node->pos;
        glm::vec3 radVec =  glm::vec3(cur_node->radius, 0, 0);
        pos2index[cur_pos] = i;

        std::vector<glm::vec3> level_vertices;
        for(int j = 0; j < points_per_lvl; j++)
        {
           glm::vec3 rotAxis = glm::vec3(0,1,0);
           if(parent_node != NULL)
                rotAxis = glm::normalize(cur_pos - parent_pos);
           float theta = 2 * M_PI / points_per_lvl * j;
           radVec = (float) cos(theta) * radVec + (glm::cross(rotAxis, radVec)) * (float)sin(theta) + rotAxis * (glm::dot(rotAxis, radVec))* (1.0f - (float)cos(theta));
           level_vertices.push_back(cur_pos + radVec);
        }
        int parent_level_index = pos2index[parent_pos];
        mesh_list.push_back(std::make_pair(parent_level_index, level_vertices));
    }
    return mesh_list;

}

void MeshGenerator::create_mesh(std::vector<std::pair<int, std::vector<glm::vec3>>> mesh_list,
                               std::vector<tree_node*> points_list,
                                 std::vector<int> close_index)
{
    for(int i = 0; i < mesh_list.size(); i++)
    {
            int parent_lvl_index = mesh_list[i].first;
            std::vector<glm::vec3> parent_lvl_vertices;
            if(parent_lvl_index != -1)
                parent_lvl_vertices = mesh_list[parent_lvl_index].second;
            int parent_lvl_size = parent_lvl_vertices.size();
            std::vector<glm::vec3> current_lvl_vertices =  mesh_list[i].second;
            int current_lvl_size = current_lvl_vertices.size();
            bool closedOff = false;
            for (int k = 0; k < current_lvl_size; k++)
            {
                //Draw to the next layer of points, unless this is the first or last layer.
                //In those situations the shape needs to close, so connect those to the center points.
                int k_next = (k + 1) % current_lvl_size;
                if (parent_lvl_size == 0)
                {
                    add_triangle_face(current_lvl_vertices[k], current_lvl_vertices[k_next], points_list[i]->pos,1);
                    continue;
                }

                //Don't connect points to the next layer of the index is a close off point
                if (closedOff || (close_index.size() > 0 && i == close_index.front()))
                {
                    add_triangle_face(current_lvl_vertices[k], current_lvl_vertices[k_next], points_list[i]->pos,1);
                    closedOff = true;

                }
                //Need to draw the face
                //Two triangles per face
                //Draw the first triangle of the face

                 add_triangle_face(parent_lvl_vertices[k], parent_lvl_vertices[k_next], current_lvl_vertices[k], 0);
                 add_triangle_face(parent_lvl_vertices[k_next], current_lvl_vertices[k_next],  current_lvl_vertices[k], 0);


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

void MeshGenerator::add_triangle_face(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int choice)
{
    glm::vec3 v1 = p1-p3;
    glm::vec3 v2 = p2-p3;
    glm::vec3 normal = glm::normalize(glm::cross(v1,v2));
    add_single_vertex(p1, normal, choice);
    add_single_vertex(p2, normal, choice);
    add_single_vertex(p3, normal, choice);

}
void MeshGenerator::add_single_vertex(glm::vec3 point, glm::vec3 normal, int choice){
    m_vertexData.push_back(point.x);
    m_vertexData.push_back(point.y);
    m_vertexData.push_back(point.z);
    m_vertexData.push_back(normal[0]);
    m_vertexData.push_back(normal[1]);
    m_vertexData.push_back(normal[2]);
}
