//
//  Shader.hpp
//  Game_ios
//
//  Created by Alberto La Scala on 29/10/2016.
//
//

#ifndef Shader_h
#define Shader_h

#ifdef linux
#include <GL/glew.h>
#endif
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif


#include <string>
#include <fstream>

class Shader
{
private:
	std::string m_name;
	GLuint m_shaderProgram;
	void setName(std::string _name) { this -> m_name = _name; }
	std::string loadShader(std::string _filename);
public:
	Shader(std::string _name, std::string _vertexPath, std::string _fragmentPath);
	std::string getName() { return this -> m_name; }
	GLuint getShaderProgram() { return m_shaderProgram; }
};

#endif /* Shader_h */
