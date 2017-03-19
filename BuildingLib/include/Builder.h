#ifndef BUILDER_H
#define BUILDER_H

#include "Building.h"

class Builder
{
protected:
	Building m_building;
public:
	Builder(int _height = 4);
	~Builder();
	Building getBuilding();
};

#endif // BUILDER_H
