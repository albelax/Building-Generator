#include "Roof.h"

Roof::Roof()
{

}

Roof::Roof(Walls _base)
{
	m_MVs.resize(_base.getMVs().size());
	// one way to approach the creation of  the roof could be
	// by taking all the mvs of the walls and organize them in a 2d array
	// sorting them by x values, so we will end up with a 2d array where all the sub array have the same x
	// this way I can fill the gap between the walls
	// this method will give me some problem since it's a naive approach but I can expand from there

	std::vector<std::vector<float>> matrices;
	std::vector<glm::mat4>::iterator it = m_MVs.begin();
	for (auto mv: _base.getMVs())
	{
		glm::vec3 scale(1.0f);
		glm::quat rotation;
		glm::vec3 translation(1.0f);
		glm::vec3 skew(1.0f);
		glm::vec4 perspective(1.0f);
		glm::mat4 p = mv;
		glm::decompose(p,scale,rotation, translation, skew, perspective);
		std::cout << glm::to_string(translation) << '\n';
		(*it++) = glm::translate(mv, glm::vec3(0,1,0));
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
