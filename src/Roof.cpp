#include "Roof.h"

Roof::Roof()
{

}

Roof::Roof(Walls _base)
{
//	m_MVs.resize(_base.getMVs().size());
	// one way to approach the creation of  the roof could be
	// by taking all the mvs of the walls and organize them in a 2d array
	// sorting them by x values, so we will end up with a 2d array where all the sub array have the same x
	// this way I can fill the gap between the walls
	// this method will give me some problem since it's a naive approach but I can expand from there


	float min_x = 10000;
	float min_z = 10000;
	float max_x = -10000;
	float max_z = -10000;

	for (auto mv : _base.getMVs())
	{
		glm::vec3 mv_scale(1.0f);
		glm::quat mv_rotation;
		glm::vec3 mv_translation(1.0f);
		glm::vec3 mv_skew(1.0f);
		glm::vec4 mv_perspective(1.0f);
		glm::decompose(mv, mv_scale, mv_rotation, mv_translation, mv_skew, mv_perspective);

//		(*it++) = glm::translate(mv, glm::vec3(0,1,0));
		min_x = mv_translation.x < min_x ? mv_translation.x : min_x;
		min_z = mv_translation.z < min_z ? mv_translation.z : min_z;
		max_x = mv_translation.x > max_x ? mv_translation.x : max_x;
		max_z = mv_translation.z > max_z ? mv_translation.z : max_z;
	}
//	std::cout << "min: " << min_x << " - " << min_z << "\nmax: " << max_x << " - " << max_z << '\n';

	for(int z = 0; min_z + z <= max_z + 1; ++z)
	{
		for (int x = 0; min_x + x <= max_x + 1; ++x)
		{
//				if (z == 0)
//				{
				glm::mat4 currentBlock = glm::translate(glm::mat4(1.0f), glm::vec3(min_x + x, 1, min_z + z));
				m_MVs.push_back(currentBlock);
//				}
		}
	}

	float base_x;
	float base_z;

	std::vector<glm::mat4>::iterator it = m_MVs.begin();
	for (it = m_MVs.begin(); it != m_MVs.end(); ++it)
	{
		glm::vec3 mv_scale(1.0f);
		glm::quat mv_rotation;
		glm::vec3 mv_translation(1.0f);
		glm::vec3 mv_skew(1.0f);
		glm::vec4 mv_perspective(1.0f);
		glm::decompose((*it), mv_scale, mv_rotation, mv_translation, mv_skew, mv_perspective);
		int smallerThan = 0;
		for (auto base_mv : _base.getMVs())
		{
				glm::vec3 base_mv_scale(1.0f);
				glm::quat base_mv_rotation;
				glm::vec3 base_mv_translation(1.0f);
				glm::vec3 base_mv_skew(1.0f);
				glm::vec4 base_mv_perspective(1.0f);
				glm::decompose(base_mv, mv_scale, mv_rotation, base_mv_translation, mv_skew, mv_perspective);

				if (base_mv_translation.x > mv_translation.x && base_mv_translation.z == mv_translation.z)
				{
//					std::cout << "base x: " << base_mv_translation.x << " z: " << base_mv_translation.z;
//					std::cout << " roof x: " << mv_translation.x << " z: " << base_mv_translation.z << "\n";
						base_x = base_mv_translation.x;
						base_z = base_mv_translation.z;
					++smallerThan;
				}
		}
		if(smallerThan % 2 == 0 && smallerThan != 0)
		{
//			std::cout << "base x: " << base_mv_translation.x << " z: " << base_mv_translation.z;
			std::cout << " roof x: " << mv_translation.x << " z: " << mv_translation.z;// << "\n";
			std::cout << " base x: " << base_x << " z: " << base_z << "\n";
			m_MVs.erase(it);
		}
	}
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
