#include "UtilityFunctions.h"

std::vector<std::string> utilityFunctions::split(std::string _stringIn, char _splitChar)
{
	int count = 0;
	std::string tempString;
	std::vector<std::string> retVector;
	for (int i = 0; i < static_cast<int>(_stringIn.length()+1); i++)
	{
		if(tempString.length() < 1 && _stringIn[i] == ' ') continue;
		else if(_stringIn[i] != _splitChar)
		{
			tempString += _stringIn[i];
		}
		else
		{
			retVector.resize(retVector.size() + 1);
			retVector[count] = tempString;
			tempString.clear();
			count++;
		}

		if(i == static_cast<int>(_stringIn.length()))
		{
			retVector.resize(retVector.size()+1);
			retVector[count] = tempString;
			tempString.clear();
			count++;
		}
	}
	return retVector;
}

//----------------------------------------------------------------------------------------------------------------------

std::vector<std::string> utilityFunctions::loadFromFile(std::string _fileName)
{
	/*
	 * reads from a file and returns a vector containing all the lines of the file
	 */
	std::vector<std::string> Lines;
	std::ifstream Source;
	Source.open(_fileName);
	Source.seekg(0, std::ios::end); // starts from the beginning and goes to the end of the file to know the lenght of it
	int len = (int)Source.tellg();
	Source.seekg(0);
	int count = 0;

	while (len > 0)
	{
		std::string line;
		getline(Source, line);
		if (line != "")
		{
			if(static_cast<int>(Lines.size()) <= count)
				Lines.resize(Lines.size()+1);
			Lines[count] = line;
			++count;
		}
		--len;
		line.clear();
	}
	Source.close();
	return Lines;
}

//----------------------------------------------------------------------------------------------------------------------

std::vector<std::array<float,3>> utilityFunctions::getVertices(std::vector<std::string> _vectorIn)
{
	std::vector<std::array<float,3>> vertices;
	std::vector<std::string> tempData;
	int count = 0;

	for (int i = 0; i < static_cast<int>(_vectorIn.size()); ++i)
	{
		tempData = split(_vectorIn[i],' ');
		if (tempData[0] == "v")
		{
			if(static_cast<int>(vertices.size()) >= count)
				vertices.resize(vertices.size() + 1);
			for (int j = 0; j < static_cast<int>(tempData.size()-1); ++j) // -1 to avoid overflow
			{
				vertices[count][j] = std::stof(tempData[j+1]); // +1 because the first is a letter, v for example
			}
			tempData.clear();
			++count;
		}
	}
	return vertices;
}

//----------------------------------------------------------------------------------------------------------------------

std::vector<std::array<float,3>> utilityFunctions::getNormals(std::vector<std::string> _vectorIn)
{
	std::vector<std::array<float,3>> normals;
	std::vector<std::string> tempData;
	int count = 0;

	for (int i = 0; i < static_cast<int>(_vectorIn.size()); ++i)
	{
		tempData = split(_vectorIn[i],' ');
		if (tempData[0] == "vn")
		{
			if(static_cast<int>(normals.size()) >= count)
				normals.resize(normals.size() + 1);
			for (int j = 0; j < static_cast<int>(tempData.size()-1); ++j) // -1 to avoid overflow
			{
				normals[count][j] = std::stof(tempData[j+1]); // +1 because the first is a letter, v for example
			}
			tempData.clear();
			++count;
		}
	}
	return normals;
}

//----------------------------------------------------------------------------------------------------------------------

std::vector<std::vector<std::string>> utilityFunctions::getFaces(const std::vector<std::string> _vectorIn)
{
	std::vector<std::vector<std::string>> faces;
	std::vector<std::string> tempFaces;
	std::vector<std::string> tempLine;
	int count = 0;

	for (int i = 0; i < static_cast<int>(_vectorIn.size()); ++i)
	{
		tempLine = split(_vectorIn[i],' ');
		if (tempLine[0] == "f")
		{
			for (int j = 0; j < static_cast<int>(tempLine.size()-1); ++j)
			{
				tempFaces.push_back(tempLine[j+1]);
			}
			faces.push_back(tempFaces);
			tempFaces.clear();
			++count;
		}
	}
	return faces;
}

//----------------------------------------------------------------------------------------------------------------------

//void utilityFunctions::DrawFaces(const std::vector<std::array<float,3>> _vertices, const std::vector<std::array<float,3>> _normals, std::vector<std::vector<std::string>> _faces, float size)
//{
//	std::vector<std::string> tempFace;
//	glBegin(GL_TRIANGLES);
//	for (int i = 0; i < static_cast<int>(_faces.size()); ++i)
//	{
//		for (int j = 0; j < static_cast<int>(_faces[i].size());++j)
//		{
//			tempFace = split(_faces[i][j],'/');
//			// -1 to convert from index 1 (the way vertices are stored in the list of faces) to index 0
//			glNormal3f(_normals[std::stoi(tempFace[2]) - 1][0]*size, _normals[std::stoi(tempFace[2]) -1][1]*size,_normals[std::stoi(tempFace[2]) -1][2]*size);
//			glVertex3f(_vertices[std::stof(tempFace[0]) - 1][0]*size, _vertices[std::stoi(tempFace[0]) - 1][1]*size,_vertices[std::stoi(tempFace[0]) - 1][2]*size);
//			//std::cout << _vertices[std::stoi(tempFace[0])][0] << " " << _vertices[std::stoi(tempFace[0])][1] << " " << _vertices[std::stoi(tempFace[0])][2] << "\n";
//		}
//	}
//	glEnd();
//}

//----------------------------------------------------------------------------------------------------------------------

std::vector<char> utilityFunctions::generateObstacles(const int _x, const int _z)
{
	std::vector<char> map;
	srand((int)time(NULL));

	for (int i = 0; i < _x*_z; ++i)
	{
		int s = rand()%3;
		if (s == 0)
			map.push_back('0');
		else
			map.push_back('1');
	}
	return map;
}

//----------------------------------------------------------------------------------------------------------------------

//void utilityFunctions::DrawSimpleLevel()
//{
//	for (int i=0; i <= 2; ++i)
//	{
//		for (int j = 0; j <=2; ++j)
//		{
//			glColor3f(1, 0, 0);
//			glPushMatrix();
//			glTranslatef(-2 + j*2, 1, -2 + i*2);
//			GLFunctions::cube(0.5,2,0.5);
//			glPopMatrix();
//		}
//	}
//}

//----------------------------------------------------------------------------------------------------------------------

//void utilityFunctions::DrawLevel(const std::vector<char> _map, const int _x)
//{
//	float x = 0;
//	float z = 0;
//	float spread = 1.5f;

//	glColor3f(1, 0, 0);
//	for (int i = 0; i < _map.size(); ++i)
//	{
//		if ( i % _x == 0)
//		{
//			z += spread;
//			x = 0;
//		}
//		if (_map[i] == '0')
//			{
//				glPushMatrix();
//				glTranslatef(-10 + x*2, 1, -10+ z*2);
//				GLFunctions::cube(1.5f,2,1.5f);
//				glPopMatrix();
//			}
//		x += spread;
//	}
//}

//----------------------------------------------------------------------------------------------------------------------
