#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include "GL/glew.h"
#include <vector>
#include <glm/glm.hpp>
#include "GL/glew.h"
#include "gl/datatype/VAO.h"


#include <iostream>

#define GRAVITY -50
#define dt 0.0167

class Particle
{

public:
    glm::vec3 pos;
    glm::vec3 velocity;
    float gravityEffect;
    float lifeLength;
    float rotation;
    float scale;

    float elapsedTime = 0;


    bool update();
    void buildVAO();
    void draw();
    glm::mat4x4 updateModelViewMatrix(float rotation, float scale, glm::mat4x4 viewMatrix);




    std::vector<GLfloat> m_vertexData = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f};
    std::unique_ptr<CS123::GL::VAO> m_VAO;

};


#endif
