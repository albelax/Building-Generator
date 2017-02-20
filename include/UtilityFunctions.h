#ifndef UTILITYFUNCTIONS_H
#define UTILITYFUNCTIONS_H


#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif

#ifdef linux
#include <GL/glew.h>
#endif

//#else
//#include <GL/glew.h>
//#endif

#include <string>
#include <vector>
#include <array>
#include <ios>
#include <iostream>
#include <fstream>
#include <random>
//#include "GLFunctions.h"

/// \class UtilityFunctions
/// \brief is a bundle of functions that helped me in the developement process

class utilityFunctions
{
public:
	utilityFunctions() = delete;
	static std::vector<std::string> split(const std::string _stringIn, char _splitChar = ' ');
	static std::vector<std::string> loadFromFile(const std::string _fileName);
	static std::vector<std::array<float,3>> getVertices(const std::vector<std::string> _vectorIn);
	static std::vector<std::vector<std::string>> getFaces(const std::vector<std::string> _vectorIn);
	static std::vector<std::array<float,3>> getNormals(const std::vector<std::string> _vectorIn);
	static void DrawFaces(const std::vector<std::array<float,3>> _vertices, const std::vector<std::array<float,3>> _normals, std::vector<std::vector<std::string>> _faces, float size);
	static void DrawSimpleLevel();
	static std::vector<char> generateObstacles(const int _x, const int _z);
	static void DrawLevel(const std::vector<char> _map, const int _x);
};

#endif // UTILITYFUNCTIONS_H


