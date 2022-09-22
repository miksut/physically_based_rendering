//
//  FrameBuffer.cpp
//  CGL
//
//  Created by Alireza Amiraghdam on 03.06.19.
//

#include "FrameBuffer.h"

namespace cgCourse
{

    void bindDefaultFrameBuffer(){
        glBindFramebuffer(GL_FRAMEBUFFER,0);
    }
    
    void clearDefaultFrameBuffer(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void FrameBuffer::create(const glm::uvec2 &_size, unsigned int _outputNum){
        size = _size;
        
        glGenFramebuffers(1,&fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        
        unsigned int  renderedTexture;
        for (int i = 0; i < _outputNum; i++){
            glGenTextures(1, &renderedTexture);
        
            // "Bind" the newly created texture : all future texture functions will modify this texture
            glBindTexture(GL_TEXTURE_2D, renderedTexture);
        
            // Give an empty image to OpenGL ( the last "0" )
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA32F, _size.x , _size.y, 0,GL_RGBA, GL_FLOAT, 0);
            
            // Poor filtering. Needed !
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            renderedTextures.push_back(renderedTexture);
        }
        glGenTextures(1, &depthtexture);
        glBindTexture(GL_TEXTURE_2D, depthtexture);
        glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, _size.x,_size.y, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthtexture, 0);
        
        // Set "renderedTexture" as our colour attachement #0
        GLenum DrawBuffers[10];
        for (int i = 0; i < _outputNum; i++){
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, renderedTextures[i], 0);
            DrawBuffers[i] = (GL_COLOR_ATTACHMENT0+i);
        }
        // Set the list of draw buffers.
        glDrawBuffers(_outputNum, DrawBuffers); 
    }
    void FrameBuffer::bind(){
                glBindFramebuffer(GL_FRAMEBUFFER,fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    std::vector<unsigned int> &FrameBuffer::getRenderTextures(){
        return renderedTextures;
    }
    unsigned int &FrameBuffer::getDepthTexture(){
        return depthtexture;
    }
    void FrameBuffer::copyToDefault(){
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
        glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }

}
