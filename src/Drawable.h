//
//  Drawable.hpp
//  CGL
//
//  Created by Alireza Amiraghdam on 17.05.19.
//

#ifndef Drawable_hpp
#define Drawable_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "GLIncludes.h"
#include <vector>
#include <memory>

namespace cgCourse
{
    class ShaderProgram;
    struct Material;
    class Drawable{
    public:
        enum PrimitveType{point=0x1,line=0x2,triangle=0x4}primitiveType;
        Drawable();
        ~Drawable();
        virtual void draw(const glm::mat4 &_projectionMatrix,const glm::mat4 &_viewMatrix, std::shared_ptr<ShaderProgram> _shaderProgram, bool _updateVMP = true , std::shared_ptr<Material> _overrideMaterial = nullptr) const;
        bool createVertexArray(GLuint posAttribLoc,
                               GLuint colAttribLoc,
                               GLuint normAttribLoc,
                               GLuint texAttribLoc,
                               GLuint tangentsAttribLoc);
    protected:
        GLuint getIndexBufferId() const;
        GLuint getVertexArrayId() const;
        virtual GLsizei getDrawElemCount() const = 0;
    private:
        GLuint vaoID;
        GLuint posBufferID;
        GLuint colorBufferID;
        GLuint normalBufferID;
        GLuint texCoordsBufferID;
        GLuint indexBufferID;
        GLuint tangentBufferID;
        
        virtual const std::vector< glm::vec3>& getPositions() const = 0;
        virtual const std::vector< glm::vec3>& getNormals() const = 0;
        virtual const std::vector< glm::uvec3>& getFaces() const = 0;
        virtual const std::vector< glm::vec3>& getFaceNormals() const = 0;
        virtual const std::vector< glm::vec3>& getTangents()const  = 0;
        virtual const std::vector< glm::vec3>& getColors() const = 0;
        virtual const std::vector< glm::vec2 >& getTexCoords()const  = 0;
        virtual const glm::mat4& getModelMatrix() const = 0;
        virtual const std::shared_ptr<Material>& getMaterial() const = 0;
        virtual const std::vector<glm::uvec2>& getLineIndices() const = 0;


        virtual void initIndexBuffer();
        virtual void initIndexBufferPoint();
        virtual void initIndexBuffeLine();
    };
    
}
#endif /* Drawable_hpp */
