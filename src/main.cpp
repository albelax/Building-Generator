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
#include "Rule.h"
#include "Mesh.h"


int main()
{
	int width = 800;
	int height = 600;
	bool mouse_down;
	int mouse_down_position_x;
	int mouse_down_position_y;

	Mesh plane("models/L.obj", "ship");
//	Mesh D("models/D.obj", "ship");
//	Mesh U("models/U.obj", "ship");
//	Mesh R("models/R.obj", "ship");
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

	int bufferSize = plane.getAmountVertexData();// + D.getAmountVertexData() + U.getAmountVertexData() + R.getAmountVertexData();
	Buffer buffer(bufferSize, sizeof(float));
	plane.setBufferIndex(buffer.append((void*) &plane.getVertexData(), plane.getAmountVertexData(), Buffer::VERTEX));
//	D.setBufferIndex(buffer.append((void*) &D.getVertexData(), D.getAmountVertexData(), Buffer::VERTEX));
//	R.setBufferIndex(buffer.append((void*) &R.getVertexData(), R.getAmountVertexData(), Buffer::VERTEX));
//	U.setBufferIndex(buffer.append((void*) &U.getVertexData(), U.getAmountVertexData(), Buffer::VERTEX));

	// pass the vertex data to the shader
	GLint pos = glGetAttribLocation(test.getShaderProgram(), "VertexPosition");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	buffer.append((void*) &plane.getNormalsData(), plane.getAmountVertexData(), Buffer::NORMAL);
//	buffer.append((void*) &D.getNormalsData(), D.getAmountVertexData(), Buffer::NORMAL);
//	buffer.append((void*) &R.getNormalsData(), R.getAmountVertexData(), Buffer::NORMAL);
//	buffer.append((void*) &U.getNormalsData(), U.getAmountVertexData(), Buffer::NORMAL);

	// pass the normals to the shader
	GLint n = glGetAttribLocation(test.getShaderProgram(), "VertexNormal");
	glEnableVertexAttribArray(n);
	glVertexAttribPointer(n, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// transformation of the mesh
	glm::mat4 MV_plane = glm::mat4(1.0);

	// virtual camera
	glm::vec3 eye(0, 3.0f, 3.0f);
	glm::mat4 view = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projection = glm::perspective(glm::radians(60.0f),
	static_cast<float>(mainWindow.getWidth())/static_cast<float>(mainWindow.getHeight()), 0.1f, 100.0f);

	/// declare matrices adresses in the shader
	glm::mat4 MVP; // MVP = projection * view * Model;
	glm::mat3 N; // inverse transpose of MV
	GLint MVP_address = glGetUniformLocation(test.getShaderProgram(), "MVP");
	GLint MV_address = glGetUniformLocation(test.getShaderProgram(), "MV");
	GLint N_address = glGetUniformLocation(test.getShaderProgram(), "N"); // inverse transpose of MV

	Building building = Building();

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
				mouse_down_position_x = event.button.x;
				mouse_down_position_y = event.button.y;
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
			glm::mat4 CameraRotation(glm::mat4(1.0f));

			float x_angle = (static_cast<float>(event.motion.x) - static_cast<float>(mouse_down_position_x))*0.1;
			//float y_angle = (static_cast<float>(event.motion.y) - static_cast<float>(mouse_down_position_y))*0.1;
//			CameraRotation = glm::rotate(CameraRotation, glm::radians(-y_angle), glm::vec3(1.0f, 0.0f, 0.0f));
			CameraRotation = glm::rotate(CameraRotation, glm::radians(x_angle), glm::vec3(0.0f, 1.0f, 0.0f));
			//CameraRotation = glm::rotate(CameraRotation, glm::radians(y_angle), glm::vec3(0.0f, 0.0f, 1.0f));

			if (mouse_down_position_x != event.motion.x || mouse_down_position_y != event.motion.y)
			{
					view = view * CameraRotation;
			}
			mouse_down_position_x = event.motion.x;
			mouse_down_position_y = event.motion.y;
		}

		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (size_t i = 0; i < building.getRule()->length(); i++)
		{
			MV_plane = building.getWallsMVs()[i];
			MVP = projection * view * MV_plane;
			N = glm::mat3(glm::inverse(glm::transpose(MV_plane)));
			glUniformMatrix4fv(MVP_address, 1, GL_FALSE, glm::value_ptr(MVP));
			glUniformMatrix4fv(MV_address, 1, GL_FALSE, glm::value_ptr(MV_plane));
			glUniformMatrix3fv(N_address, 1, GL_FALSE, glm::value_ptr(N));
			//if (building.getRule()->at(i)  =='l')
			glDrawArrays(GL_TRIANGLES, plane.getBufferIndex()/3, (plane.getAmountVertexData()/3));
//			if (building.getRule()->at(i) == 'r')
//				glDrawArrays(GL_TRIANGLES, R.getBufferIndex()/3, (R.getAmountVertexData()/3));
//			if (building.getRule()->at(i) == 'd')
//				glDrawArrays(GL_TRIANGLES, D.getBufferIndex()/3, (D.getAmountVertexData()/3));
//			if (building.getRule()->at(i) == 'u')
//				glDrawArrays(GL_TRIANGLES, U.getBufferIndex()/3, (U.getAmountVertexData()/3));
		}
		SDL_GL_SwapWindow(mainWindow.getWindow());
	}
	glDeleteProgram(test.getShaderProgram());
	return EXIT_SUCCESS;
}
