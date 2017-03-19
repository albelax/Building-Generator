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

TEST(GenerateRule, isEven)
{
	/// THIS TEST IS FRIEND WITH THE BUILDING CLASS
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

TEST(Meshes, saveSuccess)
{
	/// test if the mesh class saves correctly
	Mesh testMesh = Mesh("models/cube.obj", "cube");
	std::string address = "test.obj";
	std::ifstream out;

	testMesh.write( address );
	out.open( address );
	bool openSucceeded = out.is_open();
	out.close();
	EXPECT_TRUE( openSucceeded );
}

//----------------------------------------------------------------------------------------------------------------------

TEST(Meshes, failedOpen)
{
	/// test if the mesh class saves correctly
	std::string address = "sADFGDTHYTGDFS.obj";
	Mesh testMesh = Mesh(address, "fail");

	std::ifstream file;
	file.open( address );
	bool openSucceeded = file.is_open();
	file.close();
	EXPECT_FALSE( openSucceeded );
}

//----------------------------------------------------------------------------------------------------------------------

TEST(height, negativeHeight)
{
	Building building = Building();
	building.setHeight(-1);
	EXPECT_TRUE( building.m_height > 0 );
}

//----------------------------------------------------------------------------------------------------------------------

TEST(height, correctHeight)
{
	Building building = Building();
	building.setHeight(4);
	EXPECT_TRUE( building.m_height > 0 );
}

//----------------------------------------------------------------------------------------------------------------------

TEST(combineArrays, correctArrays)
{
	Building building = Building();
	building.setHeight(3);
	building.makeBase();
	Roof roof = Roof(building.m_walls, building.m_corners);
	Mesh roof_mesh = Mesh("models/cube.obj", "cube");
	unsigned int tot = building.m_vertices.size() + roof_mesh.getAmountVertexData() + roof.getMVs().size();
	building.m_vertices.resize(tot);
	building.m_normals.resize(tot);
	bool success = building.combinearrays(roof_mesh, dynamic_cast<Object*>(&roof), Building::Floor::TOP);
	EXPECT_TRUE( success );
}

//----------------------------------------------------------------------------------------------------------------------

TEST(combineArrays, emptyVertices)
{
	Building building = Building();
	Roof roof = Roof(building.m_walls, building.m_corners);
	Mesh roof_mesh = Mesh();
	unsigned int tot = building.m_vertices.size() + roof_mesh.getAmountVertexData() + roof.getMVs().size();
	building.m_vertices.resize(tot);
	building.m_normals.resize(tot);
	bool success = building.combinearrays(roof_mesh, dynamic_cast<Object*>(&roof), Building::Floor::TOP);
	EXPECT_FALSE( success );
}

//----------------------------------------------------------------------------------------------------------------------

TEST(combineArrays, emptyMVs)
{
	Building building = Building();
	Roof roof = Roof();
	Mesh roof_mesh = Mesh("models/cube.obj", "cube");
	unsigned int tot = building.m_vertices.size() + roof_mesh.getAmountVertexData() + roof.getMVs().size();
	building.m_vertices.resize(tot);
	building.m_normals.resize(tot);
	bool success = building.combinearrays(roof_mesh, dynamic_cast<Object*>(&roof), Building::Floor::TOP);
	EXPECT_FALSE( success );
}

//----------------------------------------------------------------------------------------------------------------------

TEST(Round, first)
{
	float test = 1.2f;
	float expected = 1.0f;
	EXPECT_TRUE( Roof::round(test) == expected);
}
