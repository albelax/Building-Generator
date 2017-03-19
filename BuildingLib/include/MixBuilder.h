#ifndef MIXBUILDER_H
#define MIXBUILDER_H

#include "Builder.h"
#include "Building.h"

/// \file MixBuilder.h
/// \brief inherits from the Builder Class, it allowes me to encapsulate
/// a set of function, it makes it easier to generate buildings,
/// this class creates a complete building, featuring decorations, windows, roof, and main entrance,
/// it creates it by randomly choosing the assets from randomly chosen directories
/// \author Alberto La Scala

class MixBuilder : public Builder
{
public:
	/// \brief MixBuilder creates a complete building, of a specified height, using the MIX mode
 MixBuilder(int _height = 4);
	~MixBuilder();
};

#endif // MIXBUILDER_H
