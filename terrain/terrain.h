#ifndef TERRAIN_H
#define TERRAIN_H

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr
#include <vector>

#include "gl/shaders/CS123Shader.h"
#include "camera/Camera.h"
#include "openglshape.h"

class Terrain {
public:
    Terrain();

    void init();
    void render(Camera *camera);
    void draw();

private:
    void loadPhongShader();
    float randValue(int row, int col);
    glm::vec3 getPosition(int row, int col);
    glm::vec3 getNormal(int row, int col);


    std::unique_ptr<OpenGLShape> m_shape;
    const float m_numRows, m_numCols;

    std::unique_ptr<CS123::GL::CS123Shader> m_phongShader;

};

#endif // TERRAIN_H
