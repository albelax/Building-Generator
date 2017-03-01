#include "Building.h"

Building::Building() : m_walls(m_rule), m_corners(m_walls)
{
	generateBase();
}

void Building::generateBase()
{
	// for now I just have a few rules,
	// maybe at some point I will actually generate them randomly
	m_rule = "ruld";
	m_rule = "rrrruuulldld";
	m_rule = "rurdruullldd";
	m_rule = "rurdruuuldlulddsd";
	m_rule = "rrrurruuuuldlluldddddd";
	m_rule = "rurdruuulllllddd";
}
