//
//  main.cpp
//  Building Generator
//
//  Created by Alberto La Scala on 08/12/15.
//  Copyright © 2015 ___AlbertoLaScala___. All rights reserved.
//

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#define LINUX
#endif

#include <iostream>
#include <SDL.h>
#include "Window.h"
#include "Shader.h"
#include "Buffer.h"
#include <time.h>
#include "Mesh.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include <memory>

int main()
{
	int width = 800;
	int height = 600;
	bool mouse_down;
	int mouse_down_position;

	Mesh plane("models/plane.obj", "ship");
	Window mainWindow(width,height);

#ifdef LINUX
	// this needs to be after the context creation, otherwise it GLEW will crash
	//std::cout <<"linux \n";
	glewExperimental = GL_TRUE;
	glewInit();
//	GLenum error = glGetError();
#endif

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	std::string address = "Shaders/";

	Shader test("pippo", address+"phong_vert.glsl", address+"phong_frag.glsl");
	glLinkProgram(test.getShaderProgram());
	glUseProgram(test.getShaderProgram());

	int bufferSize = plane.getAmountVertexData();
	Buffer buffer(bufferSize, sizeof(float));
	plane.setBufferIndex(buffer.append((void*) &plane.getVertexData(), plane.getAmountVertexData(), Buffer::VERTEX));

	// pass the vertex data to the shader
	GLint pos = glGetAttribLocation(test.getShaderProgram(), "VertexPosition");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	buffer.append((void*) &plane.getNormalsData(), plane.getAmountVertexData(), Buffer::NORMAL);

	// pass the normals to the shader
	GLint n = glGetAttribLocation(test.getShaderProgram(), "VertexNormal");
	glEnableVertexAttribArray(n);
	glVertexAttribPointer(n, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// transformation of the mesh
	glm::mat4 MV_plane = glm::mat4(1.0);
	//MV_plane = glm::translate(MV_plane, glm::vec3(0.0f, 0.0f, -2.0f));
	//MV_plane = glm::rotate(MV_plane, glm::radians(45.0f),glm::vec3(0,1,0));
	//std::cout << glm::to_string(MV_plane) << '\n';

	// virtual camera
	glm::vec3 eye(3.0, 3.0f, 3.0f);
	glm::mat4 view = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projection = glm::perspective(glm::radians(60.0f),
	static_cast<float>(mainWindow.getWidth())/static_cast<float>(mainWindow.getHeight()), 0.1f, 100.0f);

	/// declare matrices adresses in the shader
	glm::mat4 MVP; // MVP = projection * view * Model;
	glm::mat3 N; // inverse transpose of MV
	GLint MVP_address = glGetUniformLocation(test.getShaderProgram(), "MVP");
	GLint MV_address = glGetUniformLocation(test.getShaderProgram(), "MV");
	GLint N_address = glGetUniformLocation(test.getShaderProgram(), "N"); // inverse transpose of MV

	glViewport(0,0,width, height);
	SDL_Event event;
	bool quit = false;
	while (quit != true)
	{
		SDL_GetWindowSize(mainWindow.getWindow(), &width, &height);

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT )//|| event.key.keysym.sym == SDLK_ESCAPE)
				quit = true;
			if (event.type == SDL_WINDOWEVENT)
			{
				SDL_GetWindowSize(mainWindow.getWindow(), &width, &height);
				mainWindow.setWindowSize(width, height);

				glViewport(0,0,width, height);
				projection = glm::perspective(glm::radians(60.0f),
				static_cast<float>(mainWindow.getWidth())/static_cast<float>(mainWindow.getHeight()),
				0.1f, 100.0f);
			}
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT )
			{
				//std::cout << "DOWN " << event.button.x << '\n';
				mouse_down = true;
				mouse_down_position = event.button.x;
			}
			if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT )
			{
				//std::cout << "UP " << event.button.x << '\n';
				mouse_down = false;
			}
		}

		// attempt to make a trackball camera
		if (mouse_down)
		{
				float angle = (static_cast<float>(event.motion.x) - static_cast<float>(mouse_down_position))/10;
				mouse_down_position = event.motion.x;
				//eye = glm::rotate(eye, glm::radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));
				//view = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				//std::cout << angle << '\n';
		}

		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Caluclate matrices of ship and draw
		//MV_plane = glm::rotate(MV_plane, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));


		MV_plane = glm::mat4(1.0f);
		bool closing = false;
		std::string building_string = "rurdruullldd";//"rruuldld";//"ruld";

		for (int i = 0; i < building_string.length(); ++i)
		{
			if (building_string[i] == 'd') // Down
			{
				if (i > 0 && building_string[i-1] == 'd')
				{
					MV_plane = glm::translate(MV_plane, glm::vec3(0,0,1));
				}
				else if (i > 0 && building_string[i-1] == 'l')
				{
					MV_plane = glm::translate(MV_plane, glm::vec3(0.5,0,0.5));
					MV_plane = glm::rotate(MV_plane, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else
				{
					MV_plane = glm::translate(MV_plane, glm::vec3(0.5,0,-0.5));
					MV_plane = glm::rotate(MV_plane, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				}
				closing = true;
			}
			else if(building_string[i] == 'r') // Right
			{
				if (i > 0 && building_string[i-1] == 'r' )
				{
					MV_plane = glm::translate(MV_plane, glm::vec3(0,0,1));
				}
				else if(closing)
				{
					// in case the previous operation was l, it means the geometry is needs to rotate in the other direction
					MV_plane = glm::translate(MV_plane, glm::vec3(0.5f,0,0.5f));
					MV_plane = glm::rotate(MV_plane, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else if (i != 0)
				{
					MV_plane = glm::translate(MV_plane, glm::vec3(-0.5f,0,0.5f));
					MV_plane = glm::rotate(MV_plane, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				}
			}
			else if(building_string[i] == 'u') // UP
			{
				closing = false;
				if (i > 0 && building_string[i-1] == 'u')
				{
					// in case it is continuing in the same path, don't rotate, just keep going
					MV_plane = glm::translate(MV_plane, glm::vec3(0,0,1));
				}
				else if(closing)
				{
					// in case the previous operation was l, it means the geometry is needs to rotate in the other direction
					MV_plane = glm::translate(MV_plane, glm::vec3(-0.5f,0,0.5f));
					MV_plane = glm::rotate(MV_plane, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else
				{
					MV_plane = glm::translate(MV_plane, glm::vec3(0.5f,0,0.5f));
					MV_plane = glm::rotate(MV_plane, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				}

			}
			else if(building_string[i] == 'l') // Left
			{
				 // if the l operation is called it meand that the building is closing, some operations need to e inverted
				if (i > 0 && building_string[i-1] == 'l')
				{
					MV_plane = glm::translate(MV_plane, glm::vec3(0,0,1));
				}
				else if (closing)
				{
					MV_plane = glm::translate(MV_plane, glm::vec3(-0.5f,0,0.5f));
					MV_plane = glm::rotate(MV_plane, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else
				{
					MV_plane = glm::translate(MV_plane, glm::vec3(0.5f,0,0.5f));
					MV_plane = glm::rotate(MV_plane, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				}
			}

			MVP = projection * view * MV_plane;
			N = glm::mat3(glm::inverse(glm::transpose(MV_plane)));
			glUniformMatrix4fv(MVP_address, 1, GL_FALSE, glm::value_ptr(MVP));
			glUniformMatrix4fv(MV_address, 1, GL_FALSE, glm::value_ptr(MV_plane));
			glUniformMatrix3fv(N_address, 1, GL_FALSE, glm::value_ptr(N));
			glDrawArrays(GL_TRIANGLES, plane.getBufferIndex()/3, (plane.getAmountVertexData()/3));
		}
		SDL_GL_SwapWindow(mainWindow.getWindow());
	}
	glDeleteProgram(test.getShaderProgram());
	return EXIT_SUCCESS;
}
