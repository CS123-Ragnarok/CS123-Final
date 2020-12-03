/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <iostream>


CamtransCamera::CamtransCamera()
{
    // @TODO: [CAMTRANS] Fill this in...
    m_near = 1.0f;
    m_far = 30.0f;
    m_aspectRatio = 1.0f;
    m_thetaH = glm::radians(60.0f);
    m_thetaW = m_thetaH;
    m_thetaW_ratio = glm::tan(m_thetaW / 2);
    m_eye = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);
    m_up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    m_w = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    m_w = glm::vec4(glm::normalize(m_w.xyz()), 0.0f);
    m_v = m_up - glm::dot(glm::vec3(m_up), glm::vec3(m_w)) * m_w;
    m_v = glm::vec4(glm::normalize(glm::vec3(m_v)), 0.0f);
    m_u = glm::vec4(glm::cross(glm::vec3(m_v), glm::vec3(m_w)), 0.0f);
    //updateViewMatrix();
    //updateProjectionMatrix();
    //updateProjectionMatrix();
    //orientLook(glm::vec4(2.0f, 2.0f, 2.0f, 1.0f), glm::vec4(-1.0f,-1.0f,-1.0f,0.0f),glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
}

void CamtransCamera::setAspectRatio(float a)
{
    // @TODO: [CAMTRANS] Fill this in...
    m_aspectRatio = a;
    m_thetaW_ratio = m_aspectRatio * glm::tan(m_thetaH / 2);
    updateProjectionMatrix();
}

glm::vec4 CamtransCamera::getU(){
    return m_u;
}
glm::vec4 CamtransCamera::getV(){
    return m_v;
}
glm::vec4 CamtransCamera::getW(){
    return m_w;
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_perspectiveTransformation * m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_rotationMatrix * m_translationMatrix;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const {
    // @TODO: [CAMTRANS] Fill this in...
    return -m_w;
}

glm::vec4 CamtransCamera::getUp() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_up;
}

float CamtransCamera::getAspectRatio() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_thetaH;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    // @TODO: [CAMTRANS] Fill this in...
    m_eye = eye;
    m_up = up;
    m_w = -look;
    m_w = glm::vec4(glm::normalize(m_w.xyz()), 0.0f);
    m_v = m_up - glm::dot(glm::vec3(m_up), glm::vec3(m_w)) * m_w;
    m_v = glm::vec4(glm::normalize(glm::vec3(m_v)), 0.0f);
    m_u = glm::vec4(glm::cross(glm::vec3(m_v), glm::vec3(m_w)), 0.0f);
    updateViewMatrix();
    updateProjectionMatrix();
}

void CamtransCamera::setHeightAngle(float h) {
    // @TODO: [CAMTRANS] Fill this in...
    h = glm::radians(h);
    m_thetaH = h;
    m_thetaW_ratio = m_aspectRatio * glm::tan(m_thetaH / 2);
    updateProjectionMatrix();
}

void CamtransCamera::translate(const glm::vec4 &v) {
    // @TODO: [CAMTRANS] Fill this in...
    m_eye = m_eye + v;
    m_eye.w = 0.0f;
    updateViewMatrix();
}

void CamtransCamera::rotateU(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    float d = glm::radians(degrees);
    glm::vec4 old_v = m_v;
    glm::vec4 old_w = m_w;
    m_v = glm::cos(d) * old_v - glm::sin(d) * old_w;
    m_w = glm::sin(d) * old_v + glm::cos(d) * old_w;
    updateViewMatrix();
}

void CamtransCamera::rotateV(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    float d = glm::radians(degrees);
    glm::vec4 old_w = m_w;
    glm::vec4 old_u = m_u;
    m_w = glm::cos(d) * old_w - glm::sin(d) * old_u;
    m_u = glm::sin(d) * old_w + glm::cos(d) * old_u;
    updateViewMatrix();
}

void CamtransCamera::rotateW(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    float d = glm::radians(degrees);
    glm::vec4 old_u = m_u;
    glm::vec4 old_v = m_v;
    m_u = glm::cos(d) * old_u - glm::sin(d) * old_v;
    m_v = glm::sin(d) * old_u + glm::cos(d) * old_v;
    updateViewMatrix();
}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    // @TODO: [CAMTRANS] Fill this in...
    m_near = nearPlane;
    m_far = farPlane;
    updateProjectionMatrix();
}

void CamtransCamera::updateProjectionMatrix(){
    updatePerspectiveMatrix();
    updateScaleMatrix();
}

void CamtransCamera::updatePerspectiveMatrix(){
    float c = -m_near / m_far;
    m_perspectiveTransformation = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f / (1 + c), -c / (1 + c),
                              0.0f, 0.0f, -1.0f, 0.0f);
    m_perspectiveTransformation = glm::transpose(m_perspectiveTransformation);
}
void CamtransCamera::updateScaleMatrix(){
    m_scaleMatrix = glm::mat4(1 / m_far / m_thetaW_ratio, 0.0f, 0.0f, 0.0f,
                                            0.0f, 1 / m_far / glm::tan(m_thetaH / 2), 0.0f, 0.0f,
                                            0.0f, 0.0f, 1 / m_far, 0.0f,
                                            0.0f, 0.0f, 0.0f, 1.0f);
    m_scaleMatrix = glm::transpose(m_scaleMatrix);
}
void CamtransCamera::updateViewMatrix(){
    updateRotationMatrix();
    updateTranslationMatrix();
}
void CamtransCamera::updateRotationMatrix(){
    m_rotationMatrix = glm::mat4(m_u[0], m_u[1], m_u[2], 0.0f,
                                 m_v[0], m_v[1], m_v[2], 0.0f,
                                 m_w[0], m_w[1], m_w[2], 0.0f,
                                 0.0f  ,   0.0f,   0.0f, 1.0f);
    m_rotationMatrix = glm::transpose(m_rotationMatrix);
}
void CamtransCamera::updateTranslationMatrix(){
    m_translationMatrix = glm::mat4(1.0f, 0.0f, 0.0f, -m_eye[0],
                                    0.0f, 1.0f, 0.0f, -m_eye[1],
                                    0.0f, 0.0f, 1.0f, -m_eye[2],
                                    0.0f, 0.0f, 0.0f, 1.0f);
    m_translationMatrix = glm::transpose(m_translationMatrix);
}
