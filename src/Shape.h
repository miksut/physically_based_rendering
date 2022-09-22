#ifndef Example_Shape_h
#define Example_Shape_h

#include <vector>
#include <memory>
#include <glm/mat4x4.hpp>

namespace cgCourse
{
    class Texture;
    struct Material{
        glm::vec3 ka={1,1,1};
        glm::vec3 kd={1,1,1};
        glm::vec3 ks={1,1,1};
        float ns=1;
        std::shared_ptr<Texture> diffuseTexture = nullptr;
        std::shared_ptr<Texture> normalTexture = nullptr;
        std::shared_ptr<Texture> specTexture = nullptr;
        std::shared_ptr<Texture> metalnessTexture = nullptr;
        std::shared_ptr<Texture> roughnessTexture = nullptr;
        bool hasObjectColor=false;
        glm::vec3 color = {0.0,0.5,0.5};
        float illumination=0;
    };
    class Shape
	{
	public:
        Shape();
        
		const std::vector< glm::vec3 >& getPositions() const;
		const std::vector< glm::vec3 >& getNormals() const;
		const std::vector< glm::uvec3 >& getFaces() const;
		const std::vector< glm::vec3 >& getFaceNormals() const;
        const std::vector< glm::vec3 >& getTangents() const;
        const std::vector< glm::vec3>& getColors() const;
        const std::vector< glm::vec2>& getTexCoords() const;
        const std::shared_ptr<Material>& getMaterial() const;
        const std::vector< glm::uvec2>& getLineIndices() const;

        
		const glm::mat4& getModelMatrix() const;
		glm::vec3 getPosition();
		
        void setPosition(glm::vec3);
        void setRotation(float _angle, glm::vec3 _rot);
        void setScaling(glm::vec3 _scale);
		void calculateModelMatrix();
        void setMaterial(std::shared_ptr<Material> _mat);
        
        void addVertex(const glm::vec3 &_ver);
        void addFace(const glm::uvec3 &_face);
        void addNormal(const glm::vec3 &_norm);
        void addColor(const glm::vec3 &_color);
        void addTexCoord(const glm::vec3 &_texCoord);
        void addTangent(const glm::vec3 &_tangent);
        void addLine(const glm::uvec2 &_line);
        
	protected:

        
		std::vector< glm::vec3 > positions;
		std::vector< glm::vec3 > colors;
		std::vector< glm::vec3 > normals;
		std::vector< glm::uvec3 > faces;
		std::vector< glm::vec3 > faceNormals;
		std::vector< glm::vec2 > texCoords;
        std::vector< glm::vec3 > tangents;
        std::vector< glm::uvec2 > lineIndices;
        
		glm::vec3 objectPosition;

        glm::mat4 rotationMatrix=glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
        glm::mat4 translationMatrix=glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
        glm::mat4 scalingMatrix=glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
        glm::mat4 modelMatrix=glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
		

        
        std::shared_ptr<Material> material=nullptr;
        

        
	};
}

#endif

