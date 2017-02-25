#include "Corner.h"

Corner::Corner(Building &_base)
{
	makeRotationTable();
	makeTranslationTable();
	m_corners_MVs.resize(_base.getRule()->length());
	std::vector<glm::mat4>::iterator it = m_corners_MVs.begin();
	for (size_t i = 0; i < _base.getRule()->length(); ++i)
	{
		unsigned int column;
		unsigned int row;
		char next = i == _base.getRule()->length()-1 ? (*_base.getRule())[0] : (*_base.getRule())[i+1];
		switch((*_base.getRule())[i])
		{
			case 'r': column = 0; break;
			case 'u': column = 1; break;
			case 'l': column = 2; break;
			case 'd': column = 3; break;
		}
		switch (next)
		{
			case 'r': row = 0; break;
			case 'u': row = 1; break;
			case 'l': row = 2; break;
			case 'd': row = 3; break;
		}

		if (m_rotationTable[column][row] != 1.0f)
		{
			glm::mat4 tmp(1.0f);
			// in case the angle is concave use the first one else the other one, you may want to use different meshes
			if (m_translationTable[column][row] == 0)
				tmp = glm::translate(_base.getWallsMVs()[i], glm::vec3(0.15f,0,0.6495f));
			else if (m_translationTable[column][row] == 1)
				tmp = glm::translate(_base.getWallsMVs()[i], glm::vec3(-0.150f,0,0.6495f));
			(*it) = glm::rotate(tmp, glm::radians(m_rotationTable[column][row]), glm::vec3(0.0f, 0.1f, 0.0f));
			it++;
		}
	}
	m_corners_MVs.resize(std::distance(m_corners_MVs.begin(), it));
}

void Corner::makeRotationTable()
{
	// 1 ignore
	// 0 default
	m_rotationTable =
	{
	{1.0f, 0.0f, 1.0f, 90.0f},	// r
	{90.0f, 1.0f, 0.0f, 1.0f},	// u
	{1.0f, 90.0f, 1.0f, 0.0f},	// l
	{0.0f, 1.0f, 90.0f, 1.0f}		// d
	};
}

void Corner::makeTranslationTable()
{
	// 0 if standard everythong else is an exception
	m_translationTable =
	{
	{0.0f, 0.0f, 0.0f, 1.0f},	// r
	{1.0f, 0.0f, 0.0f, 0.0f},	// u
	{0.0f, 1.0f, 0.0f, 0.0f},	// l
	{0.0f, 0.0f, 1.0f, 0.0f}	// d
	};

}

