#include "GLApp.h"
#include <glm/vec2.hpp>
#include <iostream>
#include <fstream>
#include "InputManager.h"

#ifndef GLFW_TRUE
#   define GLFW_TRUE    GL_TRUE
#endif
#ifndef GLFW_FALSE
#   define GLFW_FALSE   GL_FALSE
#endif

namespace cgCourse
{

    std::string GLApp::pathToExecutable;
	GLApp::GLApp(glm::uvec2 _windowSize, std::string _title, std::string _exepath, bool _resizeable)
		: title(_title)
	{
        pathToExecutable = _exepath;
		// GLFW initialization
        if (!glfwInit()) {
            std::cout << "Failed to initialize GLFW\n" << std::endl;
        }

		// OpenGL context initialization
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, _resizeable ? GLFW_TRUE : GLFW_FALSE);

		// Window and OpenGL context creation
		window_ = glfwCreateWindow(int(_windowSize.x), int(_windowSize.y), title.c_str(),
			NULL, NULL);
		assert(window_);

		glfwSetWindowPos(window_, 64, 64);
		glfwSetWindowUserPointer(window_, this);
		glfwSetInputMode(window_, GLFW_STICKY_KEYS, 1);
		glfwSetKeyCallback(window_, InputManager::keyCallback);
		glfwSetMouseButtonCallback(window_, InputManager::mouseButtonCallback);
        glfwSetCursorPosCallback(window_, InputManager::cursorPosCallback);
        glfwSetScrollCallback(window_, InputManager::mouseScrollCallback);

		// Load OpenGL core profile functions using gl3w
		glfwMakeContextCurrent(window_);
		glfwSwapInterval(0);

		// Initialize GLEW
        if (glewInit() != GLEW_OK) {
            std::cout << "Failed to initialize GLEW\n" << std::endl;
        }
        
		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	}

	GLApp::~GLApp()
	{
		if (window_)
		{
			glfwDestroyWindow(window_);
			window_ = 0;
		}
		glfwTerminate();
	}

	bool GLApp::run()
	{
		// Call custom application initialization 
		if (!init())
			return false;
        double t;
        double dt;
        const double FPSInterval = 0.5;
		// Render loop: process input handlers and call custom rendering
		while (!glfwWindowShouldClose(window_))
		{
            t = glfwGetTime();
            dt = t - previousTime;
            if (dt>0.008){
                glfwPollEvents();
                previousTime = t;
                if (t - previousFPSTime <FPSInterval){
                    frameTick++;
                }else{
                    setTitle(std::to_string((double)frameTick/FPSInterval));
                    frameTick = 0;
                    previousFPSTime = t;
                }
                if (!this->update(dt))
                    return false;
                if (!this->render())
                    return false;
                glfwSwapBuffers(window_);
            }
		}

		// Call custom application cleaning and termination
		return end();
	}

	void GLApp::stop()
	{
		glfwSetWindowShouldClose(window_, GL_TRUE);
	}

    glm::uvec2 GLApp::getFramebufferSize() const
    {
        glm::ivec2 sizes(0);
        glfwGetFramebufferSize(window_, &sizes.x, &sizes.y);
        return glm::uvec2(sizes);
    }
    glm::uvec2 GLApp::getWindowSize() const
    {
        glm::ivec2 sizes(0);
        glfwGetWindowSize(window_, &sizes.x, &sizes.y);
        return glm::uvec2(sizes);
    }

	bool GLApp::checkGlError(const char* _message) const
	{
		int error = glGetError();

		if (error != GL_NO_ERROR)
		{
			std::string errorString;
			switch (error)
			{
			case GL_INVALID_ENUM:
				errorString = "GL_INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				errorString = "GL_INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				errorString = "GL_INVALID_OPERATION";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;
			case GL_OUT_OF_MEMORY:
				errorString = "GL_OUT_OF_MEMORY";
				break;
			default:
				errorString = "UNKNOWN";
				break;
			}
			std::cerr << "[" << _message << "] OpenGL Error(" << "): " << errorString
				<< std::endl;
		}
		return error == GL_NO_ERROR;
	}
    
    std::string GLApp::getPathToExecutable(){
        return pathToExecutable;
    }
    void GLApp::setTitle(std::string _title){
        glfwSetWindowTitle(window_, _title.c_str());
    }

}
