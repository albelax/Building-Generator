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
	const std::string * getRule() {return &m_rule;}
	const std::vector<glm::mat4> & getWallsMVs() {return m_walls_MVs;}
private:
	/// rule that determines the shape of the base
	std::string m_rule;
	/// m_MV used to move the base
	glm::mat4 m_MV;
	Mesh * m_wall;
	std::vector<glm::mat4> m_walls_MVs;
	void generateBase();
	void generateMVs();
};

#endif // BUILDING_H
