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

    m_tree = std::make_unique<MeshGenerator>(3);
    //m_tree->m_lsystem->add_rules('F', "F[Fz[zFZXFZYF]Z[ZFxzFyzF]C+]");
    //m_tree->m_lsystem->add_rules('F', "F[Fz[zFZXFZYF]Z[ZFxzFyzF]+]");
    //m_tree->m_lsystem->add_rules('R', "FFF[FXYZ[FxRxF[zFRzXFC]R[ZFZyFC]]yFRyF]");
    //m_tree->m_lsystem->add_rules('B', "XXYYYYYYYYFRFzzFRRC");
    m_tree->GenerateMesh("+TT+R", 3, glm::vec3(0.0f), 0.05f);
    m_terrain = std::make_unique<Terrain>();
    m_terrain->init();
    //paintTrees();
    m_testLight.id = 0;
    m_testLight.type = LightType::LIGHT_DIRECTIONAL;
    m_testLight.dir = glm::normalize(glm::vec4(1.0f, -1.0f, 1.0f, 0.0f));
    m_testLight.color = glm::vec4(1.0f);


    m_trees.reserve(25);
    for(int i = 0; i < 25; i++){
        m_trees.push_back(std::make_unique<MeshGenerator>(i));
        m_trees[i]->GenerateMesh("+TT+R", 3, glm::vec3(0.0f), 0.05f);
    }

    m_sphere = std::make_unique<SphereShape>(20, 20);

    m_snow.reserve(1000);
    for(int i = 0; i < 1000; i++){
        struct Snow temp;
        temp.flag = false;
        m_snow.push_back(temp);
    }


//    m_timer.start(1000.0f / m_fps);
//    m_increment = 0;
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
    res.material.cAmbient = glm::vec4(93.0f / 255.0f, 67.0f / 255.0f, 44.0f / 255.0f, 1.0f);
    res.material.cDiffuse = glm::vec4(0.5f);
    res.material.cSpecular = glm::vec4(0.1f);
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

CS123ScenePrimitive SceneviewScene::getSnow(){
    CS123ScenePrimitive res;
    res.type = PrimitiveType::PRIMITIVE_CYLINDER;
    res.material.cAmbient = glm::vec4(255.0f / 255.0f, 250.0f / 255.0f, 250.0f / 255.0f, 1.0f);
    res.material.cDiffuse = glm::vec4(0.75f);
    res.material.cSpecular = glm::vec4(0.75f);
    res.material.shininess = 2.0f;
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

void SceneviewScene::render(Camera * camera, int time, int mili) {
    m_time = time;
    glClearColor(53.f/255.f, 81.f/255.f, 92.f/255.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_phongShader->bind();
    setSceneUniforms(camera);
    setLights();

    srand(time);

    //std::cout << time << std::endl;

    for(int i = 0; i < 1000; i++){
        if(!m_snow[i].flag){
            int lottery = rand() % total;
            if(lottery == 8){
                m_snow[i].flag = true;
                m_snow[i].x = rand() % 100;
                m_snow[i].z = rand() % 100;
                m_snow[i].start = time;
                m_snow[i].current_height = y_start;
            }
        } else {
            int time_interval = time - m_snow[i].start;
            if(time_interval < 0){
                time_interval += 60;
            }
            m_snow[i].current_height = y_start - speed * (float)(time_interval) - speed * (mili) / 1000.0f;
            float height_limit = m_terrain->getPosition(m_snow[i].x, m_snow[i].z).y - 1.5f;
            if(m_snow[i].current_height < height_limit){
                m_snow[i].flag = false;
            }
        }
    }
    if(total != 10){
        total = 600 - 10 * time;
    }

    renderGeometry();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_phongShader->unbind();

}

void SceneviewScene::setSceneUniforms(Camera * camera) {
    m_phongShader->setUniform("useLighting", true);
    m_phongShader->setUniform("p" , camera->getProjectionMatrix());
    m_phongShader->setUniform("v", camera->getViewMatrix());
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

    float scale = 1.0f / 10.0f;
    CS123ScenePrimitive branch = getBranch();
    m_phongShader->applyMaterial(branch.material);

    for(int i = 1; i < 5; i++){
        for(int j = 1; j < 5; j++){
            int row = i * 20;
            int col = j * 20;
            int index = i * 5 + j;
            glm::vec3 pos = m_terrain->getPosition(row, col);
            glm::vec3 nor = m_terrain->getNormal(row, col);
            pos += glm::vec3(0.0f, -1.5f, 0.0f);
            glm::vec3 y = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 d = nor.xyz();
            glm::vec3 a = glm::cross(d, y);

            float omega = glm::acos(glm::dot(y, d));
            glm::mat4 m = glm::translate(pos) * glm::scale(glm::vec3(1/10.f, 1/10.f, 1/10.f));
            m_phongShader->setUniform("m", m);
            //m_tree->drawLeave();
            //m_tree->drawBranch();
            m_trees[index]->draw();
        }
    }
    //CS123ScenePrimitive leave = getLeave();


    m_phongShader->applyMaterial(getSnow().material);
    for(int i = 0; i< 1000; i++){
        if(m_snow[i].flag){
            glm::mat4 m = glm::translate(glm::vec3((float)m_snow[i].x / 10.0f - 5.0f, m_snow[i].current_height,(float)m_snow[i].z / 10.0f - 5.0f)) * glm::scale(glm::vec3(1/20.f));
            m_phongShader->setUniform("m", m);
            m_sphere->draw();
        }
    }


    CS123SceneMaterial topmaterial;
    //topmaterial.cAmbient = glm::vec4(37.0f / 255.0f, 69.0f / 255.0f, 18.0f / 255.0f, 1.0f);
    topmaterial.cAmbient = glm::vec4(140.0f / 255.0f, 139.0f / 255.0f, 146.0f / 255.0f, 1.0f);
    topmaterial.cDiffuse = glm::vec4(0.5f);
    topmaterial.cSpecular = glm::vec4(0.3f);
    topmaterial.shininess = 1.0f;
    //shader->applyMaterial(topmaterial);
    m_phongShader->applyMaterial(topmaterial);

    m_phongShader->setUniform("m", glm::translate(glm::mat4x4(1.0), glm::vec3(0.f, -1.5f, 0.f)));

    m_terrain->drawTop();

    CS123SceneMaterial sidematerial;
    sidematerial.cAmbient = glm::vec4(64.0f / 255.0f, 41.0f / 255.0f, 5.0f / 255.0f, 1.0f);
    sidematerial.cDiffuse = glm::vec4(0.2f);
    sidematerial.cSpecular = glm::vec4(0.f);
    sidematerial.shininess = 1.0f;
    m_phongShader->applyMaterial(sidematerial);

    m_terrain->drawElse();


//    CS123ScenePrimitive leave = getLeave();
//    m_phongShader->applyMaterial(leave.material);
//    m_phongShader->setUniform("m", glm::mat4());
//    m_tree->drawLeave();

//    CS123ScenePrimitive branch = getBranch();
//    m_phongShader->applyMaterial(branch.material);
//    m_phongShader->setUniform("m", glm::mat4());
//    m_tree->draw();


}

