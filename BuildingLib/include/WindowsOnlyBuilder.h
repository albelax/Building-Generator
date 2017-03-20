#ifndef WINDOWSONLYBUILDER_H
#define WINDOWSONLYBUILDER_H

#include "Builder.h"
#include "Building.h"

class WindowsOnlyBuilder : public Builder
{
public:
	/// \brief MixBuilder creates a complete building, of a specified height, using the MIX mode
 WindowsOnlyBuilder(int _height = 4);
 ~WindowsOnlyBuilder();
};

#endif // WINDOWSONLYBUILDER_H
