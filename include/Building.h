#ifndef BUILDING_H
#define BUILDING_H

#include <vector>
#include <string>
#include <dirent.h>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Mesh.h"
#include "Object.h"
#include "Walls.h"
#include "Corner.h"
#include "Roof.h"

class Building : public Object
{
public:
	Building();
	const std::string * getRule() const { return &m_rule; }
	//const std::vector<float> &getVertices() const { return m_vertices; }
	float * getVertices() { return &m_vertices[0]; }
	float * getNormals() { return &m_normals[0]; }
	int amountVertices() { return m_vertices.size(); }
	void setBufferIndex(int _index) { m_bufferIndex = _index; }
	int getBufferIndex() const { return m_bufferIndex; }
private:
	/// rule that determines the shape of the base
	std::string m_rule;
	/// generating the rule that will make the walls
	void generateBase();
	void combinearrays(Mesh & _mesh, Object * _object);
	std::vector<float> m_vertices;
	std::vector<float> m_normals;
	int m_height;
};

#endif // BUILDING_H
