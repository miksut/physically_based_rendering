//
//  Mesh.hpp
//  CGL
//
//  Created by Alireza Amiraghdam on 17.05.19.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include <string>
#include <map>
#include "DrawableShape.h"
    struct aiScene;
namespace cgCourse{

    struct Material;
   

    class Mesh:  public DrawableShape{
    public:
        void draw(const glm::mat4 &_projectionMatrix,const glm::mat4 &_viewMatrix, std::shared_ptr<ShaderProgram> _shaderProgram, bool _updateVMP= true, std::shared_ptr<Material> _overrideMaterial = nullptr) const override;
        bool load(std::string _path,std::string _fileName, bool _flipNormals, bool _swapYZ, bool _reverseWinding);

    private:
       const aiScene* scene = NULL;
        std::vector<std::shared_ptr<DrawableShape>> elements;
        std::map<std::string, std::shared_ptr<Material>> materials;
    
    };

}
#endif /* Mesh_hpp */
