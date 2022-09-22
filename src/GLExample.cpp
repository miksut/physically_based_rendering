#include "GLExample.h"
#include "Cube.h"
#include <iostream>
#include <fstream>


#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include "Gui.h"





namespace cgCourse
{
    Assimp::Importer importer;

    GLExample::GLExample(glm::uvec2 _windowSize, std::string _title, std::string _exepath)
		: GLApp(_windowSize, _title, _exepath, false)
	{
		this->mvpMatrix = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
		this->animation = 0.0;
		this->animationDir = Forward;
	}


	bool GLExample::init()
	{
        initGui(window_);
        lights.push_back(LightInfo());
        lights.back().radiance = glm::vec3(1.0, 1.0, 1.0);
        lights.back().position = glm::vec3(0.0, 0.0, 0.0);
        
        lights.push_back(LightInfo());
        lights.back().radiance = glm::vec3(1.0, 1.0, 1.0);
        lights.back().position = glm::vec3(-10,0,0);
        
        lights.push_back(LightInfo());
        lights.back().radiance = glm::vec3(1.0, 1.0, 1.0);
        lights.back().position = glm::vec3(10,0,0);
        
        lights.push_back(LightInfo());
        lights.back().radiance = glm::vec3(1.0, 1.0, 1.0);
        lights.back().position = glm::vec3(0,10,0);
        numberOfLights = 4;
        
        for (int i = 0 ; i<numberOfLights;i++){
            lightboxes.push_back(std::make_shared<Cube>());
            if (!lightboxes[i]->createVertexArray(0, 1, 2, 3, 4 )) {
                return false;
            };
            lightboxes[i]->setPosition(glm::vec3(0.0, 0.5, -1.0));
            lightboxes[i]->setScaling(glm::vec3(0.3, 0.3, 0.3));
            lightboxes[i]->setMaterial(std::make_shared<Material>());
        }
        
        connectVar("lightDiffuse", &lights[0].radiance.x);

        connectVar("shadingAlgorithm", &shadingAlgorithm);

		// Framebuffer size and window size may be different in high-DPI displays
		// setup camera with standard view (static for our case)
		this->cam.create(this->getFramebufferSize(),this->getWindowSize(),
			glm::vec3(3, 3, -3), 
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0));
        InputManager::registerInputAcceptor(&cam);
        connectVar("cameraType", &cam.getType());
		
        

		// Init models
		this->cube = std::make_shared<Cube>();
		bool constructed = this->cube->createVertexArray(0, 1, 2, 3, 4);
		if (!constructed) return constructed;
		this->cube->setPosition(glm::vec3(-2.5, 0.5, 1.5));
        this->cube->setScaling(glm::vec3(1.5, 1.5, 1.5));
        this->cube->getMaterial()->ka = glm::vec3(0.4);

        
        programForMeshPhong = std::make_shared<ShaderProgram>(this->getPathToExecutable() + "../../shader/Mesh_phong");

        gun.load(this->getPathToExecutable() + "../../res/FBX/","cerberus.fbx",false,false,false);
        gun.setPosition(glm::vec3(0.0,0.0,1.0));
        gun.setScaling(glm::vec3(0.02,0.02,0.02));
        gun.setRotation(-90, glm::vec3(1,0,0));
        gun.setRotation(-180, glm::vec3(0,0,1));
        gun.setMaterial(std::make_shared<Material>());
        gun.getMaterial()->diffuseTexture = std::make_shared<Texture>();
        gun.getMaterial()->diffuseTexture->loadFromFile(this->getPathToExecutable() + "../../res/FBX/Cerberus_A.tga",true);
        gun.getMaterial()->metalnessTexture = std::make_shared<Texture>();
        gun.getMaterial()->metalnessTexture->loadFromFile(this->getPathToExecutable() + "../../res/FBX/cerberus_M.png");
        gun.getMaterial()->normalTexture = std::make_shared<Texture>();
        gun.getMaterial()->normalTexture->loadFromFile(this->getPathToExecutable() + "../../res/FBX/cerberus_N.png");
        gun.getMaterial()->roughnessTexture = std::make_shared<Texture>();
        gun.getMaterial()->roughnessTexture->loadFromFile(this->getPathToExecutable() + "../../res/FBX/cerberus_R.png");
        
        
        pbrProgram = std::make_shared<ShaderProgram>(this->getPathToExecutable() + "../../shader/pbr");

        
        


        std::cout<<"Frame Buffer Size:"<<getFramebufferSize().x<<", "<<getFramebufferSize().y<<std::endl;
        
        
        return constructed;
	}

    bool GLExample::update(double dt)
    {
        updateGui();


		if(animationDir == Forward)
		{
			if (animation > 1.5) {
				animationDir = Backward;
			} else {
				animation += dt;
			}
		} else {
			if (animation < -4.0) {
				animationDir = Forward;
			}
			else {
				animation -= dt;
			}
		}
        lightboxes[0]->setPosition(glm::vec3(animation,5,-5));
        this->lights[0].position = lightboxes[0]->getPosition();

        this->addLightVariables(programForMeshPhong);
        this->addLightVariables(pbrProgram);


        for (int i = 0; i<numberOfLights;i++){
            lightboxes[i]->setPosition(lights[i].position);
            lightboxes[i]->getMaterial()->hasObjectColor=false;
            lightboxes[i]->getMaterial()->color = lights[0].radiance;
            lightboxes[i]->getMaterial()->illumination = 1;
            lightboxes[i]->getMaterial()->kd = glm::vec3(0);
            lightboxes[i]->getMaterial()->ka = glm::vec3(0);
        }
        

        return true;
    }

    bool GLExample::render()
	{
        glDisable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
        
        // Clear the color and depth buffers
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        
            bindDefaultFrameBuffer();
            clearDefaultFrameBuffer();
            
            switch (shadingAlgorithm){
                case 0:
                    activeProgram = programForMeshPhong;
                    break;
                case 1:
                    activeProgram = pbrProgram;
                    break;
            }
            for (int i = 0; i<numberOfLights;i++){

                lightboxes[i]->draw(this->cam.getProjectionMatrix(),this->cam.getViewMatrix(),programForMeshPhong);
            }
            gun.draw(this->cam.getProjectionMatrix(),this->cam.getViewMatrix(),activeProgram);
            renderGui();
            return true;
	}

    void GLExample::addLightVariables(const std::shared_ptr<ShaderProgram>& _program) {
        _program->setUniform3fv("camPos", this->cam.getPosition());
        int c =0;
        for (auto tlight:lights){
            _program->setUniform3fv("lights["+std::to_string(c)+"].position", tlight.position);
            _program->setUniform3fv("lights["+std::to_string(c)+"].radiance", tlight.radiance);
            c++;
        }
    }

	bool GLExample::end()
	{
	//	programForCube->deleteShaderProgramFromGPU();
	//	programForTorus->deleteShaderProgramFromGPU();
	//	programForTorusNormals->deleteShaderProgramFromGPU();
		return true;
	}

    
    
}

