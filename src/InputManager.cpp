//
//  InputManager.cpp
//  CGL
//
//  Created by Alireza Amiraghdam on 04.06.19.
//

#include "InputManager.h"
#include <iostream>
namespace cgCourse
{
    glm::dvec2 InputManager::lastMousePosition;
    glm::dvec2 InputManager::clickMousePosition;
    glm::dvec2 InputManager::lastProcessedMousePosition;
    int InputManager::mouseState = 0;
    std::vector<InputAcceptor*> InputManager::inputAcceptors;
    bool InputManager::consumed = false;
    int InputManager::activeButton;
    void InputManager::keyCallback(GLFWwindow* _window, int _key, int _scancode,
                                int _action, int _mods)
        {
            if (_key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(_window, GL_TRUE);
            }
        }
        
    void InputManager::mouseButtonCallback(GLFWwindow* _window, int _button,
                                        int _action, int _mods)
        {

            std::cout<<_button<<" "<<_action<<std::endl;
            if (_action == 0){
                mouseState = 0;
            }else if (_action ==1){
                activeButton = _button;
                clickMousePosition = lastMousePosition;
                lastProcessedMousePosition = lastMousePosition;
                mouseState = 1;
            }
        }
        
    void InputManager::cursorPosCallback(GLFWwindow * _window, double _xpos, double _ypos)
        {
            lastMousePosition = glm::dvec2(_xpos,_ypos);
            if (mouseState==1){
                for (auto l:inputAcceptors)
                    if (!consumed)
                        l->mouseDrag(lastProcessedMousePosition,lastMousePosition,clickMousePosition,activeButton);
                lastProcessedMousePosition = lastMousePosition;
            }
        }
    void InputManager::mouseScrollCallback(GLFWwindow* _window, double _xoffset, double _yoffset)
        {
            for (auto l:inputAcceptors)
                if (!consumed)
                    l->mouseScroll(glm::dvec2(_xoffset, _yoffset));

        }
        

     void InputManager::registerInputAcceptor(InputAcceptor* _inputAcceptor){
         inputAcceptors.push_back(_inputAcceptor);
    }


}
