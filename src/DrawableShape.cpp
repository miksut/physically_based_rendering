//
//  DrawableShape.cpp
//  CGL
//
//  Created by Alireza Amiraghdam on 27.05.19.
//

#include "DrawableShape.h"
namespace cgCourse
{
    
    const std::vector< glm::vec3>& DrawableShape::getPositions() const{
        return Shape::getPositions();
    }
    const std::vector< glm::vec3>& DrawableShape::getNormals() const{
        return Shape::getNormals();
    }
    const std::vector< glm::uvec3>& DrawableShape::getFaces() const{
        return Shape::getFaces();
    }
    const std::vector< glm::vec3>& DrawableShape::getFaceNormals() const{
        return Shape::getFaceNormals();
    }
    const std::vector< glm::vec3>& DrawableShape::getTangents() const{
        return Shape::getTangents();
    }
    const std::vector< glm::vec3>& DrawableShape::getColors() const{
        return Shape::getColors();
    }
    const std::vector< glm::vec2 >& DrawableShape::getTexCoords() const{
        return Shape::getTexCoords();
    }
    const glm::mat4& DrawableShape::getModelMatrix() const{
        return Shape::getModelMatrix();
    }
    const std::shared_ptr<Material>& DrawableShape::getMaterial() const{
        return Shape::getMaterial();
    }
    GLsizei DrawableShape::getDrawElemCount() const{
        return Drawable::getDrawElemCount();
    }
    const std::vector< glm::uvec2>& DrawableShape::getLineIndices() const{
        return Shape::getLineIndices();
    }

    
}
