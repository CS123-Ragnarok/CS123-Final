#ifndef MESHGEN_H
#define MESHGEN_H

#include "LSystem.h"
#include <glm/glm.hpp>
#include "GL/glew.h"
#include "datatype/VAO.h"
#include<memory>
#include <vector>

class MeshGenerator {
public:
    MeshGenerator();
    ~MeshGenerator();

    void draw();
    void buildVAO();

    void GenerateMesh(std::string system, int iterations, glm::vec3 startingPoint, float radius);

    int points_per_lvl = 6;
    std::shared_ptr<LSystem> m_lsystem;


private:


    float rotationOffset = M_PI / 8.0f;
    float scaleOffset = 2.0f;
    float translationOffset = 1.0f;

    std::vector<GLfloat> m_vertexData;
    std::unique_ptr<CS123::GL::VAO> m_VAO;

     std::vector<std::vector<glm::vec3>> generate_vertice(std::vector<std::pair<glm::vec3, float>> points_list);

     void create_mesh(std::vector<std::vector<glm::vec3>> mesh_list,
                      std::vector<std::pair<glm::vec3, float>> points_list,
                      std::vector<int> close_index);

     void add_triangle_face(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

     void add_single_vertex(glm::vec3 point, glm::vec3 normal);




    glm::mat3 rotate_up(float rad);
    glm::mat3 rotate_left(float rad);
    glm::mat3 rotate_head(float rad);



};



#endif
