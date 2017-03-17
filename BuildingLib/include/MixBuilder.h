#ifndef MIXBUILDER_H
#define MIXBUILDER_H

#include "Builder.h"
#include "Building.h"

class MixBuilder : public Builder
{
public:
	MixBuilder();
	MixBuilder(int _height);
	~MixBuilder();
};

#endif
