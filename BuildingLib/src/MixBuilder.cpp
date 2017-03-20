#include "MixBuilder.h"

MixBuilder::MixBuilder(int _height) : Builder(_height)
{
	m_building.setMode(Building::MIX);
	m_building.addDecorations();
	m_building.addWindows();
	m_building.addRoof();
	m_building.addFront();
	m_building.clear();
}

//----------------------------------------------------------------------------------------------------------------------

MixBuilder::~MixBuilder()
{

}

//----------------------------------------------------------------------------------------------------------------------

