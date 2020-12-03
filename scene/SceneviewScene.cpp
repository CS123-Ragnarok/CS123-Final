#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include "camera/Camera.h"


#include "lib/ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"
#include "shapes/ConeShape.h"
#include "shapes/CubeShape.h"
#include "shapes/CylinderShape.h"
#include "shapes/SphereShape.h"

using namespace CS123::GL;


SceneviewScene::SceneviewScene()
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
    loadPhongShader();


    m_cube = std::make_unique<CubeShape>(1, 1);
    m_cone = std::make_unique<ConeShape>(1, 20);
    m_cylinder = std::make_unique<CylinderShape>(1, 20);
    m_sphere = std::make_unique<SphereShape>(20, 20);
}

SceneviewScene::~SceneviewScene()
{
}

void SceneviewScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/shader.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/shader.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
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
//    m_phongShader->setUniform("useLighting", settings.useLighting);
//    m_phongShader->setUniform("useArrowOffsets", false);
//    m_phongShader->setUniform("p" , camera->getProjectionMatrix());
//    m_phongShader->setUniform("v", camera->getViewMatrix());
}

void SceneviewScene::setMatrixUniforms(Shader *shader) {
//    shader->setUniform("p", context->getCamera()->getProjectionMatrix());
//    shader->setUniform("v", context->getCamera()->getViewMatrix());
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

}

