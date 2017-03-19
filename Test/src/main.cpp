#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <gtest/gtest.h>
#include <fstream>
#include <random>
#include <iostream>
#include <random>

#include "Building.h"
#include "Object.h"
#include "Walls.h"
#include "Mesh.h"
#include "Corner.h"
#include "Builder.h"
#include "MixBuilder.h"


int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

//----------------------------------------------------------------------------------------------------------------------

TEST(BuildingConstructor, DefaultConstructor)
{
	EXPECT_TRUE( true );
}

//----------------------------------------------------------------------------------------------------------------------

TEST(GenerateRule, isEven)
{
	Building building = Building();
	EXPECT_TRUE( building.m_rule.length()%2 == 0 );
}

//----------------------------------------------------------------------------------------------------------------------

TEST(GenerateRule, hasRightCharacters)
{
	/// tests if all the characters are in thes string and if there are illegal characters
	/// THIS TEST IS FRIEND WITH THE BUILDING CLASS
	Building building = Building();
	bool r, u, l, d = false;
	bool illegalChar = false;
	for (auto i : building.m_rule)
	{
		switch(i)
		{
			case('r'): { r = true; break; }
			case('u'): { u = true; break; }
			case('l'): { l = true; break; }
			case('d'): { d = true; break; }
			default: { illegalChar = true; break; }
		}
	}
	EXPECT_TRUE( r && u && l && d && !illegalChar);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(SaveBuilding, saveSuccess)
{
	/// test if the mesh class saves correctly
	Mesh testMesh = Mesh("../models/cube.obj", "cube");
	std::string address = "test.obj";
	std::ifstream out;

	testMesh.write( address );
	out.open( address );
	bool openSucceeded = out.is_open();
	out.close();
	EXPECT_TRUE( openSucceeded );
}

//----------------------------------------------------------------------------------------------------------------------

TEST(Builder, pippo)
{
	MixBuilder pippo = MixBuilder();
	EXPECT_TRUE( true );
}

