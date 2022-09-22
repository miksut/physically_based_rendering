//
//  Gui.hpp
//  CGL
//
//  Created by Alireza Amiraghdam on 03.06.19.
//

#ifndef Gui_hpp
#define Gui_hpp

#include <stdio.h>
#include <map>
#include "GLIncludes.h"
namespace cgCourse
{
    void initGui(GLFWwindow* _window);
    void updateGui();
    void renderGui();
    static std::map<std::string, void *> vars;
    extern void connectVar(std::string _varName, void * _varAddress);
    extern void *getVar(std::string _varName);
    extern bool inputAccept;

}
#endif /* Gui_hpp */
