#include "Shape.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace cgCourse
{
	Shape::Shape()
		:  positions(0), colors(0), normals(0), faces(0), faceNormals(0), tangents(0)
	{
		// Model matrix : an identity matrix (model will be at the origin)
		this->modelMatrix = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	}

	void Shape::setPosition(glm::vec3 _pos)
	{
		this->objectPosition = _pos;
        this->translationMatrix = glm::translate(glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1), _pos);
        this->calculateModelMatrix();
	}
    
    void Shape::setRotation(float _angle, glm::vec3 _rot)
    {
        this->rotationMatrix = glm::rotate(this->rotationMatrix, glm::radians(_angle), _rot);
        this->calculateModelMatrix();
    }
    
    void Shape::setScaling(glm::vec3 _scale)
    {
        this->scalingMatrix = glm::scale(glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1), _scale);
        this->calculateModelMatrix();
    }

    void Shape::calculateModelMatrix()
    {
        this->modelMatrix = translationMatrix * rotationMatrix * scalingMatrix * glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
    }
      

    
    const std::vector< glm::vec3 >& Shape::getPositions() const
    {
        return this->positions;
    }
    
    const std::vector< glm::vec3 >& Shape::getNormals() const
    {
        return this->normals;
    }
    
    const std::vector< glm::uvec3 >& Shape::getFaces() const
    {
        return this->faces;
    }

    const std::vector< glm::vec3 >& Shape::getFaceNormals() const
    {
        return this->faceNormals;
    }
    
    const std::vector< glm::vec3 >& Shape::getTangents() const
    {
        return this->tangents;
    }
    
	const glm::mat4& Shape::getModelMatrix() const
	{
		return this->modelMatrix;
	}



	glm::vec3 Shape::getPosition() {
		return this->objectPosition;
	}
    void Shape::setMaterial(std::shared_ptr<Material> _mat){
        material = _mat;
    }
    const std::vector< glm::vec3>& Shape::getColors() const{
        return this->colors;
    }
    const std::vector< glm::vec2>& Shape::getTexCoords() const{
        return this->texCoords;
    }
    const std::shared_ptr<Material>& Shape::getMaterial() const{
        return material;
    }
    
    void Shape::addVertex(const glm::vec3 &_ver){
        positions.push_back(_ver);
    }
    void Shape::addFace(const glm::uvec3 &_face){
        faces.push_back(_face);
    }
    void Shape::addNormal(const glm::vec3 &_norm){
        normals.push_back(_norm);
    }
    void Shape::addColor(const glm::vec3 &_color){
        colors.push_back(_color);
    }
    void Shape::addTexCoord(const glm::vec3 &_texCoord){
        texCoords.push_back(_texCoord);
    }
    void Shape::addTangent(const glm::vec3 &_tangent){
        tangents.push_back(_tangent);
    }
    void Shape::addLine(const glm::uvec2 &_line){
        lineIndices.push_back(_line);
    }
    const std::vector< glm::uvec2>& Shape::getLineIndices() const{
        return lineIndices;
    }

}
