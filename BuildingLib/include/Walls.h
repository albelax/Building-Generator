#ifndef WALLS_H
#define WALLS_H
#include "Object.h"

/// \file Walls.h
/// \brief this is the class that makes The the walls, effectively the most important class of the library,
/// it makes the outline of the building and allows the creation of all the other components
/// \author Alberto La Scala

class Walls : public Object
{
public:

	/// \brief Walls creates an empty wall object
	Walls();

	/// \brief Walls makes the walls based on the rule
	Walls(std::string _rule);

	/// \brief getRule returns a constant pointer to the rule that generated the walls
	const std::string * getRule() const { return &m_rule; }

	void clear() override;
private:
	/// \brief m_padding is the spacing to translate each wall etc
	float m_padding;

	/// \brief m_rule is rule that determines the shape of the base
	std::string m_rule;

	/// \brief generateMVs construct the shape of the building by generating the matrices
	void generateMVs();

	/// \brief makeTranslationTable makes the translation table
	void makeRotationTable() override;

	/// \brief makeTranslationTable makes the translation table
	void makeTranslationTable() override;
};

#endif // WALLS_H
