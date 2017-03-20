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

/// \file Corner.h
/// \brief this class creates the corners of the building,
/// it uses the rule of the building and the transformation matrices of the walls
/// to place the corners where it's needed
/// \author Alberto La Scala

class Corner : public Object
{
public:
	Corner();

	/// \brief Corner constructs the corners using transformation matrices
	Corner(std::vector<glm::mat4> &_MVs);

	/// \brief Corner constructs the corners using the walls of the building
	Corner(Walls &_base);

	/// \brief getCornersMVs return a reference to the std::vector containing the transformation matrices of the corners
	const std::vector<glm::mat4> & getCornersMVs() { return m_MVs; }

	/// \brief getExceptions returns the list of the exceptions, the size of the std::vector is the same as the matrices,
	/// each index corresponds to a matrix, if it's 1 is an exception otherwise it isn't
	std::vector<int> getExceptions() { return m_isException; }

	void clear() override;
private:

	///
	/// \brief makeRotationTable stores the rotation table
	void makeRotationTable() override;

	/// \brief makeTranslationTable stores the translation table
	void makeTranslationTable() override;

	/// \brief m_isException stores 1 for exceptional and 0 for normal corners
	std::vector<int> m_isException;
};

#endif // CORNER_H
