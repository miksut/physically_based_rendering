#ifndef Example_Camera_h
#define Example_Camera_h

#include "GLIncludes.h"
#include <glm/gtc/matrix_transform.hpp>
#include "InputManager.h"

namespace cgCourse
{
    class Camera : public InputAcceptor
	{
	public:
		Camera();

        void create(glm::uvec2 _viewPortSize,glm::uvec2 _windowSize, glm::vec3 _pos, glm::vec3 _target, glm::vec3 _up);
        void create(glm::uvec2 _viewPortSize,glm::uvec2 _windowSize, glm::vec3 _pos, glm::vec3 _target, glm::vec3 _up, float _fieldOfView);

		void setViewport(glm::uvec2 _extent);
        void lookAt(glm::vec3 _pos, glm::vec3 _target, glm::vec3 _up);
        void setPosition(glm::vec3 _pos);

		glm::vec3 getPosition() const;
		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjectionMatrix() const;
		glm::mat4 getViewProjectionMatrix() const;
        void simpleRotate(glm::vec2 _vector);
        void arcballScroll(glm::dvec2 _offset);
        void arcballPan(glm::dvec2 _vector);
        void flyRotate(glm::vec2 _vector);
        void flyPan(glm::vec2 _vector);
        void flyScroll(glm::dvec2 _offset);
        void arcballRotate(glm::vec2 _p1, glm::vec2 _p2);


        void mouseDrag(glm::dvec2 _p1, glm::dvec2 _p2, glm::dvec2 _base, int _activeButton) override;
        void mouseScroll(glm::dvec2 _offset) override;
        int &getType();

    private:
		void computeViewProjectionMatrix();
		void computeProjectionMatrix();
		void computeViewMatrix();
        void orientByAngleDirectionDistance(float _angle, glm::vec3 _horizontalDirection,float _distance);

		glm::vec3 position;
		glm::vec3 target;
		glm::vec3 up;
        glm::uvec2 viewPortSize;
        glm::uvec2 windowSize;

		glm::mat4 viewMatrix=glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
		glm::mat4 projectionMatrix=glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
		glm::mat4 viewProjectionMatrix=glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
        
        const float cameraMouseIntensity = 100;
        
        int type = 0;
        float fieldOfView = 45.0f;
        float nearPlane = 0.1;
        float farPlane = 50.0;
	};
}

#endif
