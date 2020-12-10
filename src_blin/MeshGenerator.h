#ifndef MESHGEN_H
#define MESHGEN_H

#include "LSystem.h"
#include <glm/glm.hpp>
#include "GL/glew.h"
#include "gl/datatype/VAO.h"

#include <unordered_map>
#include <memory>
#include <vector>

struct tree_node{
    glm::vec3 pos;
    tree_node* parent;

    float radius;
};

struct KeyFuncs
{
    size_t operator()(const glm::vec3& k)const
    {
        return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
    }

    bool operator()(const glm::vec3& a, const glm::vec3& b)const
    {
            return a.x == b.x && a.y == b.y && a.z == b.z;
    }
};

typedef std::unordered_map<glm::vec3, int, KeyFuncs, KeyFuncs> myMap;
class MeshGenerator {
public:
    MeshGenerator(int seed);
    ~MeshGenerator();

    void draw();
    void drawLeave();
    void drawBranch();
    void buildVAO();

    void GenerateMesh(std::string L_base, int iterations, glm::vec3 start_pos, float radius);

    int points_per_lvl = 10;

     std::unique_ptr<LSystem> m_lsystem;


private:


    float rotationOffset = M_PI / 8.0f;
    float scaleOffset = 2.0f;
    float translationOffset = 1.0f;

    std::vector<GLfloat> m_vertexData;
    std::unique_ptr<CS123::GL::VAO> m_VAO;

    std::vector<GLfloat> m_leaves;
    std::vector<GLfloat> m_branches;

    std::unique_ptr<CS123::GL::VAO> m_leave_VAO;
    std::unique_ptr<CS123::GL::VAO> m_branch_VAO;

    std::vector<std::pair<int, std::vector<glm::vec3>>> generate_vertice(std::vector<tree_node*> points_list);


     void create_mesh(std::vector<std::pair<int, std::vector<glm::vec3>>> mesh_list,
                                    std::vector<tree_node*> points_list,
                                      std::vector<int> close_index);

     void add_triangle_face(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int choice);

     void add_single_vertex(glm::vec3 point, glm::vec3 normal, int choice);




    glm::mat3 rotate_up(float rad);
    glm::mat3 rotate_left(float rad);
    glm::mat3 rotate_head(float rad);



};



#endif
