#include "SameStyleBuilder.h"

SameStyleBuilder::SameStyleBuilder(int _height) : Builder(_height)
{
	m_building.setMode(Building::RANDOM_STYLE);
	m_building.addDecorations();
	m_building.addWindows();
	m_building.addRoof();
	m_building.addFront();
	m_building.clear();
}

//----------------------------------------------------------------------------------------------------------------------

SameStyleBuilder::~SameStyleBuilder()
{

}

//----------------------------------------------------------------------------------------------------------------------

