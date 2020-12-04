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

private:
    LSystem *m_lsystem;

    float rotationOffset = M_PI / 8.0f;
    float scaleOffset = 2.0f;
    float translationOffset = 1.0f;

    std::vector<GLfloat> m_vertexData;
    std::unique_ptr<CS123::GL::VAO> m_VAO;

     glm::vec3 crossProduct(glm::vec3 v_A, glm::vec3 v_B);

    void GenerateMesh(std::string system, int iterations, std::string modelName, glm::vec3 startingPoint, float radius, int pointsPerLevel);
};



#endif
