#include "Building.h"

Building::Building() /*: m_walls(m_rule), m_corners(m_walls)*/
{
	generateBase();
	Walls m_walls = Walls(m_rule);
	Corner m_corners = Corner(m_walls);
	Roof m_roof = Roof(m_walls);
	Mesh m_wall_mesh = Mesh("models/Walls/L.obj", "wall");
	Mesh m_corner_mesh = Mesh("models/Corners/b_oriented_cut_corner.obj", "corner");
	Mesh m_roof_mesh = Mesh("models/cube.obj", "cube");
	size_t walls_size = m_wall_mesh.getAmountVertexData() * m_walls.getMVs().size();
	size_t corners_size = m_corner_mesh.getAmountVertexData() * m_corners.getMVs().size();
	size_t roof_size = m_roof_mesh.getAmountVertexData() * m_roof.getMVs().size();
	m_vertices.resize( walls_size + corners_size + roof_size);
	m_normals.resize( walls_size + corners_size + roof_size);
	combinearrays(m_wall_mesh, dynamic_cast<Object*>(&m_walls));
	combinearrays(m_corner_mesh, dynamic_cast<Object*>(&m_corners));
	combinearrays(m_roof_mesh, dynamic_cast<Object*>(&m_roof));
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
//	m_rule = "rurdruuulllllddd";
}

void Building::combinearrays(Mesh & _mesh, Object * _object)
{
	static std::vector<float>::iterator vertices_it = m_vertices.begin();
	static std::vector<float>::iterator normals_it = m_normals.begin();

	for (auto mv : _object->getMVs())
	{
		for (size_t i = 0; i < _mesh.getAmountVertexData()/3; ++i)
		{
			glm::vec4 tmp_vertices(_mesh.getVertices()[i*3], _mesh.getVertices()[i*3+1], _mesh.getVertices()[i*3+2], 1);
			glm::vec4 tmp_normals(_mesh.getNormals()[i*3], _mesh.getNormals()[i*3+1], _mesh.getNormals()[i*3+2], 1);
			tmp_vertices = mv * tmp_vertices;
			tmp_normals = tmp_normals;

			(*vertices_it++) = (tmp_vertices.x);
			(*vertices_it++) = (tmp_vertices.y);
			(*vertices_it++) = (tmp_vertices.z);

			(*normals_it++) = (tmp_normals.x);
			(*normals_it++) = (tmp_normals.y);
			(*normals_it++) = (tmp_normals.z);
		}
	}
}

