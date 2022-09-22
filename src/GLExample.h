#ifndef Example_GLExample_h
#define Example_GLExample_h

#include "GLApp.h"
#include "Cube.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Texture.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "Mesh.h"
#include "FrameBuffer.h"


namespace cgCourse
{
	struct LightInfo
	{
		glm::vec3 radiance;
        glm::vec3 position;
	};

	class GLExample : public GLApp
	{
	public:
		enum LightMotionMode {
			Forward = 0, Backward  = 1
		};

		GLExample(glm::uvec2 _windowSize, std::string _title, std::string _exepath);

		bool init() override;
        bool update(double dt) override;
		bool render() override;
		bool end() override;

        std::vector<LightInfo> lights;
	private:
        void addLightVariables(const std::shared_ptr<ShaderProgram>& _program);
        std::shared_ptr<ShaderProgram> programForMeshPhong;
        std::shared_ptr<ShaderProgram> activeProgram;
        std::shared_ptr<ShaderProgram> pbrProgram;


		std::shared_ptr<Cube> cube;
        std::vector<std::shared_ptr<Cube>> lightboxes;

        
		float animation;
		LightMotionMode animationDir;
		
		glm::mat4 mvpMatrix=glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

		Camera cam;

        
        Mesh gun;
        int shadingAlgorithm = 0; //phong, PBR
        
        int numberOfLights;
	};
}

#endif
