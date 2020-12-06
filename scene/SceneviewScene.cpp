#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include <QObject>

#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr


#include "lib/ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"
#include "shapes/ConeShape.h"
#include "shapes/CubeShape.h"
#include "shapes/CylinderShape.h"
#include "shapes/SphereShape.h"

#include <list>

#include <iostream>

using namespace CS123::GL;

SceneviewScene::SceneviewScene()
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
    loadPhongShader();


    m_cube = std::make_unique<CubeShape>(1, 1);
    m_cone = std::make_unique<ConeShape>(1, 20);
    m_cylinder = std::make_unique<CylinderShape>(1, 20);
    m_sphere = std::make_unique<SphereShape>(20, 20);

    m_tree = std::make_unique<MeshGenerator>();
    //m_tree->m_lsystem->add_rules('F', "F[Fz[zFZXFZYF]Z[ZFxzFyzF]C+]");
    //m_tree->m_lsystem->add_rules('F', "F[Fz[zFZXFZYF]Z[ZFxzFyzF]+]");
    //m_tree->m_lsystem->add_rules('R', "FFF[FXYZ[FxRxF[zFRzXFC]R[ZFZyFC]]yFRyF]");
    //m_tree->m_lsystem->add_rules('B', "XXYYYYYYYYFRFzzFRRC");
    m_tree->GenerateMesh("+TT+R", 3, glm::vec3(0.0f), 0.1f);

    //paintTrees();

    m_testLight.type = LightType::LIGHT_DIRECTIONAL;
    m_testLight.dir = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
    m_testLight.color = glm::vec4(1.0f);

    m_timer.start(1000.0f / m_fps);
    m_increment = 0;
}

SceneviewScene::~SceneviewScene()
{
}

void SceneviewScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

CS123ScenePrimitive SceneviewScene::getBranch(){
    CS123ScenePrimitive res;
    res.type = PrimitiveType::PRIMITIVE_CYLINDER;
    res.material.cAmbient = glm::vec4(139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f, 1.0f);
    res.material.cDiffuse = glm::vec4(0.5f);
    res.material.cSpecular = glm::vec4(0.5f);
    res.material.shininess = 1.0f;
    return res;
}

CS123ScenePrimitive SceneviewScene::getLeave(){
    CS123ScenePrimitive res;
    res.type = PrimitiveType::PRIMITIVE_CYLINDER;
    res.material.cAmbient = glm::vec4(0.0f / 255.0f, 255.0f / 255.0f, 127.0f / 255.0f, 1.0f);
    res.material.cDiffuse = glm::vec4(0.5f);
    res.material.cSpecular = glm::vec4(0.5f);
    res.material.shininess = 1.0f;
    return res;
}

void SceneviewScene::paintTree(glm::vec4 place, glm::vec4 dir, std::string lTree, float scale, float angle){
    /*
    place :


    */
    int size = lTree.size();
    glm::vec3 y = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 d = dir.xyz();
    glm::vec3 a = glm::cross(y, d);

    float omega = glm::acos(glm::dot(y, d));

    glm::mat4 base = glm::translate(glm::vec3(0.0f, scale * 1.0f, 0.0f)) * glm::scale(glm::vec3(scale * 0.5f, scale * 2.0f, scale * 0.5f));
    glm::mat4 cur_rotation = glm::rotate(omega, a);
    glm::vec4 cur_translation = place;
    std::list<glm::mat4> rotations;
    std::list<glm::vec4> translations;

    for(int i = 0; i < size; i++){
        char c = lTree[i];
        switch (c) {
        case 'f':
            m_primitives.push_back(getBranch());
            m_trans.push_back(glm::translate(cur_translation.xyz()) * cur_rotation * base);
            cur_translation += scale * glm::normalize(cur_rotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
            break;
        case '+':
            cur_rotation = glm::rotate( angle, (cur_rotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)).xyz()) * cur_rotation;
            break;
        case '-':
            cur_rotation = glm::rotate(-angle, (cur_rotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)).xyz()) * cur_rotation;
            break;
        case '&':
            cur_rotation = glm::rotate( angle, (cur_rotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)).xyz()) * cur_rotation;
            break;
        case '^':
            cur_rotation = glm::rotate(-angle, (cur_rotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)).xyz()) * cur_rotation;
            break;
        case '\\':
            cur_rotation = glm::rotate( angle, (cur_rotation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)).xyz()) * cur_rotation;
            break;
        case '/':
            cur_rotation = glm::rotate(-angle, (cur_rotation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)).xyz()) * cur_rotation;
            break;
        case '[':
            rotations.push_back(cur_rotation);
            translations.push_back(cur_translation);
            break;
        case ']':
            cur_rotation = rotations.back();
            cur_translation = translations.back();
            rotations.pop_back();
            translations.pop_back();
            break;
        }

    }
}

void SceneviewScene::paintTrees(){
    m_primitives.push_back(getBranch());
    m_trans.push_back(glm::mat4());
    paintTree(glm::vec4(0.0f), glm::vec4(1.0f / sqrt(6), 2.0f / sqrt(6), 1.0f / sqrt(6),0.0f), "f[-f][+f]f", 1.0f, glm::radians(90.0f));
}

void SceneviewScene::render() {
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_phongShader->bind();
    setSceneUniforms();
    setLights();
    renderGeometry();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_phongShader->unbind();

}

void SceneviewScene::setSceneUniforms() {
//    Camera *camera = context->getCamera();
    m_phongShader->setUniform("useLighting", true);
//    m_phongShader->setUniform("useArrowOffsets", false);
//    m_phongShader->setUniform("p" , camera->getProjectionMatrix());
//    m_phongShader->setUniform("v", camera->getViewMatrix());


    //float time = m_increment++ / (float) m_fps;      // Time in seconds.

    float fieldOfViewY = 0.8f;                       // Vertical field of view angle, in radians.
    //float aspectRatio = (float)width() / height();   // Aspect ratio of the window.
    float aspectRatio = 1.03257f;
    float nearClipPlane = 0.1f;                      // Near clipping plane.
    float farClipPlane = 100.f;                      // Far clipping plane.

    // TODO: Adjust the eye coordinates so the camera goes in a circle of radius 6 where
    // y is always equal to 1. (Task 7)

    //glm::vec3 eye = glm::vec3(0.f, 1, 6.f);        // Camera position.
    glm::vec3 eye = glm::vec3(20.0f, 10.0f, 20.0f);        // Camera position.
    glm::vec3 center = glm::vec3(0.f, 5.f, 0.f);     // Where camera is looking.
    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);         // Up direction.

    // TODO: Generate view matrix and pass it to vertex shader. (Task 4)
    glm::mat4 view = glm::lookAt(eye, center, up);
    glm::mat4 perspective = glm::perspective(fieldOfViewY, aspectRatio, nearClipPlane, farClipPlane);

    m_phongShader->setUniform("p", perspective);
    m_phongShader->setUniform("v", view);
}

void SceneviewScene::setMatrixUniforms(Shader *shader) {
//    shader->setUniform("p", context->getCamera()->getProjectionMatrix());
//    shader->setUniform("v", context->getCamera()->getViewMatrix());
    float time = m_increment++ / (float) m_fps;      // Time in seconds.

    float fieldOfViewY = 0.8f;                       // Vertical field of view angle, in radians.
    //float aspectRatio = (float)width() / height();   // Aspect ratio of the window.
    float aspectRatio = 1.03257f;
    float nearClipPlane = 0.1f;                      // Near clipping plane.
    float farClipPlane = 100.f;                      // Far clipping plane.

    // TODO: Adjust the eye coordinates so the camera goes in a circle of radius 6 where
    // y is always equal to 1. (Task 7)

    glm::vec3 eye = glm::vec3(6.0f, 6.0f, 6.0f);        // Camera position.
    //glm::vec3 eye = glm::vec3(sqrt(time), 2.0f, 2.0f);        // Camera position.
    glm::vec3 center = glm::vec3(0.f, 1.f, 0.f);     // Where camera is looking.
    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);         // Up direction.

    // TODO: Generate view matrix and pass it to vertex shader. (Task 4)
    glm::mat4 view = glm::lookAt(eye, center, up);
    glm::mat4 perspective = glm::perspective(fieldOfViewY, aspectRatio, nearClipPlane, farClipPlane);

    shader->setUniform("p", perspective);
    shader->setUniform("v", view);

}

void SceneviewScene::setLights()
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Set up the lighting for your scene using m_phongShader.
    // The lighting information will most likely be stored in CS123SceneLightData structures.
    //
//    for(auto light : m_lights)
//    {
//        m_phongShader->setLight(light);
//    }
    m_phongShader->setLight(m_testLight);
}

void SceneviewScene::renderGeometry() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // TODO: [SCENEVIEW] Fill this in...
    // You shouldn't need to write *any* OpenGL in this class!
    //
    //
    // This is where you should render the geometry of the scene. Use what you
    // know about OpenGL and leverage your Shapes classes to get the job done.
    //

//    for(auto tup : m_primitives){
//        CS123ScenePrimitive p = std::get<0>(tup);
//        glm::mat4x4 trans = std::get<1>(tup);
//        p.material.cDiffuse = m_global.kd * p.material.cDiffuse;
//        p.material.cAmbient = m_global.ka * p.material.cAmbient;
//        m_phongShader->applyMaterial(p.material);
//        m_phongShader->setUniform("m", trans);
//        switch(p.type){
//        case PrimitiveType::PRIMITIVE_CUBE:
//            m_cube->draw();
//            break;
//        case PrimitiveType::PRIMITIVE_CONE:
//            m_cone->draw();
//            break;
//        case PrimitiveType::PRIMITIVE_CYLINDER:
//            m_cylinder->draw();
//            break;
//        case PrimitiveType::PRIMITIVE_SPHERE:
//            m_sphere->draw();
//            break;
//        }
//    }
//    int size = m_primitives.size();
//    for(int i = 0; i < size; i++){
//        CS123ScenePrimitive p = m_primitives[i];
//        glm::mat4 t = m_trans[i];
//        m_phongShader->applyMaterial(p.material);
//        m_phongShader->setUniform("m", t);
//        switch(p.type){
//        case PrimitiveType::PRIMITIVE_CUBE:
//            m_cube->draw();
//            break;
//        case PrimitiveType::PRIMITIVE_CONE:
//            m_cone->draw();
//            break;
//        case PrimitiveType::PRIMITIVE_CYLINDER:
//            m_cylinder->draw();
//            break;
//        case PrimitiveType::PRIMITIVE_SPHERE:
//            m_sphere->draw();
//            break;
//        }
//    }

    CS123ScenePrimitive leave = getLeave();
    CS123ScenePrimitive branch = getBranch();
    m_phongShader->applyMaterial(branch.material);
    m_phongShader->setUniform("m", glm::mat4());
    m_tree->drawLeave();


    m_phongShader->applyMaterial(leave.material);
    m_phongShader->setUniform("m", glm::mat4());
    m_tree->drawBranch();

//    CS123ScenePrimitive leave = getLeave();
//    m_phongShader->applyMaterial(leave.material);
//    m_phongShader->setUniform("m", glm::mat4());
//    m_tree->drawLeave();

//    CS123ScenePrimitive branch = getBranch();
//    m_phongShader->applyMaterial(branch.material);
//    m_phongShader->setUniform("m", glm::mat4());
//    m_tree->draw();


}

