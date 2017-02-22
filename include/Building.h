#ifndef BUILDING_H
#define BUILDING_H

#include "Mesh.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"
class Building
{

private:
	std::string m_rule;
	/// m_MV used to move the
	glm::mat4 m_MV;
	glm::mat4 m_walls_MV; // used just to draw the walls
	Mesh * m_wall;
	void generateBase();
public:
	Building();
	void draw();
	const std::string * const getRule() {return &m_rule;}
	glm::mat4 *getCurrentMV(int _index);
};

#endif // BUILDING_H
