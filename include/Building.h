#ifndef BUILDING_H
#define BUILDING_H

#include "Mesh.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>

class Building
{
public:
	Building();
	void draw();
	const std::string * getRule() const {return &m_rule;}
	const std::vector<glm::mat4> & getWallsMVs() {return m_walls_MVs;}
	const glm::mat4 & getMV() const {return  m_MV;}
private:
	/// spacing to translate each wall etc
	float m_padding;
	/// rule that determines the shape of the base
	std::string m_rule;
	Mesh * m_wall;
	/// m_MV used to move the base
	glm::mat4 m_MV;
	std::vector<glm::mat4> m_walls_MVs;
	std::vector<std::vector<int>> m_rotationTable;
	std::vector<std::vector<float>> m_translationTable;
	void generateBase();
	void generateMVs();
};

#endif // BUILDING_H
