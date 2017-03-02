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

	Mesh plane("models/Walls/L.obj", "wall");
//	Mesh plane("models/Walls/Plane.obj", "wall");
//	Mesh corner("models/Corners/b_Sharp_Corner_oriented.obj", "corner");
	Mesh corner("models/Corners/b_oriented_cut_corner.obj", "corner");
	std::vector<Mesh*> meshes;
	meshes.resize(2);
	meshes[0] = &plane;
	meshes[1] = &corner;
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

	Walls walls = Walls("rrrurruuuuldlluldddddd");
	Corner corners = Corner(walls);
	glViewport(0,0,width, height);

	std::vector<float> originalVertices = plane.getCopy();
	std::vector<float> originalNormals = plane.getCopyNormal();
	std::vector<float> transformedVertices;
	std::vector<float> transformedNormals;

	transformedVertices.resize(originalVertices.size()*walls.getMVs().size());
	transformedNormals.resize(originalNormals.size()*walls.getMVs().size());

	std::vector<float>::iterator vertices_it = transformedVertices.begin();
	std::vector<float>::iterator normals_it = transformedNormals.begin();

	for (auto mv : walls.getMVs())
	{
		for (size_t i = 0; i < originalVertices.size()/3; ++i)
		{
			glm::vec4 _tmp_vertices(originalVertices[i*3], originalVertices[i*3+1],originalVertices[i*3+2], 1);
			glm::vec4 _tmp_normals(originalNormals[i*3], originalNormals[i*3+1],originalNormals[i*3+2], 1);
			_tmp_vertices = mv * _tmp_vertices;
			_tmp_normals = /*mv **/ _tmp_normals;
			//			_tmp_normals = glm::normalize(_tmp_normals);

			(*vertices_it++) = (_tmp_vertices.x);
			(*vertices_it++) = (_tmp_vertices.y);
			(*vertices_it++) = (_tmp_vertices.z);

			(*normals_it++) = (_tmp_normals.x);
			(*normals_it++) = (_tmp_normals.y);
			(*normals_it++) = (_tmp_normals.z);
		}
	}

	int bufferSize = transformedVertices.size();
	std::vector<Mesh*>::iterator meshes_it;
//	for (meshes_it = meshes.begin(); meshes_it != meshes.end(); ++meshes_it)
//	{
//		bufferSize += (*meshes_it)->getAmountVertexData();
//	}

	Buffer buffer(bufferSize, sizeof(float)); // generate vbo buffer
	buffer.append((void *) &transformedVertices[0], transformedVertices.size(), Buffer::VERTEX);
//	for (meshes_it = meshes.begin(); meshes_it != meshes.end(); ++meshes_it)
//	{
//		// push vertices of each mesh to the buffer assigning the corresponding index in the buffer to the mesh
//		std::vector<float> tmp = (*meshes_it)->getCopy();
//		(*meshes_it)->setBufferIndex(buffer.append((void*) &tmp[0], (*meshes_it)->getAmountVertexData(), Buffer::VERTEX));
//	}


	// pass the vertex data to the shader
	GLint pos = glGetAttribLocation(test.getShaderProgram(), "VertexPosition");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	buffer.append((void *) &transformedNormals[0], transformedNormals.size(), Buffer::NORMAL);
//	for (meshes_it = meshes.begin(); meshes_it != meshes.end(); ++meshes_it)
//	{
//		(*meshes_it)->setBufferIndex(buffer.append((void*) &(*meshes_it)->getNormalsData(), (*meshes_it)->getAmountVertexData(), Buffer::NORMAL));
//	}

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
	mainCamera.setEye(0.0f, 2.0f, -5.0f);

	SDL_Event event;
	bool quit = false;

	// main loop
	while (quit != true)
	{
		SDL_GetWindowSize(mainWindow.getWindow(), &width, &height);

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) // || event.key.keysym.sym == SDLK_ESCAPE)
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

			if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
				mainCamera.handleMouseClick(event.button.x, event.button.y, event.button.button, event.type, 0);
			mainCamera.handleMouseMove(event.button.x, event.button.y);
		}

		mainCamera.update();
		glClearColor(1.0f,1.0f,1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//	for (size_t i = 0; i < building.getWallsMVs().size(); i++)
//	{
		MVP = projection * mainCamera.viewMatrix() * glm::mat4(1.0f);
		N = glm::mat3(glm::inverse(glm::transpose(glm::mat4(1.0f))));
		glUniformMatrix4fv(MVP_address, 1, GL_FALSE, glm::value_ptr(MVP));
		glUniformMatrix4fv(MV_address, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		glUniformMatrix3fv(N_address, 1, GL_FALSE, glm::value_ptr(N));
		glDrawArrays(GL_TRIANGLES, 0, transformedVertices.size()/3);
//	}

//	if (corners.getCornersMVs().size() > 0)
//	for (size_t i = 0; i < corners.getCornersMVs().size(); ++i)
//	{
//		MVP = projection * mainCamera.viewMatrix() * corners.getCornersMVs()[i];
//		N = glm::mat3(glm::inverse(glm::transpose(corners.getCornersMVs()[i])));
//		glUniformMatrix4fv(MVP_address, 1, GL_FALSE, glm::value_ptr(MVP));
//		glUniformMatrix4fv(MV_address, 1, GL_FALSE, glm::value_ptr(corners.getCornersMVs()[i]));
//		glUniformMatrix3fv(N_address, 1, GL_FALSE, glm::value_ptr(N));
//		glDrawArrays(GL_TRIANGLES, corner.getBufferIndex()/3, (corner.getAmountVertexData()/3));
//	}

	SDL_GL_SwapWindow(mainWindow.getWindow());
	}
	glDeleteProgram(test.getShaderProgram());
	return EXIT_SUCCESS;
}
