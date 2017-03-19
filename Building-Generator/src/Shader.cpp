//
//  Shader.cpp
//  Game_ios
//
//  Created by Alberto La Scala on 29/10/2016.
//
//

#include <stdio.h>
#include <iostream>
#include "Shader.h"


Shader::Shader(std::string _name, std::string _vertex, std::string _fragment)
{
	m_shaderProgram = glCreateProgram();
	m_name = _name;

	GLint tmp_shader;

	// creation of the vertex shader
	tmp_shader = glCreateShader(GL_VERTEX_SHADER);
	std::string pippo = loadShader(_vertex);
	GLchar * tmp_shaderSource = (GLchar *)pippo.c_str();
	glShaderSource(tmp_shader, 1, &tmp_shaderSource, NULL);
	glCompileShader(tmp_shader);
	glAttachShader(m_shaderProgram, tmp_shader);

	GLint status;
	glGetShaderiv(tmp_shader, GL_COMPILE_STATUS, &status);

	if( !status ) { std::cout << "vertex shader did not compile" << '\n'; }

	// creation of the fragment shader
	tmp_shader = glCreateShader(GL_FRAGMENT_SHADER);
	pippo = loadShader(_fragment);
	tmp_shaderSource = (GLchar *)pippo.c_str();
	glShaderSource(tmp_shader, 1, &tmp_shaderSource, NULL);
	glCompileShader(tmp_shader);
	glAttachShader(m_shaderProgram, tmp_shader);

	glGetShaderiv(tmp_shader, GL_COMPILE_STATUS, &status);

	if( !status ) { std::cout << "fragment shader did not compile" << '\n'; }

	GLchar buffer[500];
	glGetShaderInfoLog(tmp_shader,
	500,
	nullptr,
	buffer);
	glDeleteShader(tmp_shader);
}

std::string Shader::loadShader(std::string _filename)
{
	std::ifstream shaderFile(_filename);
	std::string tmp_source;
	// iteration from beginning to end of the file
	// The default-constructed std::istreambuf_iterator is known as the end-of-stream iterator
	tmp_source = std::string(std::istreambuf_iterator<char>(shaderFile), std::istreambuf_iterator<char>());
	shaderFile.close();
	tmp_source += '\0';
	return tmp_source;
}
