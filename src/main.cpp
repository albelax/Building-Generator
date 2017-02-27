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
#include "Mesh.h"
#include "Corner.h"

#define TRACKBALL 1

int main()
{
	int width = 800;
	int height = 600;
	bool mouse_down;
	int mouse_down_position_x;
	int mouse_down_position_y;

	Mesh plane("models/Walls/L.obj", "wall");
//	Mesh plane("models/Walls/Plane.obj", "wall");
	Mesh corner("models/Corners/b_Sharp_Corner_oriented.obj", "corner");
//	Mesh corner("models/Corners/b_oriented_cut_corner.obj", "corner");
	Mesh ship("models/ship2.obj", "ship");
	std::vector<Mesh*> meshes;
	meshes.resize(3);
	meshes[0] = &plane;
	meshes[1] = &corner;
	meshes[2] = &ship;
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

	int bufferSize = 0;
	std::vector<Mesh*>::iterator meshes_it;
	for (meshes_it = meshes.begin(); meshes_it != meshes.end(); ++meshes_it)
	{
		bufferSize += (*meshes_it)->getAmountVertexData();
	}

	Buffer buffer(bufferSize, sizeof(float)); // generate vbo buffer
	for (meshes_it = meshes.begin(); meshes_it != meshes.end(); ++meshes_it)
	{
		// push vertices of each mesh to the buffer assigning the corresponding index in the buffer to the mesh
		(*meshes_it)->setBufferIndex(buffer.append((void*) &(*meshes_it)->getVertexData(), (*meshes_it)->getAmountVertexData(), Buffer::VERTEX));
	}

	// pass the vertex data to the shader
	GLint pos = glGetAttribLocation(test.getShaderProgram(), "VertexPosition");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	for (meshes_it = meshes.begin(); meshes_it != meshes.end(); ++meshes_it)
	{
		(*meshes_it)->setBufferIndex(buffer.append((void*) &(*meshes_it)->getNormalsData(), (*meshes_it)->getAmountVertexData(), Buffer::NORMAL));
	}

	// pass the normals to the shader
	GLint n = glGetAttribLocation(test.getShaderProgram(), "VertexNormal");
	glEnableVertexAttribArray(n);
	glVertexAttribPointer(n, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// virtual camera
	glm::vec3 eye(0, 2.0f, -5.0f);
	glm::mat4 view = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projection = glm::perspective(glm::radians(60.0f),
	static_cast<float>(mainWindow.getWidth())/static_cast<float>(mainWindow.getHeight()), 0.1f, 100.0f);

	/// declare matrices adresses in the shader
	glm::mat4 MVP;	// MVP = projection * view * Model;
	glm::mat3 N;		// inverse transpose of MV
	GLint MVP_address = glGetUniformLocation(test.getShaderProgram(), "MVP");
	GLint MV_address = glGetUniformLocation(test.getShaderProgram(), "MV");
	GLint N_address = glGetUniformLocation(test.getShaderProgram(), "N"); // inverse transpose of MV

	Building building = Building();
	Corner corners = Corner(building);
	glViewport(0,0,width, height);
	SDL_Event event;
	bool quit = false;

	// main loop
	while (quit != true)
	{
		SDL_GetWindowSize(mainWindow.getWindow(), &width, &height);

		while (SDL_PollEvent(&event))
		{
	if (event.type == SDL_QUIT)// || event.key.keysym.sym == SDLK_ESCAPE)
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
				mouse_down = true;
				mouse_down_position_x = event.button.x;
				mouse_down_position_y = event.button.y;
			}
			if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT )
			{
				mouse_down = false;
			}
		}


		// attempt to make a trackball camera
	if (mouse_down && TRACKBALL)
	{
	glm::mat4 CameraRotation(glm::mat4(1.0f));

	float x_angle = (static_cast<float>(event.motion.x) - static_cast<float>(mouse_down_position_x))*0.1;

	//x_angle = x_angle > 1.0f ? 1.0f : x_angle;
	//x_angle = x_angle < -1.0f ? -1.0f : x_angle;

	//float y_angle = (static_cast<float>(event.motion.y) - static_cast<float>(mouse_down_position_y))*0.1;
	CameraRotation = glm::rotate(CameraRotation, glm::radians(x_angle), glm::vec3(0.0f, 1.0f, 0.0f));
	//CameraRotation = glm::rotate(CameraRotation, glm::radians(y_angle), glm::vec3(0.0f, 0.0f, 1.0f));

	if (mouse_down_position_x != event.motion.x || mouse_down_position_y != event.motion.y)
	{
	//std::cout << x_angle << '\n';
	view = view * CameraRotation;
}
	mouse_down_position_x = event.motion.x;
	mouse_down_position_y = event.motion.y;
}
	else if(!TRACKBALL)
	{
	float x_angle = 0.5f;
	glm::mat4 CameraRotation(glm::mat4(1.0f));
	CameraRotation = glm::rotate(CameraRotation, glm::radians(x_angle), glm::vec3(0.0f, 1.0f, 0.0f));
	view = view * CameraRotation;
}

		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (size_t i = 0; i < building.getWallsMVs().size(); i++)
	{
		MVP = projection * view * building.getWallsMVs()[i];
		N = glm::mat3(glm::inverse(glm::transpose(building.getWallsMVs()[i])));
		glUniformMatrix4fv(MVP_address, 1, GL_FALSE, glm::value_ptr(MVP));
		glUniformMatrix4fv(MV_address, 1, GL_FALSE, glm::value_ptr(building.getWallsMVs()[i]));
		glUniformMatrix3fv(N_address, 1, GL_FALSE, glm::value_ptr(N));
		glDrawArrays(GL_TRIANGLES, plane.getBufferIndex()/3, (plane.getAmountVertexData()/3));
	}

	if (corners.getCornersMVs().size() > 0)
	for (size_t i = 0; i < corners.getCornersMVs().size(); ++i)
	{
		MVP = projection * view * corners.getCornersMVs()[i];
		N = glm::mat3(glm::inverse(glm::transpose(corners.getCornersMVs()[i])));
		glUniformMatrix4fv(MVP_address, 1, GL_FALSE, glm::value_ptr(MVP));
		glUniformMatrix4fv(MV_address, 1, GL_FALSE, glm::value_ptr(corners.getCornersMVs()[i]));
		glUniformMatrix3fv(N_address, 1, GL_FALSE, glm::value_ptr(N));
		glDrawArrays(GL_TRIANGLES, corner.getBufferIndex()/3, (corner.getAmountVertexData()/3));
	}

	SDL_GL_SwapWindow(mainWindow.getWindow());
	}
	glDeleteProgram(test.getShaderProgram());
	return EXIT_SUCCESS;
}
