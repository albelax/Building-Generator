#ifndef BUILDER_H
#define BUILDER_H

#include "Building.h"

/// \file Builder.h
/// \brief the Builder class encapsulates the construction of a basic building,
/// the base class (this) only creates the shape of the building
/// \author Alberto La Scala

class Builder
{
protected:
	/// \brief building to be created
	Building m_building;
public:
	///\brief the constructor encapsulates the function calls needed to generate the basic outline of the building
	Builder(int _height = 4);
	~Builder();
	/// \brief returns the building constructed
	Building & getBuilding();
};

#endif // BUILDER_H
