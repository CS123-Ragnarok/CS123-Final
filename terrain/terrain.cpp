#include "terrain.h"

#include <math.h>
#include "gl/shaders/ShaderAttribLocations.h"

#include "ResourceLoader.h"

Terrain::Terrain() : m_numRows(100), m_numCols(m_numRows)
{
    loadPhongShader();
}

void Terrain::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

/**
 * Returns a pseudo-random value between -1.0 and 1.0 for the given row and column.
 */
float Terrain::randValue(int row, int col) {
    return -1.0 + 2.0 * glm::fract(sin(row * 127.1f + col * 311.7f) * 43758.5453123f);
}


/**
 * Returns the object-space position for the terrain vertex at the given row and column.
 */
glm::vec3 Terrain::getPosition(int row, int col) {
    glm::vec3 position;
    position.x = 10 * row/m_numRows - 5;
    position.z = 10 * col/m_numCols - 5;

    // TODO: Adjust position.y using value noise.
    for (int i = 1; i <= 5; i++){

        float A = randValue(floor(row / 20 * i), floor(col / 20 * i));
        float B = randValue(floor(row / 20 * i), floor(col / 20 * i)+1);
        float C = randValue(floor(row / 20 * i)+1, floor(col / 20 * i));
        float D = randValue(floor(row / 20 * i)+1, floor(col / 20 * i)+1);

        float x = glm::fract(col/20.f * i);
        float y = glm::fract(row/20.f * i);
        float AB = glm::mix(A, B, (float) x*x*(3-2*x));
        float CD = glm::mix(C, D, (float) x*x*(3-2*x));
        float val = glm::mix(AB, CD, (float) y*y*(3-2*y));
        position.y += val / glm::exp2((float) (i-1));

    }
    return position;
}


/**
 * Returns the normal vector for the terrain vertex at the given row and column.
 */
glm::vec3 Terrain::getNormal(int row, int col) {
    // TODO: Compute the normal at the given row and column using the positions of the
    //       neighboring vertices.
    auto p = getPosition(row, col);
    auto n0 = getPosition(row, col+1);
    auto n1 = getPosition(row-1, col+1);
    auto n2 = getPosition(row-1, col);
    auto n3 = getPosition(row-1, col-1);
    auto n4 = getPosition(row, col-1);
    auto n5 = getPosition(row+1, col-1);
    auto n6 = getPosition(row+1, col);
    auto n7 = getPosition(row+1, col+1);

    auto normal0 = glm::normalize(glm::cross(n1-p, n0-p));
    auto normal1 = glm::normalize(glm::cross(n2-p, n1-p));
    auto normal2 = glm::normalize(glm::cross(n3-p, n2-p));
    auto normal3 = glm::normalize(glm::cross(n4-p, n3-p));
    auto normal4 = glm::normalize(glm::cross(n5-p, n4-p));
    auto normal5 = glm::normalize(glm::cross(n6-p, n5-p));
    auto normal6 = glm::normalize(glm::cross(n7-p, n6-p));
    auto normal7 = glm::normalize(glm::cross(n0-p, n7-p));

    return glm::normalize(normal0 + normal1 + normal2 +
                          normal3 + normal4 + normal5 +
                          normal6 + normal7);
}


/**
 * Initializes the terrain by storing positions and normals in a vertex buffer.
 */
void Terrain::init() {
    // TODO: Change from GL_LINE to GL_FILL in order to render full triangles instead of wireframe.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    // Initializes a grid of vertices using triangle strips.
    int numVertices = (m_numRows - 1) * (2 * m_numCols + 2);
    std::vector<glm::vec3> data(2 * numVertices);
    int index = 0;
    for (int row = 0; row < m_numRows - 1; row++) {
        for (int col = m_numCols - 1; col >= 0; col--) {
            data[index++] = getPosition(row, col);
            data[index++] = getNormal  (row, col);
            data[index++] = getPosition(row + 1, col);
            data[index++] = getNormal  (row + 1, col);
        }
        data[index++] = getPosition(row + 1, 0);
        data[index++] = getNormal  (row + 1, 0);
        data[index++] = getPosition(row + 1, m_numCols - 1);
        data[index++] = getNormal  (row + 1, m_numCols - 1);
    }

    // Initialize OpenGLShape.
    m_shape = std::make_unique<OpenGLShape>();
    m_shape->setVertexData(&data[0][0], data.size() * 3, VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, numVertices);
    m_shape->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->setAttribute(ShaderAttrib::NORMAL, 3, sizeof(glm::vec3), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_shape->buildVAO();
}


/**
 * Draws the terrain.
 */
void Terrain::draw()
{
    //m_phongShader.
    m_shape->draw();
}
