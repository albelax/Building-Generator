#include "Building.h"

Building::Building()
{
	m_walls_MV = glm::mat4(1.0f);
	generateBase();
}

void Building::generateBase()
{
	m_rule = "rurdruullldd"; // just for testing purpose
}

glm::mat4 * Building::getCurrentMV(int _index)
{
	// probably I need to take the index of the
	// plane, use the letter to modify the MV,
	// and return a pointer to the MV,
	// this way I can handle the drawing somewhere else
	bool closing = false;
	m_walls_MV = glm::mat4(1.0);

	for (int i = 0; i < m_rule.length(); ++i)
	{
		if (m_rule[i] == 'd') // Down
		{
			if (i > 0 && m_rule[i-1] == 'd')
			{
				m_walls_MV = glm::translate(m_walls_MV, glm::vec3(0,0,1));
			}
			else if (i > 0 && m_rule[i-1] == 'l')
			{
				m_walls_MV = glm::translate(m_walls_MV, glm::vec3(0.5,0,0.5));
				m_walls_MV = glm::rotate(m_walls_MV, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}
			else
			{
				m_walls_MV = glm::translate(m_walls_MV, glm::vec3(0.5,0,-0.5));
				m_walls_MV = glm::rotate(m_walls_MV, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}
			closing = true;
		}
		else if(m_rule[i] == 'r') // Right
		{
			if (i > 0 && m_rule[i-1] == 'r' )
			{
				m_walls_MV = glm::translate(m_walls_MV, glm::vec3(0,0,1));
			}
			else if(closing)
			{
				// in case the previous operation was l, it means the geometry is needs to rotate in the other direction
				m_walls_MV = glm::translate(m_walls_MV, glm::vec3(0.5f,0,0.5f));
				m_walls_MV = glm::rotate(m_walls_MV, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}
			else if (i != 0)
			{
				m_walls_MV = glm::translate(m_walls_MV, glm::vec3(-0.5f,0,0.5f));
				m_walls_MV = glm::rotate(m_walls_MV, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}
		}
		else if(m_rule[i] == 'u') // UP
		{
			closing = false;
			if (i > 0 && m_rule[i-1] == 'u')
			{
				// in case it is continuing in the same path, don't rotate, just keep going
				m_walls_MV = glm::translate(m_walls_MV, glm::vec3(0,0,1));
			}
			else if(closing)
			{
				// in case the previous operation was l, it means the geometry is needs to rotate in the other direction
				m_walls_MV = glm::translate(m_walls_MV, glm::vec3(-0.5f,0,0.5f));
				m_walls_MV = glm::rotate(m_walls_MV, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}
			else
			{
				m_walls_MV = glm::translate(m_walls_MV, glm::vec3(0.5f,0,0.5f));
				m_walls_MV = glm::rotate(m_walls_MV, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}
		}
		else if(m_rule[i] == 'l') // Left
		{
			// if the l operation is called it meand that the building is closing, some operations need to e inverted
			if (i > 0 && m_rule[i-1] == 'l')
			{
				m_walls_MV = glm::translate(m_walls_MV, glm::vec3(0,0,1));
			}
			else if (closing)
			{
				m_walls_MV = glm::translate(m_walls_MV, glm::vec3(-0.5f,0,0.5f));
				m_walls_MV = glm::rotate(m_walls_MV, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}
			else
			{
				m_walls_MV = glm::translate(m_walls_MV, glm::vec3(0.5f,0,0.5f));
				m_walls_MV = glm::rotate(m_walls_MV, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}
		}
		if (i == _index)
		{
			return &m_walls_MV;
		}

	}

}
