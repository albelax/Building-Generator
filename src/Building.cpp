#include "Building.h"

Building::Building()
{
	//m_walls_MV = glm::mat4(1.0f);
	generateBase();
	m_walls_MVs.resize(m_rule.length());
	generateMVs();
}

void Building::generateBase()
{
	// for now I just have a few rules,
	// maybe at some point I will actually generate them randomly
	m_rule = "ruld";
	m_rule = "rruuldld";
	m_rule = "rurdruullldd";
	m_rule = "rurdruuuldlulddd";
}

void Building::generateMVs()
{
	glm::mat4 wall_MV = glm::mat4(1.0);

	for (size_t i = 0; i < m_rule.length(); ++i)
	{
		switch(m_rule[i])
		{
			case 'd':
				std::cout << "D" << '\n';
				if(i > 0 && m_rule[i-1] == 'd')
					wall_MV = glm::translate(wall_MV, glm::vec3(0,0,1));
				else if (i > 0 && m_rule[i-1] == 'r') // one of the few special cases where the corner needs to be -90
				{
					wall_MV = glm::translate(wall_MV, glm::vec3(-0.5,0,0.5));
					wall_MV = glm::rotate(wall_MV, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else
					{
						wall_MV = glm::translate(wall_MV, glm::vec3(0.5,0,0.5));
						wall_MV = glm::rotate(wall_MV, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					}
				break;

			case 'r':
				std::cout << "R" << '\n';
				if(i > 0 && m_rule[i-1] == 'r')
					wall_MV = glm::translate(wall_MV, glm::vec3(0,0,1));
				else if (i > 0 && m_rule[i-1] == 'u') // one of the few special cases where the corner needs to be -90
				{
					wall_MV = glm::translate(wall_MV, glm::vec3(-0.5,0,0.5));
					wall_MV = glm::rotate(wall_MV, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else
					{
						wall_MV = glm::translate(wall_MV, glm::vec3(0.5,0,0.5));
						wall_MV = glm::rotate(wall_MV, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					}
				break;

			case 'l':
				std::cout << "L" << '\n';
				if(i > 0 && m_rule[i-1] == 'l')
					wall_MV = glm::translate(wall_MV, glm::vec3(0,0,1));
				else if (i > 0 && m_rule[i-1] == 'd') // one of the few special cases where the corner needs to be -90
				{
						wall_MV = glm::translate(wall_MV, glm::vec3(-0.5,0,0.5));
						wall_MV = glm::rotate(wall_MV, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else
				{
						wall_MV = glm::translate(wall_MV, glm::vec3(0.5,0,0.5));
						wall_MV = glm::rotate(wall_MV, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				}
				break;

			case 'u':
				std::cout << "U" << '\n';
				if(i > 0 && m_rule[i-1] == 'u')
					wall_MV = glm::translate(wall_MV, glm::vec3(0,0,1));
				else if (i > 0 && m_rule[i-1] == 'l') // one of the few special cases where the corner needs to be -90
				{
						wall_MV = glm::translate(wall_MV, glm::vec3(-0.5,0,0.5));
						wall_MV = glm::rotate(wall_MV, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else
					{
						wall_MV = glm::translate(wall_MV, glm::vec3(0.5,0,0.5));
						wall_MV = glm::rotate(wall_MV, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					}
				break;
		}
		m_walls_MVs[i] = wall_MV;
	}
}
