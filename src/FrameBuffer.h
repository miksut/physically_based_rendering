//
//  FrameBuffer.hpp
//  CGL
//
//  Created by Alireza Amiraghdam on 03.06.19.
//

#ifndef FrameBuffer_hpp
#define FrameBuffer_hpp

#include <stdio.h>
#include "GLIncludes.h"
#include <glm/glm.hpp>
#include <vector>
namespace cgCourse
{
    void bindDefaultFrameBuffer();
    void clearDefaultFrameBuffer();
    
    class FrameBuffer{
    public:
        void create(const glm::uvec2 &_size, unsigned int _outputNum=1);
        void bind();
        void copyToDefault();
        std::vector<unsigned int> &getRenderTextures();
        unsigned int &getDepthTexture();
    private:
        unsigned int fbo;
        std::vector<unsigned int> renderedTextures;
        unsigned int depthtexture;
        glm::uvec2 size;
    };

}
#endif /* FrameBuffer_hpp */
