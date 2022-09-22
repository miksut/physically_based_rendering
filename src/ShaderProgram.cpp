#include "ShaderProgram.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <iostream>
#include <fstream>

namespace cgCourse
{
    ShaderProgram::ShaderProgram(std::string _name)
	{
		program = 0;
		
        // load shader source code
        std::string vertShaderString = this->loadFile(_name + ".vert");
        std::string fragShaderString = this->loadFile(_name + ".frag");

		// Build and compile our shader program consisting of 
		// a vertex and fragment shader
		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

		loadAndCompileShaderToGPU(vertShader, vertShaderString);
		loadAndCompileShaderToGPU(fragShader, fragShaderString);
		
		// Link shaders
		this->program = glCreateProgram();
		// check if it exists
		assert(this->program);
		// attach the shaders to the shader program
		glAttachShader(this->program, vertShader);
		glAttachShader(this->program, fragShader);
		// link the program to make it ready to use by the GPU
		glLinkProgram(this->program);
		// check the link status of the program and show linking errors
		// if there are any
		this->checkLinkStatusOfProgram(this->program);
		
		// shader programs can be marked as to delete. We don't have
		// to do this here if it is done later anywhen in the program. An explanation is here:
		// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDeleteShader.xhtml
        //
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
	}
    
    std::string ShaderProgram::loadFile(const std::string& _filename) const
    {
        std::string result;
        std::ifstream stream(_filename.c_str());
        
        if (!stream.is_open()){
            return result;
        }
        stream.seekg(0, std::ios::end);
        result.reserve(stream.tellg());
        stream.seekg(0, std::ios::beg);
        
        result.assign(
                      (std::istreambuf_iterator<char>(stream)),
                      std::istreambuf_iterator<char>());
        
        return result;
    }

	bool ShaderProgram::loadAndCompileShaderToGPU(GLuint _program, std::string _source) {
		GLint status;
		GLchar errorMessage[512];

		char* shadersource = (char *)_source.c_str();
		int shaderstringlength = _source.length();
		glShaderSource(_program, 1, (const GLchar **)&shadersource, &shaderstringlength);
		glCompileShader(_program);
		glGetShaderiv(_program, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			glGetShaderInfoLog(_program, 512, 0, errorMessage);
			std::cerr << "Fragment Shader compilation error:" << std::endl
				<< errorMessage << std::endl;
			return false;
		}
		return true;
	}

	bool ShaderProgram::checkLinkStatusOfProgram(GLuint _program) {
		GLint status;
		GLchar errorMessage[512];
		glGetProgramiv(_program, GL_LINK_STATUS, &status);
		if (!status) {
			glGetProgramInfoLog(_program, 512, 0, errorMessage);
			std::cerr << "Linking error:" << std::endl
				<< errorMessage << std::endl;
			return false;
		}
		return true;
	}
    
    GLuint ShaderProgram::getUniformLocation(const std::string& _uniformName) const{
        return glGetUniformLocation(this->program, _uniformName.c_str());
    }
                              
    void ShaderProgram::deleteShaderProgramFromGPU() {
        if (this->program) {
            glDeleteProgram(this->program);
        }
    }
    
    void ShaderProgram::bind() const {
        glUseProgram(this->program);
        int c =0;
        for (auto &t:textures){
            glActiveTexture(GL_TEXTURE0 + c);
            glBindTexture(GL_TEXTURE_2D, t.second);
            glUniform1i(this->getUniformLocation(t.first), c);
            c++;
        }
        for (auto &u:uniforms3fv){
            glUniform3fv(this->getUniformLocation(u.first), 1, &u.second[0]);
        }
        for (auto &u:uniformsMat4fv){
            glUniformMatrix4fv(this->getUniformLocation(u.first), 1, GL_FALSE, &u.second[0][0]);
        }
        for (auto &u:uniformsf){
            glUniform1f(this->getUniformLocation(u.first), u.second);
        }
        for (auto &u:uniformsi){
            glUniform1i(this->getUniformLocation(u.first), u.second);
        }
        for (auto &u:uniforms2fv){
            glUniform2fv(this->getUniformLocation(u.first), 1, &u.second[0]);
        }
        int bindingPoint = 0;
        for (auto &u:uniformBlockBuffers){
            bindingPoint++;
            glUniformBlockBinding(program, glGetUniformBlockIndex(program, u.first.c_str()), bindingPoint);
            glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, u.second);
        }

    }
    
    void ShaderProgram::unbind() const {
        for (int i = 0 ; i<textures.size();i++){
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glUseProgram(0);
    }
    void ShaderProgram::addTexture(std::string _textureVarName, unsigned int _handle){
        std::map<std::string,unsigned int>::iterator it = textures.find(_textureVarName);
        if (it != textures.end())
            it->second = _handle;
        else
            textures.insert(std::pair<std::string,unsigned int>(_textureVarName,_handle));
    }
    void ShaderProgram::clearTextures(){
        textures.clear();
    }
    void ShaderProgram::setUniform3fv(std::string _uniformName, glm::vec3 _value){
        std::map<std::string,glm::vec3>::iterator it = uniforms3fv.find(_uniformName);
        if (it != uniforms3fv.end())
            it->second = _value;
        else
            uniforms3fv.insert(std::pair<std::string,glm::vec3>(_uniformName,_value));
    }
    void ShaderProgram::setUniformMat4fv(std::string _uniformName, glm::mat4 _value){
        std::map<std::string,glm::mat4>::iterator it = uniformsMat4fv.find(_uniformName);
        if (it != uniformsMat4fv.end())
            it->second = _value;
        else
            uniformsMat4fv.insert(std::pair<std::string,glm::mat4>(_uniformName,_value));
    }
    void ShaderProgram::setUniformf(std::string _uniformName, float _value){
        std::map<std::string,float>::iterator it = uniformsf.find(_uniformName);
        if (it != uniformsf.end())
            it->second = _value;
        else
            uniformsf.insert(std::pair<std::string,float>(_uniformName,_value));
        
    }
    void ShaderProgram::setUniformi(std::string _uniformName, int _value){
        std::map<std::string,int>::iterator it = uniformsi.find(_uniformName);
        if (it != uniformsi.end())
            it->second = _value;
        else
            uniformsi.insert(std::pair<std::string,int>(_uniformName,_value));
        
    }
    void ShaderProgram::setUniform2fv(std::string _uniformName, glm::vec2 _value){
        std::map<std::string,glm::vec2>::iterator it = uniforms2fv.find(_uniformName);
        if (it != uniforms2fv.end())
            it->second = _value;
        else
            uniforms2fv.insert(std::pair<std::string,glm::vec2>(_uniformName,_value));
    }
    void ShaderProgram::setUniformBlockBuffer(std::string _uniformBlockName, unsigned int _bufferObject){
        std::map<std::string,unsigned int>::iterator it = uniformBlockBuffers.find(_uniformBlockName);
        if (it != uniformBlockBuffers.end())
            it->second = _bufferObject;
        else
            uniformBlockBuffers.insert(std::pair<std::string,unsigned int>(_uniformBlockName,_bufferObject));
    }
}

