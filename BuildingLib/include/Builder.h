#ifndef BUILDER_H
#define BUILDER_H

#include "Building.h"

class Builder
{
protected:
	Building m_building;
public:
	Builder();
	Builder(int _height);
	~Builder();
	Building getBuilding();
};

#endif
