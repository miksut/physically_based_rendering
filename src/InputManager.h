//
//  InputManager.hpp
//  CGL
//
//  Created by Alireza Amiraghdam on 04.06.19.
//

#ifndef InputManager_hpp
#define InputManager_hpp

#include <stdio.h>
#include "GLIncludes.h"
#include <glm/glm.hpp>
#include <vector>
namespace cgCourse
{
    class InputAcceptor{
    public:
        virtual void mouseDrag(glm::dvec2 _p1, glm::dvec2 _p2, glm::dvec2 _base, int _button) = 0;
        virtual void mouseScroll(glm::dvec2 _offset) = 0;
    };
    class InputManager{
    public:
        static void keyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
        static void mouseButtonCallback(GLFWwindow* _window, int _button, int _action, int _mods);
        static void cursorPosCallback(GLFWwindow * _window, double _xpos, double _ypos);
        static void mouseScrollCallback(GLFWwindow* _window, double _xoffset, double _yoffset);
        static void registerInputAcceptor(InputAcceptor*);
        static glm::dvec2 lastMousePosition;
        static glm::dvec2 clickMousePosition;
        static glm::dvec2 lastProcessedMousePosition;
        static int mouseState;
        static int activeButton;
        static std::vector<InputAcceptor*> inputAcceptors;
        static bool consumed;
    };
    
    
    
}
#endif /* InputManager_hpp */
