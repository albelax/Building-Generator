#ifndef BUILDING_H
#define BUILDING_H

#include <vector>
#include <string>
#include <dirent.h>


#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "ext.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtc/type_ptr.hpp"
#include "Mesh.h"
#include "Object.h"
#include "Walls.h"
#include "Corner.h"
#include "Roof.h"

#define TESTING 0

#if TESTING
#include <gtest/gtest.h>
#endif // TESTING

/// \file Building.h
/// \brief this is the class that makes a building, it contains all the components to make a building and all the functions needed
/// to make various buildings, the creation is modular, the user can decide whether to add decorations, windows or even a roof.
///	This class controls also the way the building is generated, either way mixing between various foulders of assets or
/// mixing within the same folder.
/// \author Alberto La Scala

class Building : public Object
{
public:
	Building();

	///\brief GenerationMode dictates how to generate the building, whether is from the same folder,
	/// one folder picked randomly, or completely randomly mixing multiple folders containing different styles
	enum GenerationMode { RANDOM_MIX, MIX, RANDOM_STYLE, SELECT_STYLE };

	///\brief element is used to load a specific element of the building
	enum element { WALL, CORNER, DECORATION, WINDOW, ROOF, FRONT };

	///\brief fileType is either way an obj file or a directory
	enum fileType { DIRECTORY, OBJ_FILE };

	///\brief getRule returns the rule that was used to construct the building
	const std::string * getRule() const { return &m_rule; }

	///\brief getVertices return  pointer to the first element of the array of vertices
	float const * getVertices() const { return &m_vertices[0]; }

	///\brief getNormals return  pointer to the first element of the array of normals
	float const * getNormals() const { return &m_normals[0]; }

	///\brief getVerticesContainer return pointer to the std::vector containing the vertices
	std::vector<float> const & getVerticesContainer() { return m_vertices; }

	///\brief getNormalsContainer return pointer to the std::vector containing the normals
	std::vector<float> const & getNormalsContainer() { return m_normals; }

	/// \brief amountVertices returns the size of the array of its vertices, which are stored as floats
	int amountVertices() const { return m_vertices.size(); }

	/// \brief getBufferIndex returns the index of the buffer that store its vertices, it makes it easy for drawing and instantiating
	int getBufferIndex() const { return m_bufferIndex; }

	///\brief assign the index of the buffer where its vertices are stored
	void setBufferIndex(int _index) { m_bufferIndex = _index; }

	/// \brief makeBase constructs the outline of the building, walls and corners
	void makeBase();

	/// \brief addDecorations adds the decorations to the walls, using the transformations of the walls
	void addDecorations();

	/// \brief addWindows adds windows to the walls, using the transformations of the walls
	void addWindows();

	/// \brief addRoof generates the roof of the building using the transformations of walls and corners,
	void addRoof();

	/// \brief addFront places the front door, it will add it based on the rule that generates the walls, using the first letter
	void addFront();

	/// \brief setHeight decides how many stories tall the building should be
	void setHeight(int _height);

	/// \brief setMode is used by the user to decide whether the building should be generate using multiple folders or only one
	void setMode(GenerationMode _mode) { m_mode = _mode; }

	void clear() override;
private:
	/// \brief Floor dictates where each elements is attached to the building
	enum Floor {ALL, BOTTOM, TOP, NOT_BOTTOM, ENTRANCE };

	enum GenerationMode m_mode;

	int m_height;

	/// rule that determines the shape of the base
	std::string m_rule;

	/// generating the rule that will make the walls
	void generateRule();

	///\brief m_walls are the walls of the building
	Walls m_walls;

	///\brief m_corners are the corners of the building
	Corner m_corners;

	///\brief combinearrays takes as input an element of the building, its 3d model, and where it needs to be applied,
	/// applying the transformation matrices of the element combines the vertices and normals of the mesh with the ones of the building
	/// allowing the building to be used as a single mesh optimising the draw calls
	bool combinearrays(Mesh const & _mesh, Object *_object, Floor const _floor);

	/// \brief m_vertices contains the vertices of the building, stored as floats
	std::vector<float> m_vertices;

	/// \brief m_normals contains the normals of the building, stored as floats
	std::vector<float> m_normals;

	/// \brief selectFolder selects the assets from a specific style
	/// \param _MODE if the mode is MIX the style will keep changing
	/// \param _ELEMENT specifies the element that needs to be selected
	std::string selectFromFolder(GenerationMode const _MODE, element const _ELEMENT) const;

	/// \brief ls is just like the unix command ls, gets the list of files and directories inside a specific directory
	/// \param _directory is where the function will operate
	/// \param _type if DIRECTORY will look for the directories, otherwise it will look for obj files
	/// \return return the list of directories or obj files inside a directory
	std::vector<std::string> ls(const std::__1::string _directory, const fileType _type) const;

	/// \brief splits the concave corners from the convex ones using the exceptions and the MVs stored,
	/// this is useful to select specific assests,
	/// the function doesn't return anything, however it will modify the Inputs
	void splitCorners(std::vector<glm::mat4> * io_standard, std::vector<glm::mat4> * io_exception);

#if TESTING
	FRIEND_TEST(GenerateRule, isEven);
	FRIEND_TEST(GenerateRule, hasRightCharacters);
	FRIEND_TEST(height, correctHeight);
	FRIEND_TEST(height, negativeHeight);
	FRIEND_TEST(combineArrays, correctArrays);
	FRIEND_TEST(combineArrays, emptyVertices);
	FRIEND_TEST(combineArrays, emptyMVs);
#endif // TESTING

};


#endif // BUILDING_H
