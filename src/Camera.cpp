#include "Camera.h"
#include <iostream>

// to get M_PI from math libary enable defines
#define _USE_MATH_DEFINES
#include <math.h>

namespace cgCourse
{
    Camera::Camera()
    {
        
    }
    void Camera::create(glm::uvec2 _viewPortSize,glm::uvec2 _windowSize, glm::vec3 _pos, glm::vec3 _target, glm::vec3 _up, float _fieldOfView)
    {
        fieldOfView = _fieldOfView;
        create(_viewPortSize,_windowSize,_pos,_target,_up);
    }
    void Camera::create(glm::uvec2 _viewPortSize,glm::uvec2 _windowSize, glm::vec3 _pos, glm::vec3 _target, glm::vec3 _up)
    {
        windowSize = _windowSize;
        this->setViewport(_viewPortSize);
        this->lookAt(_pos, _target, _up);
    }
    
    void Camera::lookAt(glm::vec3 _pos, glm::vec3 _target, glm::vec3 _up){
        this->position = _pos;
        this->target = _target;
        glm::vec3 v = target - position;
        glm::vec3 right = glm::cross(v,_up);
        this->up = glm::normalize(glm::cross(right, v));
        
        computeViewMatrix();
        computeProjectionMatrix();
        computeViewProjectionMatrix();
    }
    
    void Camera::setViewport(glm::uvec2 _viewPortSize)
    {
        viewPortSize = _viewPortSize;
        glViewport(0, 0, int(viewPortSize.x), int(viewPortSize.y));
    }
    
    void Camera::computeViewProjectionMatrix()
    {
        this->viewProjectionMatrix = projectionMatrix * viewMatrix;
    }
    
    void Camera::computeViewMatrix()
    {
        
        this->viewMatrix = glm::lookAt(position,target,up);
    }
    
    void Camera::computeProjectionMatrix()
    {
        float aspect = float(viewPortSize.x) / float(viewPortSize.y);
        
        this->projectionMatrix = glm::perspective(fieldOfView, aspect,
                                                  nearPlane, farPlane);
    }
    void Camera::setPosition(glm::vec3 _pos){
        position = _pos;
        computeViewMatrix();
        computeViewProjectionMatrix();
    }
    
    glm::vec3 Camera::getPosition() const
    {
        return this->position;
    }
    
    glm::mat4 Camera::getViewMatrix() const
    {
        return this->viewMatrix;
    }
    
    glm::mat4 Camera::getProjectionMatrix() const
    {
        return this->projectionMatrix;
    }
    
    glm::mat4 Camera::getViewProjectionMatrix() const
    {
        return this->viewProjectionMatrix;
    }
    
    void Camera::simpleRotate(glm::vec2 _v){
        glm::vec3 v = position - target;
        glm::mat3 rotMat = glm::rotate(glm::mat4(1), -_v.x / cameraMouseIntensity, glm::vec3(0,1,0));
        glm::vec3 newv = rotMat * v;
        glm::vec3 newup = rotMat * up;
        up = newup;
        position = target + newv;
        
        double currentAngle =  glm::dot(glm::normalize(newv), glm::vec3(0,1,0));
        float newAngle = currentAngle - _v.y / cameraMouseIntensity;
        newAngle = glm::clamp(newAngle, -1.f, 1.f);
        if (currentAngle != newAngle){
            glm::vec3 projectedV = newv;
            projectedV.y = 0;
            glm::vec3 projectedUp = newup;
            projectedUp.y = 0;
            glm::vec3 dir;
            if (glm::length(projectedV) > glm::length(projectedUp)){
                dir = projectedV;
            }else{
                dir = (newAngle >= 0 ? -1.0f : 1.0f) * projectedUp;
            }
            orientByAngleDirectionDistance(newAngle, dir, glm::length(newv));
        }
        
        computeViewMatrix();
        computeViewProjectionMatrix();
    }
    
    void Camera::arcballScroll(glm::dvec2 _offset){
        glm::vec3 v = position - target;
        glm::vec3 newv = (1.0f + (float) - _offset.y / cameraMouseIntensity) * v;
        position = target + newv;
        computeViewMatrix();
        computeViewProjectionMatrix();
        
    }
    
    void Camera::arcballPan(glm::dvec2 _vector){
        glm::vec3 v = position - target;
        glm::vec3 r = glm::cross(up , v);
        r = glm::normalize(r);
        glm::vec3 move = (float)-_vector.x/ cameraMouseIntensity * r + (float)_vector.y/ cameraMouseIntensity * up;
        position = position + move;
        target = target + move;
        computeViewMatrix();
        computeViewProjectionMatrix();
    }
    
    void Camera::flyRotate(glm::vec2 _vector){
        glm::vec3 v = target - position;
        glm::mat3 rotMat = glm::rotate(glm::mat4(1), -_vector.x / cameraMouseIntensity, glm::vec3(0,1,0));
        glm::vec3 newv = rotMat * v;
        glm::vec3 newup = rotMat * up;
        glm::vec3 r = glm::cross(newv,newup);
        rotMat = glm::rotate(glm::mat4(1), -_vector.y / cameraMouseIntensity, r);
        newv = rotMat * newv;
        newup = rotMat * newup;
        up = newup;
        target = position + newv;
        computeViewMatrix();
        computeViewProjectionMatrix();
    }
    void Camera::flyPan(glm::vec2 _vector){
        glm::vec3 v = position - target;
        glm::vec3 r = glm::cross(up , v);
        r = glm::normalize(r);
        r[1] = 0;
        glm::vec3 projectedV = normalize(v);
        projectedV[1] = 0;
        glm::vec3 move = (float)-_vector.x/ cameraMouseIntensity * r + (float)+_vector.y/ cameraMouseIntensity * up;
        position = position + move;
        target = target + move;
        computeViewMatrix();
        computeViewProjectionMatrix();
    }
    void Camera::flyScroll(glm::dvec2 _offset){
        glm::vec3 v =  target  -  position;
        glm::vec3 newv = ((float)  _offset.y / cameraMouseIntensity) * 10.0f* glm::normalize(v);
        position = position + newv;
        target = target + newv;
        computeViewMatrix();
        computeViewProjectionMatrix();
        
    }
    
    void Camera::arcballRotate(glm::vec2 _p1, glm::vec2 _p2){
        glm::vec2 center = glm::vec2(windowSize)/2.0f;
        float r = (windowSize.y/2.0f)*0.8;
        glm::vec2 cp1 = _p1 - center;
        glm::vec2 cp2 = _p2 - center;
        cp1.x = - cp1.x;
        cp2.x = - cp2.x;
        
        cp1 = cp1 / r;
        cp2 = cp2 /r;
        float z1sq = 1- cp1.x*cp1.x - cp1.y*cp1.y;
        float z2sq = 1- cp2.x*cp2.x - cp2.y*cp2.y;
        
        glm::vec3 p1_z,p2_z;
        if (z1sq>0){
            p1_z = glm::vec3(cp1.x,cp1.y, sqrtf(z1sq));
        }else{
            p1_z = glm::vec3(glm::sign(cp1.x)*sqrtf(cp1.x*cp1.x/(cp1.x*cp1.x+cp1.y*cp1.y)),glm::sign(cp1.y)*sqrtf(cp1.y*cp1.y/(cp1.x*cp1.x+cp1.y*cp1.y)), 0);
        }
        if (z2sq>0){
            p2_z = glm::vec3(cp2.x,cp2.y, sqrtf(z2sq));
        }else{
            p2_z = glm::vec3(glm::sign(cp2.x)*sqrtf(cp2.x*cp2.x/(cp2.x*cp2.x+cp2.y*cp2.y)),glm::sign(cp2.y)*sqrtf(cp2.y*cp2.y/(cp2.x*cp2.x+cp2.y*cp2.y)), 0);
        }
        p1_z = glm::inverse(glm::mat3(viewMatrix)) * p1_z;
        p2_z = glm::inverse(glm::mat3(viewMatrix)) * p2_z;
        p1_z = glm::normalize(p1_z);
        p2_z = glm::normalize(p2_z);
        glm::vec3 axis = glm::cross(p1_z,p2_z);
        
        float angle = acos(glm::dot(p1_z,p2_z));
        if (abs(angle)>0.0005){
            axis = glm::normalize(axis);
            
            auto v = position - target;
            glm::mat3 rotMat = glm::rotate(glm::mat4(1),glm::sign(axis.y)*(glm::clamp(abs(angle * glm::dot(axis,glm::vec3(0,1,0))),0.0f,1.0f)) , glm::vec3(0,1,0));
            glm::vec3 newv = rotMat * v;
            glm::vec3 newup = rotMat * up;
            auto right = glm::normalize(glm::cross(newv,newup));
            
            rotMat = glm::rotate(glm::mat4(1),glm::sign(glm::dot(axis,right))*(glm::clamp(abs(angle * glm::dot(axis,right)),0.0f,1.0f)) , right);
            newv = rotMat * newv;
            newup = rotMat * newup;
            up = newup;
            position = target + newv;
            
        }
        computeViewMatrix();
        computeViewProjectionMatrix();
    }
    
    
    void Camera::mouseDrag(glm::dvec2 _p1, glm::dvec2 _p2, glm::dvec2 _base, int _activeButton){
        switch (_activeButton){
            case 0:
                switch (type) {
                    case 0:
                        arcballRotate(_p1, _p2);
                        break;
                    case 1:
                        flyPan(_p2-_p1);
                        break;
                }
                break;
            case 1:
                switch (type) {
                    case 0:
                        flyPan(_p2-_p1);
                        break;
                    case 1:
                        flyRotate(_p2-_p1);
                        break;
                }
                break;
                
        }
    }
    void Camera::mouseScroll(glm::dvec2 _offset){
        switch (type) {
            case 0:
                arcballScroll(_offset);
                break;
            case 1:
                flyScroll(_offset);
                break;
        }
    }
    int &Camera::getType(){
        return type;
    }
    
    void Camera::orientByAngleDirectionDistance(float _angle, glm::vec3 _horizontalDirection,float _distance){
        glm::vec3 dir = _horizontalDirection;
        glm::vec3 v = position - target;
        dir.y = 0;
        float a = sqrt((1 - _angle * _angle) / (dir.x * dir.x + dir.z * dir.z));
        dir = a * dir;
        dir.y = _angle;
        dir = _distance * glm::normalize(dir);
        glm::vec3 r = glm::normalize(glm::cross(up, v));
        position = target + dir;
        up = glm::cross(dir, r);
    }
    
}

