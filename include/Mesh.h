#ifndef MESH_H
#define MESH_H


#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "UtilityFunctions.h"
#include "glm.hpp"


/// \class Mesh
/// \brief contains the data imported from a .OBJ file

class Mesh
{
private:
	std::vector<float> m_vertices;
	std::vector<float> m_normals;
	float m_minX, m_minY, m_minZ, m_maxX, m_maxY, m_maxZ;
	std::string m_name;
	int m_bufferIndex;
public:
	Mesh(std::string _address, std::string _name);
	Mesh();
	void setBufferIndex(int _index) { m_bufferIndex = _index; }
	void draw() const;
	glm::vec4 min()const { return glm::vec4(m_minX,m_minY,m_minZ,1); }
	glm::vec4 max()const { return glm::vec4(m_maxX,m_maxY,m_maxZ,1); }
	const float &getVertexData() const { return m_vertices[0]; }
	const float &getNormalsData() const { return m_normals[0]; }
	int getAmountVertexData() const { return static_cast<int>(m_vertices.size()); }
	int getBufferIndex() const { return m_bufferIndex; }
	std::string name() const { return m_name; }


};

#endif // MESH_H
