#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include "Mesh.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtc/type_ptr.hpp"
#include "ext.hpp"
#include "gtx/matrix_decompose.hpp"

/// \file Object.h
/// \brief this is the base class of all the elements of the building, and of the building itself
/// \author Alberto La Scala

class Object
{
public:
	/// \brief Object constructs an empty object
	Object();

	/// \brief Object creates an object using the a std::vector of transformation matrices
	Object(std::vector<glm::mat4> &_MVs);

	~Object();

	/// \brief getMVs returns a reference to the std::vector containing the transformation matrices
	const std::vector<glm::mat4> & getMVs() { return m_MVs; }

protected:
	/// \brief m_rotationTable is a table containing all the rotations of an object can have
	std::vector<std::vector<float>> m_rotationTable;

	/// \brief m_translationTable is a table containing all the translations of an object can have
	std::vector<std::vector<float>> m_translationTable;

	/// \brief m_MVs is the std::vector that contains the transformation matrices of this object
	std::vector<glm::mat4> m_MVs;

	/// \brief makeRotationTable makes the rotation table
	virtual void makeRotationTable();

	/// \brief makeTranslationTable makes the translation table
	virtual void makeTranslationTable();

	/// \brief m_bufferIndex is the index of the buffer that store its vertices
	int m_bufferIndex;
};

#endif
