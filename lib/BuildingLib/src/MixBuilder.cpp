#include "MixBuilder.h"

MixBuilder::MixBuilder() : Builder()
{
	addElements();
}

//----------------------------------------------------------------------------------------------------------------------

MixBuilder::MixBuilder(int _height) : Builder(_height)
{
	addElements();
}

//----------------------------------------------------------------------------------------------------------------------

MixBuilder::~MixBuilder()
{

}

//----------------------------------------------------------------------------------------------------------------------

void MixBuilder::addElements()
{
	/// adds all the components needed to generate this type of building
	m_building.setMode(Building::MIX);
	m_building.addDecorations();
	m_building.addWindows();
	m_building.addRoof();
	m_building.addFront();
}

//----------------------------------------------------------------------------------------------------------------------

