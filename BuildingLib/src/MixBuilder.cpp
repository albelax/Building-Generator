#include "MixBuilder.h"

MixBuilder::MixBuilder() : Builder()
{
	m_building.setMode(Building::MIX);
	m_building.makeDecorations();
	m_building.makeWindows();
	m_building.makeRoof();
}

MixBuilder::~MixBuilder()
{

}
