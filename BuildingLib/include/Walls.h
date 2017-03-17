#ifndef WALLS_H
#define WALLS_H
#include "Object.h"


class Walls : public Object
{
public:
	Walls();
	Walls(std::string _rule);
	const std::string * getRule() const { return &m_rule; }
private:
	/// spacing to translate each wall etc
	float m_padding;
	/// rule that determines the shape of the base
	std::string m_rule;
	void generateMVs();
	void makeRotationTable() override;
	void makeTranslationTable() override;
};

#endif // WALLS_H
