#include "terrain.h"

#include <math.h>
#include "gl/shaders/ShaderAttribLocations.h"
#include <iostream>

#include "ResourceLoader.h"
#include "lib/CS123SceneData.h"

Terrain::Terrain() : m_numRows(100), m_numCols(m_numRows)
{
    //loadPhongShader();
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

    for (int i = 1; i <= 3; i++){

        float A = randValue(floor(row / 20 * i), floor(col / 20 * i));
        float B = randValue(floor(row / 20 * i), floor(col / 20 * i)+1);
        float C = randValue(floor(row / 20 * i)+1, floor(col / 20 * i));
        float D = randValue(floor(row / 20 * i)+1, floor(col / 20 * i)+1);

        float x = glm::fract(col/20.f * i);
        float y = glm::fract(row/20.f * i);
        float AB = glm::mix(A, B, (float) x*x*(3-2*x));
        float CD = glm::mix(C, D, (float) x*x*(3-2*x));
        float val = glm::mix(AB, CD, (float) y*y*(3-2*y));
        position.y += val / pow(4.f, (float) (i-1));

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
    loadPhongShader();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    // Initializes a grid of vertices using triangle strips.
    // top
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
    m_top = std::make_unique<OpenGLShape>();
    m_top->setVertexData(&data[0][0], data.size() * 3, VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, numVertices);
    m_top->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_top->setAttribute(ShaderAttrib::NORMAL, 3, sizeof(glm::vec3), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    //m_shape->setAttribute(ShaderAttrib::TEXCOORD2, 2, 2*sizeof(glm::vec3), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_top->buildVAO();

    // initialize side boxes
    genSides();
}

void Terrain::genSides() {
    glm::vec3 temp, first, last;
    int index;
    float bot_y = -1.5f;
    // right
    index = 0;
    std::vector<glm::vec3> right_data(4 * m_numCols + 4);
    for (int col = m_numCols - 1; col >= 0; col--) {
        temp = getPosition(m_numRows-2, col);
        right_data[index++] = temp;
        right_data[index++] = glm::vec3(1.f, 0.f, 0.f);
        right_data[index++] = glm::vec3(temp.x, bot_y, temp.z);
        right_data[index++] = glm::vec3(1.f, 0.f, 0.f);
    }
    last = getPosition(m_numRows-2, 0);
    right_data[index++] = glm::vec3(last.x, bot_y, last.z);
    right_data[index++] = glm::vec3(-1.f, 0.f, 0.f);
    first = getPosition(m_numRows-2, m_numCols - 1);
    right_data[index++] = glm::vec3(first.x, bot_y, first.z);
    right_data[index++] = glm::vec3(-1.f, 0.f, 0.f);

    m_right = std::make_unique<OpenGLShape>();
    m_right->setVertexData(&right_data[0][0], right_data.size() * 3, VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 2 * m_numCols + 2);
    m_right->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_right->setAttribute(ShaderAttrib::NORMAL, 3, sizeof(glm::vec3), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_right->buildVAO();

    // left
    index = 0;
    std::vector<glm::vec3> left_data(4 * m_numCols + 4);
    for (int col = 0; col <= m_numCols - 1; col++) {
        temp = getPosition(0, col);
        left_data[index++] = temp;
        left_data[index++] = glm::vec3(-1.f, 0.f, 0.f);
        left_data[index++] = glm::vec3(temp.x, bot_y, temp.z);
        left_data[index++] = glm::vec3(-1.f, 0.f, 0.f);
    }
    last = getPosition(0, m_numCols - 1);
    left_data[index++] = glm::vec3(last.x, bot_y, last.z);
    left_data[index++] = glm::vec3(-1.f, 0.f, 0.f);
    first = getPosition(0, 0);
    left_data[index++] = glm::vec3(first.x, bot_y, first.z);
    left_data[index++] = glm::vec3(-1.f, 0.f, 0.f);

    m_left = std::make_unique<OpenGLShape>();
    m_left->setVertexData(&left_data[0][0], left_data.size() * 3, VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 2 * m_numCols + 2);
    m_left->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_left->setAttribute(ShaderAttrib::NORMAL, 3, sizeof(glm::vec3), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_left->buildVAO();

    // front
    index = 0;
    std::vector<glm::vec3> front_data(4 * m_numRows);
    for (int row = 0; row < m_numRows - 1; row++) {
        temp = getPosition(row, m_numCols - 1);
        front_data[index++] = temp;
        front_data[index++] = glm::vec3(0.f, 0.f, 1.f);
        front_data[index++] = glm::vec3(temp.x, bot_y, temp.z);
        front_data[index++] = glm::vec3(0.f, 0.f, 1.f);
    }
    last = getPosition(m_numRows - 2, m_numCols - 1);
    front_data[index++] = glm::vec3(last.x, bot_y, last.z);
    front_data[index++] = glm::vec3(0.f, 0.f, 1.f);
    first = getPosition(0, m_numCols - 1);
    front_data[index++] = glm::vec3(first.x, bot_y, first.z);
    front_data[index++] = glm::vec3(0.f, 0.f, 1.f);

    m_front = std::make_unique<OpenGLShape>();
    m_front->setVertexData(&front_data[0][0], front_data.size() * 3, VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 2 * m_numRows);
    m_front->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_front->setAttribute(ShaderAttrib::NORMAL, 3, sizeof(glm::vec3), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_front->buildVAO();

    // back
    index = 0;
    std::vector<glm::vec3> back_data(4 * m_numRows);
    for (int row = m_numRows - 2; row >= 0 ; row--) {
        temp = getPosition(row, 0);
        back_data[index++] = temp;
        back_data[index++] = glm::vec3(0.f, 0.f, -1.f);
        back_data[index++] = glm::vec3(temp.x, bot_y, temp.z);
        back_data[index++] = glm::vec3(0.f, 0.f, -1.f);
    }
    last = getPosition(0, 0);
    back_data[index++] = glm::vec3(last.x, bot_y, last.z);
    back_data[index++] = glm::vec3(0.f, 0.f, -1.f);
    first = getPosition(m_numRows - 2, 0);
    back_data[index++] = glm::vec3(first.x, bot_y, first.z);
    back_data[index++] = glm::vec3(0.f, 0.f, -1.f);

    m_back = std::make_unique<OpenGLShape>();
    m_back->setVertexData(&back_data[0][0], back_data.size() * 3, VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 2 * m_numRows);
    m_back->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_back->setAttribute(ShaderAttrib::NORMAL, 3, sizeof(glm::vec3), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_back->buildVAO();

    // bot
    std::vector<glm::vec3> bot_data(8);
    index = 0;
    temp = getPosition(m_numRows - 2, 0);
    bot_data[index++] = glm::vec3(temp.x, bot_y, temp.z);
    bot_data[index++] = glm::vec3(0.f, -1.f, 0.f);
    temp = getPosition(m_numRows - 2, m_numCols - 1);
    bot_data[index++] = glm::vec3(temp.x, bot_y, temp.z);
    bot_data[index++] = glm::vec3(0.f, -1.f, 0.f);
    temp = getPosition(0, 0);
    bot_data[index++] = glm::vec3(temp.x, bot_y, temp.z);
    bot_data[index++] = glm::vec3(0.f, -1.f, 0.f);
    temp = getPosition(0, m_numCols - 1);
    bot_data[index++] = glm::vec3(temp.x, bot_y, temp.z);
    bot_data[index++] = glm::vec3(0.f, -1.f, 0.f);

    m_bot = std::make_unique<OpenGLShape>();
    m_bot->setVertexData(&bot_data[0][0], bot_data.size() * 3, VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 4);
    m_bot->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_bot->setAttribute(ShaderAttrib::NORMAL, 3, sizeof(glm::vec3), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    //m_shape->setAttribute(ShaderAttrib::TEXCOORD2, 2, 2*sizeof(glm::vec3), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_bot->buildVAO();
}

/**
 * Draws the terrain.
 */
void Terrain::draw()
{
    CS123SceneMaterial topmaterial;
    topmaterial.cAmbient = glm::vec4(37.0f / 255.0f, 69.0f / 255.0f, 18.0f / 255.0f, 1.0f);
    topmaterial.cDiffuse = glm::vec4(0.5f);
    topmaterial.cSpecular = glm::vec4(0.3f);
    topmaterial.shininess = 1.0f;
    m_phongShader->applyMaterial(topmaterial);

    m_phongShader->setUniform("m", glm::translate(glm::mat4x4(1.0), glm::vec3(0.f, -1.5f, 0.f)));

    m_top->draw();

    CS123SceneMaterial sidematerial;
    sidematerial.cAmbient = glm::vec4(64.0f / 255.0f, 41.0f / 255.0f, 5.0f / 255.0f, 1.0f);
    sidematerial.cDiffuse = glm::vec4(0.2f);
    sidematerial.cSpecular = glm::vec4(0.f);
    sidematerial.shininess = 1.0f;
    m_phongShader->applyMaterial(sidematerial);

    m_right->draw();
    m_left->draw();
    m_front->draw();
    m_back->draw();
    m_bot->draw();
}


void Terrain::render(Camera *camera) {
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_phongShader->bind();
    // set sceneuniforms
    m_phongShader->setUniform("useLighting", true);
    m_phongShader->setUniform("useArrowOffsets", false);
    m_phongShader->setUniform("p" , camera->getProjectionMatrix());
    m_phongShader->setUniform("v", camera->getViewMatrix());

    // set lights
    CS123SceneLightData testlight;
    testlight.id = 0;
    testlight.type = LightType::LIGHT_DIRECTIONAL;
    testlight.dir = glm::normalize(glm::vec4(1.0f, -1.0f, -1.0f, 0.0f));
    testlight.color = glm::vec4(1.0f);
    m_phongShader->setLight(testlight);

    // draw terrain
    draw();

    m_phongShader->unbind();
}
