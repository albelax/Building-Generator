#ifndef BUILDING_H
#define BUILDING_H

#include <vector>
#include <string>
#include <dirent.h>
#include <gtest/gtest.h>

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

#define TESTING 1

class Building : public Object
{
public:
	Building();
	enum GenerationMode { RANDOM_MIX, MIX, RANDOM_STYLE, SELECT_STYLE };
	enum element { WALL, CORNER, DECORATION, WINDOW, ROOF, FRONT };
	enum fileType { DIRECTORY, OBJ_FILE };
	const std::string * getRule() const { return &m_rule; }
	float const * getVertices() const { return &m_vertices[0]; }
	float const * getNormals() const { return &m_normals[0]; }
	std::vector<float> const & getVerticesContainer() { return m_vertices; }
	std::vector<float> const & getNormalsContainer() { return m_normals; }

	///
	/// \brief amountVertices
	/// \return
	///
	int amountVertices() const { return m_vertices.size(); }
	int getBufferIndex() const { return m_bufferIndex; }
	void setBufferIndex(int _index) { m_bufferIndex = _index; }
	void create();
	void makeBase();
	void addDecorations();
	void addWindows();
	void addRoof();
	void addFront();
	void setHeight(int _height);
	void setMode(GenerationMode _mode) { m_mode = _mode; }

private:
	enum Floor {ALL, BOTTOM, TOP, NOT_BOTTOM, ENTRANCE };
	enum GenerationMode m_mode;
	int m_height;
	/// rule that determines the shape of the base
	std::string m_rule;
	/// generating the rule that will make the walls
	void generateRule();
	Walls m_walls;
	Corner m_corners;
	bool combinearrays(Mesh const & _mesh, Object *_object, Floor const _floor);
	std::vector<float> m_vertices;
	std::vector<float> m_normals;
	std::string selectFolder(GenerationMode const _MODE, element const _ELEMENT) const;
	std::vector<std::string> ls(const std::__1::string _directory, const fileType _type) const;
	void splitCorners(std::vector<glm::mat4> * o_standard, std::vector<glm::mat4> * o_exception);

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
