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
#endif // __APPLE__

#include <iostream>
#include <SDL.h>
#include <vector>
#include <dirent.h>
#include <random>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "ext.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtc/type_ptr.hpp"

#include "Window.h"
#include "Shader.h"
#include "Buffer.h"
#include "Building.h"
#include "Object.h"
#include "Walls.h"
#include "Mesh.h"
#include "Corner.h"
#include "TrackballCamera.h"
#include "Builder.h"
#include "MixBuilder.h"
#include "WindowsOnlyBuilder.h"


int main()
{
	int width = 800;
	int height = 600;

	Window mainWindow(width,height);

#ifdef LINUX
	// initialise GLEW
	// it needs to be after the context creation, otherwise it GLEW will crash
	//std::cout <<"linux \n";
	glewExperimental = GL_TRUE;
	glewInit();
//	GLenum error = glGetError();
#endif // LINUX

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	std::string address = "Shaders/";

	Shader test("pippo", address+"phong_vert.glsl", address+"phong_frag.glsl");
	glLinkProgram(test.getShaderProgram());
	glUseProgram(test.getShaderProgram());

	glViewport(0,0,width, height);
	srandom(time(NULL));

	MixBuilder mixBuilder = MixBuilder((random()%3)+2);
	Building mixBuilding = mixBuilder.getBuilding();

	WindowsOnlyBuilder windowsOnlyBuiler = WindowsOnlyBuilder((random()%3)+2);
	Building windowsOnly = windowsOnlyBuiler.getBuilding();

	MixBuilder mixBuilder2 = MixBuilder((random()%3)+2);
	Building mixBuilding2 = mixBuilder2.getBuilding();

	std::vector<Building> buildings;
	buildings.push_back(mixBuilding);
	buildings.push_back(mixBuilding2);
	buildings.push_back(windowsOnly);

	int bufferSize = 0;
	for (auto building : buildings)
	{
		bufferSize += building.amountVertices();
	}


	Buffer buffer(bufferSize, sizeof(float)); // generate vbo buffer

	for (auto &building : buildings)
	{
		building.setBufferIndex(buffer.append((void *) building.getVertices(), building.amountVertices(), Buffer::VERTEX));
	}

	// pass the vertex data to the shader
	GLint pos = glGetAttribLocation(test.getShaderProgram(), "VertexPosition");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	for (auto &building : buildings)
	{
		buffer.append((void *) building.getNormals(), building.amountVertices(), Buffer::NORMAL);
	}

	// pass the normals to the shader
	GLint n = glGetAttribLocation(test.getShaderProgram(), "VertexNormal");
	glEnableVertexAttribArray(n);
	glVertexAttribPointer(n, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glm::mat4 projection = glm::perspective(glm::radians(60.0f),
	static_cast<float>(mainWindow.getWidth())/static_cast<float>(mainWindow.getHeight()), 0.1f, 100.0f);

	/// declare matrices adresses in the shader
	glm::mat4 MVP;	// MVP = projection * view * Model;
	glm::mat3 N;		// inverse transpose of MV
	GLint MVP_address = glGetUniformLocation(test.getShaderProgram(), "MVP");
	GLint MV_address = glGetUniformLocation(test.getShaderProgram(), "MV");
	GLint N_address = glGetUniformLocation(test.getShaderProgram(), "N"); // inverse transpose of MV

	TrackballCamera mainCamera = TrackballCamera();
	mainCamera.setInitialMousePos(0,0);
	mainCamera.setTarget(0.0f,0.0f,0.0f);
	mainCamera.setEye(-6.0f, 4.0f, 0.0f);

	SDL_Event event;
	bool quit = false;

	// main loop
	while (quit != true)
	{
		SDL_GetWindowSize(mainWindow.getWindow(), &width, &height);

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					quit = true;
					break;
				}
				case SDL_WINDOWEVENT:
				{
					SDL_GetWindowSize(mainWindow.getWindow(), &width, &height);
					mainWindow.setWindowSize(width, height);
					glViewport(0,0,width, height);
					projection = glm::perspective(glm::radians(60.0f),
					static_cast<float>(mainWindow.getWidth())/static_cast<float>(mainWindow.getHeight()), 0.1f, 100.0f);
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
				{
					mainCamera.handleMouseClick(event.button.x, event.button.y, event.button.button, event.type, 0);
					break;
				}
				default: { break; }
			}

			switch (event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
				{
					quit = true;
					break;
				}
				case SDLK_1:
				{
					if (buildings.size() > 0)
					{
						Mesh::write(buildings[0].getVerticesContainer(), buildings[0].getNormalsContainer(), "1_out.obj");
					}
					break;
				}

				case SDLK_2:
				{
					if (buildings.size() > 1)
					{
						Mesh::write(buildings[1].getVerticesContainer(), buildings[1].getNormalsContainer(), "2_out.obj");
					}
					break;
				}
				case SDLK_3:
				{
					if (buildings.size() > 2)
					{
						Mesh::write(buildings[2].getVerticesContainer(), buildings[2].getNormalsContainer(), "3_out.obj");
					}
					break;
				}
				default:break;
			}
			mainCamera.handleMouseMove(event.button.x, event.button.y);
		}
		mainCamera.update();
		glClearColor(0.36f,0.36f,0.36f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::vec3 offset = glm::vec3(0,0,7);
		glm::mat4 MV = glm::mat4(1.0);
		MV = glm::translate(MV, -offset*buildings.size()/2);
		for (auto &building : buildings)
		{
			MV = glm::translate(MV, offset);
			MVP = projection * mainCamera.viewMatrix() * MV;
			N = glm::mat3(glm::inverse(glm::transpose(MV)));
			glUniformMatrix4fv(MVP_address, 1, GL_FALSE, glm::value_ptr(MVP));
			glUniformMatrix4fv(MV_address, 1, GL_FALSE, glm::value_ptr(MV));
			glUniformMatrix3fv(N_address, 1, GL_FALSE, glm::value_ptr(N));
			glDrawArrays(GL_TRIANGLES, building.getBufferIndex()/3, building.amountVertices()/3);
		}

		SDL_GL_SwapWindow(mainWindow.getWindow());
	}
	glDeleteProgram(test.getShaderProgram());
	return EXIT_SUCCESS;
}
