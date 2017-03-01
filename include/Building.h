#ifndef BUILDING_H
#define BUILDING_H

#include <vector>
#include <string>
#include "Mesh.h"
#include "Object.h"
#include "Walls.h"
#include "Corner.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"

class Building : public Object
{
public:
	Building();
	const std::string * getRule() const { return &m_rule; }
private:
	/// rule that determines the shape of the base
	std::string m_rule;
	/// generating the rule that will make the walls
	void generateBase();
	Walls m_walls;
	Corner m_corners;
};

#endif // BUILDING_H
