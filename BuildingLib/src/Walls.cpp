#include "Walls.h"

Walls::Walls(std::string _rule)
{
	m_padding = 1.5f;
	m_rule = _rule;
	m_MVs.resize(m_rule.length());
	makeRotationTable();
	makeTranslationTable();
	generateMVs();
}


void Walls::makeRotationTable()
{
	/// this matrix contains all the rotations of the rule,
	/// it shows what how each component rotates based on the previous one
	/// each row represent the interaction of one element with the others

	//		r	u	l	d
	// r	x	x	x	x
	// u	x	x	x	x
	// l	x	x	x	x
	// d	x	x	x	x

	m_rotationTable =
	{
	{0, -90, 90, 90}, // r
	{90, 0, -90, 90}, // u
	{90, 90, 0, -90},	// l
	{-90, 90, 90, 0}	// d
	};
}

void Walls::makeTranslationTable()
{
	m_translationTable =
	{
	{1, -m_padding, m_padding, m_padding}, // r
	{m_padding, 1, -m_padding, m_padding}, // u
	{m_padding, m_padding, 1, -m_padding}, // l
	{-m_padding, m_padding, m_padding, 1}	 // d
	};
}

void Walls::generateMVs()
{
	glm::mat4 wall_MV = glm::mat4(1.0);
	unsigned int column;
	unsigned int row;
	float totX = 0.f;
	float totZ = 0.f;

	for (size_t i = 1; i < m_rule.length(); ++i)
	{
		switch(m_rule[i])
		{
			case 'r': column = 0; break;
			case 'u': column = 1; break;
			case 'l': column = 2; break;
			case 'd': column = 3; break;
		}
		switch (m_rule[i-1])
		{
			case 'r': row = 0; break;
			case 'u': row = 1; break;
			case 'l': row = 2; break;
			case 'd': row = 3; break;
		}
		// if the column is the same as the

		float x = m_translationTable[column][row] == 1 ? 0 : m_translationTable[column][row];
		float correction_x = 0;
		if (x != 0)
			correction_x = x > 0 ? -.5f : 0.5f;
		wall_MV = glm::translate(wall_MV, glm::vec3(x, 0, m_padding));
		wall_MV = glm::translate(wall_MV, glm::vec3(correction_x, 0, -0.5f));

		totX = std::max(totX, wall_MV[3][0]);
		totZ = std::max(totZ, wall_MV[3][2]);
		wall_MV = glm::rotate(wall_MV, glm::radians(static_cast<float>(m_rotationTable[column][row])), glm::vec3(0,1.0f,0));

		m_MVs[i] = wall_MV;
	}

	// makes sure that the building is always in the middle of the scene
	for (size_t i = 0; i < m_MVs.size(); ++i)
	{
		m_MVs[i] = glm::translate(glm::mat4(1.0),glm::vec3(-totX/2.f, 0.f, -totZ/2.f))*m_MVs[i];
	}
}
