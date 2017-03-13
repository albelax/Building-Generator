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
#include <vector>
#include <dirent.h>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Window.h"
#include "Shader.h"
#include "Buffer.h"
#include "Building.h"
#include "Object.h"
#include "Walls.h"
#include "Mesh.h"
#include "Corner.h"
#include "TrackballCamera.h"

#define TRACKBALL 1

int main()
{
	int width = 800;
	int height = 600;

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

	glViewport(0,0,width, height);

	Building building = Building();
	int bufferSize = building.amountVertices();

	Buffer buffer(bufferSize, sizeof(float)); // generate vbo buffer
	building.setBufferIndex(buffer.append((void *) building.getVertices(), building.amountVertices(), Buffer::VERTEX));

	// pass the vertex data to the shader
	GLint pos = glGetAttribLocation(test.getShaderProgram(), "VertexPosition");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	buffer.append((void *) building.getNormals(), building.amountVertices(), Buffer::NORMAL);
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
	// Needed for the fixed camera
	mainCamera.setTarget(0.0f,0.0f,0.0f);
	mainCamera.setEye(0.0f, 2.0f, 5.0f);

	SDL_Event event;
	bool quit = false;

	// main loop
	while (quit != true)
	{
		SDL_GetWindowSize(mainWindow.getWindow(), &width, &height);

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
				mainCamera.handleMouseClick(event.button.x, event.button.y, event.button.button, event.type, 0);
			mainCamera.handleMouseMove(event.button.x, event.button.y);

			switch (event.type)
			{
				case SDL_QUIT: quit = true; break;
				case SDL_WINDOWEVENT:
					SDL_GetWindowSize(mainWindow.getWindow(), &width, &height);
					mainWindow.setWindowSize(width, height);
					glViewport(0,0,width, height);
					projection = glm::perspective(glm::radians(60.0f),
					static_cast<float>(mainWindow.getWidth())/static_cast<float>(mainWindow.getHeight()), 0.1f, 100.0f);
					break;
				default: break;
			}
//			mainCamera.handleMouseMove(event.button.x, event.button.y);
//			switch(event.key.keysym.sym)
//			{
//				case SDLK_ESCAPE: quit = true; break;
//			}

		}
		mainCamera.update();
		glClearColor(0.36f,0.36f,0.36f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MVP = projection * mainCamera.viewMatrix() * glm::mat4(1.0f);
		N = glm::mat3(glm::inverse(glm::transpose(glm::mat4(1.0f))));
		glUniformMatrix4fv(MVP_address, 1, GL_FALSE, glm::value_ptr(MVP));
		glUniformMatrix4fv(MV_address, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		glUniformMatrix3fv(N_address, 1, GL_FALSE, glm::value_ptr(N));
		glDrawArrays(GL_TRIANGLES, building.getBufferIndex()/3, building.amountVertices()/3);

		SDL_GL_SwapWindow(mainWindow.getWindow());
	}
	glDeleteProgram(test.getShaderProgram());
	return EXIT_SUCCESS;
}
