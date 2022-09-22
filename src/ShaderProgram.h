#ifndef Example_ShaderProgram_h
#define Example_ShaderProgram_h

#include "GLIncludes.h"
#include <map>
#include <string>
#include <glm/mat4x4.hpp>
#include "Texture.h"
#include <memory>

namespace cgCourse
{

	class ShaderProgram
	{
	public:
		ShaderProgram(std::string _name);

        
        
        void deleteShaderProgramFromGPU();
        void bind() const;
        void unbind() const;
        void addTexture(std::string _textureVarName, unsigned int _handle);
        void clearTextures();
        // Get uniform locations, e.g. for our mvp matrix
        // This can be done on the fly however, usually
        // the location is stored while shader compilation
        // to avoid the name lookup.
        GLuint getUniformLocation(const std::string& _uniformName) const;
        void setUniform3fv(std::string _uniformName, glm::vec3 _value);
        void setUniformMat4fv(std::string _uniformName, glm::mat4 _value);
        void setUniformf(std::string _uniformName, float _value);
        void setUniformi(std::string _uniformName, int _value);
        void setUniform2fv(std::string _uniformName, glm::vec2 _value);
        void setUniformBlockBuffer(std::string _uniformBlockName, unsigned int _bufferObject);

	private:
		bool loadAndCompileShaderToGPU(GLuint _program, std::string _source);
		bool checkLinkStatusOfProgram(GLuint _program);
        std::string loadFile(const std::string & filename) const;
        
		GLuint program;
        std::map<std::string,unsigned int> textures;
        std::map<std::string,glm::vec3> uniforms3fv;
        std::map<std::string,glm::mat4> uniformsMat4fv;
        std::map<std::string,float> uniformsf;
        std::map<std::string,int> uniformsi;
        std::map<std::string,glm::vec2> uniforms2fv;
        std::map<std::string,unsigned int> uniformBlockBuffers;
	};
}

#endif
