#include "MixBuilder.h"

MixBuilder::MixBuilder() : Builder()
{
	m_building.setMode(Building::MIX);
	m_building.makeDecorations();
}
