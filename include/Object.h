#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include "Mesh.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"

class Object
{
public:
	Object();
	~Object();
	const std::vector<glm::mat4> & getMVs() { return m_MVs; }
protected:
	std::vector<std::vector<float>> m_rotationTable;
	std::vector<std::vector<float>> m_translationTable;
	std::vector<glm::mat4> m_MVs;
	virtual void makeRotationTable();
	virtual void makeTranslationTable();
	int m_bufferIndex;
};

#endif
