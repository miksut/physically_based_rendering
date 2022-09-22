//
//  DrawableShape.hpp
//  CGL
//
//  Created by Alireza Amiraghdam on 27.05.19.
//

#ifndef DrawableShape_hpp
#define DrawableShape_hpp

#include <stdio.h>
#include "Drawable.h"
#include "Shape.h"


namespace cgCourse
{
    class DrawableShape: public Drawable,  public Shape{
    public:
        const std::vector< glm::vec3 >& getPositions() const override;
        const std::vector< glm::vec3 >& getNormals() const override;
        const std::vector< glm::uvec3 >& getFaces() const override;
        const std::vector< glm::vec3 >& getFaceNormals() const override;
        const std::vector< glm::vec3 >& getTangents() const override;
        const std::vector< glm::vec3>& getColors() const override;
        const std::vector< glm::vec2>& getTexCoords() const override;
        const std::shared_ptr<Material>& getMaterial() const override;
        const glm::mat4& getModelMatrix() const override;
        virtual GLsizei getDrawElemCount() const override;
        const std::vector< glm::uvec2>& getLineIndices() const override;
    };
    
    
    
}
#endif /* DrawableShape_hpp */
