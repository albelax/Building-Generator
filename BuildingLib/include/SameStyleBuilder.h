#ifndef SAMESTYLEBUILDER_H
#define SAMESTYLEBUILDER_H

#include "Builder.h"
#include "Building.h"

class SameStyleBuilder : public Builder
{
public:
	/// \brief MixBuilder creates a complete building, of a specified height, using the MIX mode
 SameStyleBuilder(int _height = 4);
 ~SameStyleBuilder();
};

#endif // SAMESTYLEBUILDER_H
