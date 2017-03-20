#include "WindowsOnlyBuilder.h"

WindowsOnlyBuilder::WindowsOnlyBuilder(int _height) : Builder(_height)
{
	m_building.setMode(Building::MIX);
	m_building.addWindows();
	m_building.addRoof();
	m_building.addFront();
	m_building.clear();
}

//----------------------------------------------------------------------------------------------------------------------

WindowsOnlyBuilder::~WindowsOnlyBuilder()
{

}

//----------------------------------------------------------------------------------------------------------------------

