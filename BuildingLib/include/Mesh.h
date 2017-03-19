#ifndef MESH_H
#define MESH_H

///**********************************PLEASE NOTICE*****************************************///
///****************************************************************************************///
///*																																										  *///
///*	THIS CLASS WAS INITIALLY DESIGNED FOR THE SECOND PPP ASSIGNMENT (FIRST YEAR)        *///
///*	MINOR TWEAKS WERE MADE TO THIS CLASS BUT OVERALL IT SHOUDLN'T BE MARKED             *///
///*	AS IT HAS ALREADY BEEN SUBMITTED AND MARKED																			    *///
///*	EXEPT FOR THE TWO "WRITE" FUNCTIONS WHICH WERE ADDED SPECIFICALLY FOR THIS PROJECT  *///
///*																																										  *///
///*  original file: https://github.com/albelax/PPP_Assignment2/blob/master/src/Mesh.cpp  *///
///*																																										  *///
///****************************************************************************************///
///****************************************************************************************///

#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "glm.hpp"

/// \class Mesh
/// \brief contains the data imported from a .OBJ file

class Mesh
{
private:
	std::vector<float> m_vertices;
	std::vector<float> m_normals;
	std::vector<std::string> m_address;
	float m_minX, m_minY, m_minZ, m_maxX, m_maxY, m_maxZ;
	std::string m_name;
	int m_bufferIndex;
	std::vector<std::string> split(std::string _stringIn, char _splitChar = ' ');
public:
	Mesh(std::string _address, std::string _name);
	Mesh();
	void setBufferIndex(int _index) { m_bufferIndex = _index; }
	void draw() const;
	glm::vec4 min()const { return glm::vec4(m_minX,m_minY,m_minZ,1); }
	glm::vec4 max()const { return glm::vec4(m_maxX,m_maxY,m_maxZ,1); }
	const std::vector<float> &getVertices() const { return m_vertices; }
	std::vector<float> getCopy() const { return m_vertices; }
	std::vector<float> getCopyNormal() const { return m_normals; }
	const std::vector<float> &getNormals() const { return m_normals; }
	size_t getAmountVertexData() const { return static_cast<int>(m_vertices.size()); }
	int getBufferIndex() const { return m_bufferIndex; }
	std::string name() const { return m_name; }
	std::vector<std::string> getAddress() { return m_address; }
	void write(std::string _destination);
	static void write(const std::vector<float> &_vertices, const std::vector<float> &_normals, std::string _destination);
};

#endif // MESH_H
