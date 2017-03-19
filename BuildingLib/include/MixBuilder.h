#ifndef MIXBUILDER_H
#define MIXBUILDER_H

#include "Builder.h"
#include "Building.h"

class MixBuilder : public Builder
{
public:
 MixBuilder(int _height = 4);
	~MixBuilder();
};

#endif // MIXBUILDER_H
