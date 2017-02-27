#ifndef CORNER_H
#define CORNER_H

#include <vector>
#include <string>
#include "Mesh.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/matrix_decompose.hpp"
//#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext.hpp"
#include "Building.h"

class Corner
{
public:
	Corner() = delete;
	Corner(Building &_base);
	const std::vector<glm::mat4> & getCornersMVs() {return m_corners_MVs;}
private:
	std::vector<std::vector<float>> m_rotationTable;
	std::vector<std::vector<float>> m_translationTable;
	std::vector<glm::mat4> m_corners_MVs;
	//void generateCorners();
	void makeRotationTable();
	void makeTranslationTable();
};

#endif // CORNER_H
