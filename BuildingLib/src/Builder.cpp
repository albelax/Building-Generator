#include "Builder.h"

Builder::Builder()
{
	m_building = Building();
	m_building.setHeight(4);
	m_building.makeBase();
}

Builder::Builder(int _height)
{
	m_building = Building();
	m_building.setHeight(_height);
	m_building.makeBase();
}

Builder::~Builder()
{

}

Building Builder::getBuilding()
{
	return m_building;
}
