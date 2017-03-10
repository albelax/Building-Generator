#include "Roof.h"
#include <iostream>
#include <iomanip>

Roof::Roof()
{

}

Roof::Roof(Object _walls, Object _corners)
{
	m_MVs = _walls.getMVs();
	for (auto m : _corners.getMVs())
	{
		m_MVs.push_back(m);
	}
	sortEdges();
	fill();
}


void Roof::makeTranslationTable()
{
	m_translationTable =
	{
	{0,0,0,0}, // r
	{0,0,0,0}, // u
	{0,0,0,0}, // l
	{0,0,0,0}	 // d
	};
}

float Roof::round(float _in)
{
	int integerPart = trunc(_in);
	float decimalPart = _in - integerPart;
	if (fabs(decimalPart) < 0.3f )
	decimalPart = 0;
	else if (fabs(decimalPart) > 0.3f && fabs(decimalPart) < 0.6f)
		decimalPart = 0.5f;
	else decimalPart = 1;
	decimalPart = _in > 0 ? decimalPart : -decimalPart;
	return integerPart + decimalPart;
}


void Roof::sortEdges()
{
	std::vector<glm::mat4> matrices;
	for (unsigned int i = 0; i < m_MVs.size(); ++i)
	{
		glm::vec3 mv_scale(1.0f);
		glm::quat mv_rotation;
		glm::vec3 mv_translation(1.0f);
		glm::vec3 mv_skew(1.0f);
		glm::vec4 mv_perspective(1.0f);
		glm::decompose(m_MVs[i], mv_scale, mv_rotation, mv_translation, mv_skew, mv_perspective);
		// sort matrices by Z
		bool isIn = false;
		for (auto m : matrices)
		{
			if (m_MVs[i] == m)
				isIn = true;
		}
		if (!isIn)
		{
			matrices.push_back(m_MVs[i]);
		}
		for (unsigned int j = i+1; j < m_MVs.size(); ++j)
		{
			glm::vec3 j_scale(1.0f);
			glm::quat j_rotation;
			glm::vec3 j_translation(1.0f);
			glm::vec3 j_skew(1.0f);
			glm::vec4 j_perspective(1.0f);
			glm::decompose(m_MVs[j], j_scale, j_rotation, j_translation, j_skew, j_perspective);

			if (Roof::round(mv_translation.z) == Roof::round(j_translation.z) && Roof::round(mv_translation.x) != Roof::round(j_translation.x) )
			{
				isIn = false;
				for (auto m : matrices)
				{
					if (m_MVs[j] == m)
						isIn = true;
				}
				if (!isIn)
				{
					matrices.push_back(m_MVs[j]);
				}
			}
		}
	}
	m_MVs = matrices;
	matrices.clear();

	// sort by X
	for (unsigned int i = 0; i < m_MVs.size(); ++i)
	{
		glm::vec3 i_scale(1.0f);
		glm::quat i_rotation;
		glm::vec3 i_translation(1.0f);
		glm::vec3 i_skew(1.0f);
		glm::vec4 i_perspective(1.0f);
		glm::decompose(m_MVs[i], i_scale, i_rotation, i_translation, i_skew, i_perspective);

		for (unsigned int j = 0; j < m_MVs.size(); ++j)
		{
			glm::vec3 j_scale(1.0f);
			glm::quat j_rotation;
			glm::vec3 j_translation(1.0f);
			glm::vec3 j_skew(1.0f);
			glm::vec4 j_perspective(1.0f);
			glm::decompose(m_MVs[j], j_scale, j_rotation, j_translation, j_skew, j_perspective);
			if ( Roof::round(i_translation.z) == Roof::round(j_translation.z) && Roof::round(i_translation.x) > Roof::round(j_translation.x) && i < j)
			{
					glm::mat4 tmp = m_MVs[i];
					m_MVs[i] = m_MVs[j];
					m_MVs[j] = tmp;
			}
		}
	}
}

void Roof::fill()
{
	std::vector<glm::mat4> matrices;
	for( unsigned int i = 0; i < m_MVs.size(); ++i)
	{
		if (i == 0 || m_MVs[i] != m_MVs[i-1])
			matrices.push_back(m_MVs[i]);
		glm::mat4 tmp = m_MVs[i];

		glm::vec3 it_scale(1.0f);
		glm::quat it_rotation;
		glm::vec3 it_translation(1.0f);
		glm::vec3 it_skew(1.0f);
		glm::vec4 it_perspective(1.0f);
		glm::decompose(m_MVs[i], it_scale, it_rotation, it_translation, it_skew, it_perspective);

		glm::vec3 next_scale(1.0f);
		glm::quat next_rotation;
		glm::vec3 next_translation(1.0f);
		glm::vec3 next_skew(1.0f);
		glm::vec4 next_perspective(1.0f);
		glm::decompose(m_MVs[i+1], next_scale, next_rotation, next_translation, next_skew, next_perspective);
		std::cout << "x: " << Roof::round(it_translation.x) <<  " z: " << Roof::round(it_translation.z) << "\n";

		// fill interpolating in X
		while(Roof::round(it_translation.z) == Roof::round(next_translation.z) && Roof::round(it_translation.x+1) < Roof::round(next_translation.x))
		{
			it_translation.x += 1.0f;
			tmp = glm::translate(glm::mat4(1.0f), it_translation);
			std::cout << "tmp x: " << Roof::round(it_translation.x) <<  " tmp z: " << Roof::round(it_translation.z) << "\n";
			matrices.push_back(tmp);
		}
	}

	m_MVs = matrices;
}
