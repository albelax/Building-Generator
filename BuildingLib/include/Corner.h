#ifndef CORNER_H
#define CORNER_H

#include <vector>
#include <string>
#include "Mesh.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "ext.hpp"
#include "Walls.h"
#include "Object.h"


class Corner : public Object
{
public:
	Corner() = delete;
	Corner(Walls &_base);
	const std::vector<glm::mat4> & getCornersMVs() { return m_MVs; }
private:
	void makeRotationTable() override;
	void makeTranslationTable() override;
};

#endif // CORNER_H