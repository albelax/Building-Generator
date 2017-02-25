#ifndef BUILDING_H
#define BUILDING_H

#include <vector>
#include <string>
#include "Mesh.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"

class Building
{
public:
	Building();
	const std::string * getRule() const {return &m_rule;}
	const std::vector<glm::mat4> & getWallsMVs() {return m_walls_MVs;}
private:
	/// spacing to translate each wall etc
	float m_padding;
	/// rule that determines the shape of the base
	std::string m_rule;
	/// pointer to the mesh that will be drawn as a wall
	//Mesh * m_wall;
	/// vector that contains all the matrices (MV) of each wall
	std::vector<glm::mat4> m_walls_MVs;
	std::vector<std::vector<int>> m_rotationTable;
	std::vector<std::vector<float>> m_translationTable;
	void generateBase();
	void generateMVs();
	void makeRotationTable();
	void makeTranslationTable();
};

#endif // BUILDING_H
