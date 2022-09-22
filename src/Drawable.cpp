//
//  Drawable.cpp
//  CGL
//
//  Created by Alireza Amiraghdam on 17.05.19.
//

#include "Drawable.h"
#include "ShaderProgram.h"
#include "Shape.h"

namespace cgCourse
{
    Drawable::Drawable() :vaoID(0), posBufferID(0), colorBufferID(0), normalBufferID(0), texCoordsBufferID(0),
    indexBufferID(0){
        
    }
    Drawable::~Drawable(){
        if (this->posBufferID != 0)
        {
            glDeleteBuffers(1, &this->posBufferID);
        }
        if (this->colorBufferID != 0)
        {
            glDeleteBuffers(1, &this->colorBufferID);
        }
        if (this->normalBufferID != 0)
        {
            glDeleteBuffers(1, &this->normalBufferID);
        }
        if (this->tangentBufferID != 0)
        {
            glDeleteBuffers(1, &this->tangentBufferID);
        }
        if (this->texCoordsBufferID != 0)
        {
            glDeleteBuffers(1, &this->texCoordsBufferID);
        }
        if (this->indexBufferID != 0)
        {
            glDeleteBuffers(1, &this->indexBufferID);
        }
        if (this->vaoID != 0)
        {
            glDeleteVertexArrays(1, &this->vaoID);
        }
    }
    bool Drawable::createVertexArray(GLuint posAttribLoc,
                                  GLuint colAttribLoc,
                                  GLuint normAttribLoc, GLuint texAttribLoc, GLuint tangentsAttribLoc)
    {
        // check if all buffer locations are somehow defined
        if ((posAttribLoc == GLuint(-1)) ||
            (colAttribLoc == GLuint(-1)) ||
            (normAttribLoc == GLuint(-1))||
            (texAttribLoc == GLuint(-1))||
            (tangentsAttribLoc == GLuint(-1)))
        {
            return false;
        }
        const std::vector< glm::vec3 >& positions = getPositions();
        const std::vector< glm::vec3 >& colors = getColors();
        const std::vector< glm::vec3 >& normals = getNormals();
        const std::vector< glm::vec2 >& texCoords = getTexCoords();
        const std::vector< glm::vec3 >& tangents = getTangents();
        // Initialize Vertex Array Object
        glGenVertexArrays(1, &vaoID);
        glBindVertexArray(vaoID);
        
        // Initialize buffer objects with geometry data
        // for positions
        glGenBuffers(1, &posBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, posBufferID);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3),
                     positions.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(posAttribLoc);
        
        // for colors
        if (colors.size()>0){
            glGenBuffers(1, &colorBufferID);
            glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
            glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3),
                         colors.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(colAttribLoc);
        }
        // for normals
        if(normals.size() != 0)
        {
            glGenBuffers(1, &normalBufferID);
            glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
            glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
                         normals.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(normAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(normAttribLoc);
        }
        
        // for texture coordinates
        if (texCoords.size() != 0)
        {
            glGenBuffers(1, &texCoordsBufferID);
            glBindBuffer(GL_ARRAY_BUFFER, texCoordsBufferID);
            glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2),
                         texCoords.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(texAttribLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(texAttribLoc);
        }
        
        // for texture coordinates
        if (tangents.size() != 0)
        {
            glGenBuffers(1, &tangentBufferID);
            glBindBuffer(GL_ARRAY_BUFFER, tangentBufferID);
            glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3),
                         tangents.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(tangentsAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(tangentsAttribLoc);
        }
        glGenBuffers(1, &indexBufferID);
        switch (primitiveType) {
            case point:
                this->initIndexBufferPoint();
                break;
            case line:
                this->initIndexBuffeLine();
                break;
            case triangle:
                this->initIndexBuffer();
                break;
        }
        // Reset state
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        return true;
    }
    void Drawable::initIndexBuffer() {
        // Initialize buffer objects with index data
        const std::vector< glm::uvec3>& faces = getFaces();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(glm::uvec3),
                     faces.data(), GL_STATIC_DRAW);
    }
    void Drawable::initIndexBufferPoint() {
        // Initialize buffer objects with index data
        const std::vector< glm::vec3 >& positions = getPositions();
        std::vector<unsigned int> indices;
        for (int i = 0;i<positions.size();i++){
            indices.push_back(i);
        }
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                     indices.data(), GL_STATIC_DRAW);
    }
    void Drawable::initIndexBuffeLine(){
        const std::vector< glm::uvec2>& lineIndices = getLineIndices();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getIndexBufferId());
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, lineIndices.size() * sizeof(glm::uvec2),
                     lineIndices.data(), GL_STATIC_DRAW);
    }

    void Drawable::draw(const glm::mat4 &_projectionMatrix, const glm::mat4 &_viewMatrix,std::shared_ptr<ShaderProgram> _shaderProgram, bool _updateVMP, std::shared_ptr<Material> _overrideMaterial ) const
    {
        const std::vector< glm::vec3 >& colors = getColors();
        std::shared_ptr<Material>  material;
        if (_overrideMaterial == nullptr)
            material = getMaterial();
        else
            material = _overrideMaterial;
        if (material != nullptr){
            
            if (material->diffuseTexture != nullptr){
                _shaderProgram->addTexture("diffuseTexture", material->diffuseTexture->getTexHandle());
            }
            if (material->normalTexture != nullptr){
                _shaderProgram->addTexture("normalTexture", material->normalTexture->getTexHandle());
            }
            if (material->specTexture != nullptr){
                _shaderProgram->addTexture("specTexture", material->specTexture->getTexHandle());
            }
            if (material->metalnessTexture != nullptr){
                _shaderProgram->addTexture("metalnessTexture", material->metalnessTexture->getTexHandle());
            }
            if (material->roughnessTexture != nullptr){
                _shaderProgram->addTexture("roughnessTexture", material->roughnessTexture->getTexHandle());
            }

        }
        _shaderProgram->bind();
        if (_updateVMP){
            auto mvpMatrix = _projectionMatrix * _viewMatrix * this->getModelMatrix();
            glUniformMatrix4fv(_shaderProgram->getUniformLocation("modelMatrix"), 1, GL_FALSE, &this->getModelMatrix()[0][0]);
            glUniformMatrix4fv(_shaderProgram->getUniformLocation("viewMatrix"), 1, GL_FALSE, &_viewMatrix[0][0]);
            glUniformMatrix4fv(_shaderProgram->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &mvpMatrix[0][0]);
        }
        if (colors.size()>0){
            glUniform1i(_shaderProgram->getUniformLocation("ifVertexColor"), 1);
        }else{
            glUniform1i(_shaderProgram->getUniformLocation("ifVertexColor"), 0);
        }
        if (material != nullptr){
            glUniform3fv(_shaderProgram->getUniformLocation("mat.ka"), 1, &material->ka[0]);
            glUniform3fv(_shaderProgram->getUniformLocation("mat.kd"), 1, &material->kd[0]);
            glUniform3fv(_shaderProgram->getUniformLocation("mat.ks"), 1, &material->ks[0]);
            
            glUniform1f(_shaderProgram->getUniformLocation("mat.ns"), material->ns);
            if (material->diffuseTexture != nullptr){
                glUniform1i(_shaderProgram->getUniformLocation("mat.ifTextureColor"), 1);
            }else{
                glUniform1i(_shaderProgram->getUniformLocation("mat.ifTextureColor"), 0);
            }
            if (material->normalTexture != nullptr){
                glUniform1i(_shaderProgram->getUniformLocation("mat.hasNormalTexture"), 1);
            }else{
                glUniform1i(_shaderProgram->getUniformLocation("mat.hasNormalTexture"), 0);
            }
            if (material->specTexture != nullptr){
                glUniform1i(_shaderProgram->getUniformLocation("mat.hasSpecTexture"), 1);
            }else{
                glUniform1i(_shaderProgram->getUniformLocation("mat.hasSpecTexture"), 0);
            }
            glUniform1i(_shaderProgram->getUniformLocation("mat.hasObjectColor"), material->hasObjectColor);
            glUniform3fv(_shaderProgram->getUniformLocation("mat.color"),1, &material->color[0]);
            glUniform1f(_shaderProgram->getUniformLocation("mat.illumination"), material->illumination);
        }else{
            glm::vec3 defaultVec(1,1,1);
            glUniformMatrix3fv(_shaderProgram->getUniformLocation("mat.ka"), 1, GL_FALSE, &defaultVec[0]);
            glUniformMatrix3fv(_shaderProgram->getUniformLocation("mat.kd"), 1, GL_FALSE, &defaultVec[0]);
            glUniformMatrix3fv(_shaderProgram->getUniformLocation("mat.ks"), 1, GL_FALSE, &defaultVec[0]);
            
            glUniform1f(_shaderProgram->getUniformLocation("mat.ns"), 1);
            glUniform1i(_shaderProgram->getUniformLocation("mat.ifTextureColor"), 0);
            glUniform1i(_shaderProgram->getUniformLocation("mat.hasObjectColor"), 0);
            glUniform3fv(_shaderProgram->getUniformLocation("mat.color"),1, &defaultVec[0]);
            glUniform1f(_shaderProgram->getUniformLocation("mat.illumination"), 0);
        }

        // bind the Vertex Array Object of the cube and draw the triangles
        glBindVertexArray(this->getVertexArrayId());
        switch (primitiveType) {
            case point:
                glDrawElements(GL_POINTS, this->getDrawElemCount(), GL_UNSIGNED_INT, 0);
                break;
            case line:
                glDrawElements(GL_LINES, this->getDrawElemCount(), GL_UNSIGNED_INT, 0);
                break;
            case triangle:
                glDrawElements(GL_TRIANGLES, this->getDrawElemCount(), GL_UNSIGNED_INT, 0);
                break;
        }
        glBindVertexArray(0);
        _shaderProgram->unbind();
    }
    
    
    
    GLuint Drawable::getIndexBufferId() const{
        return indexBufferID;
    }
    GLuint Drawable::getVertexArrayId() const{
        return vaoID;
    }
    int Drawable::getDrawElemCount() const
    {
        switch (primitiveType) {
            case point:
                return getPositions().size();
                break;
            case line:
                return getLineIndices().size()*2;
                break;
            case triangle:
                return 3 * getFaces().size();
                break;
        }
    }
}
