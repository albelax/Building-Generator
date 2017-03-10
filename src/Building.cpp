#include "Building.h"

Building::Building() /*: m_walls(m_rule), m_corners(m_walls)*/
{
	generateBase();
	m_height = 5;
	Walls m_walls = Walls(m_rule);
	Corner m_corners = Corner(m_walls);
	Roof m_roof = Roof(m_walls, m_walls);
	Mesh m_wall_mesh = Mesh("models/my_Building/Walls/Plane.obj", "wall");
	Mesh m_corner_mesh = Mesh("models/my_Building/Corners/b_oriented_cut_corner.obj", "corner");
	Mesh m_roof_mesh = Mesh("models/my_Building/Roofs/cube.obj", "cube");
	Mesh m_decoration = Mesh("models/new_Building/Decorations/deco.obj", "deco");
	Mesh m_window = Mesh("models/my_Building/Windows/window.obj", "win");

	unsigned int walls_size = m_wall_mesh.getAmountVertexData() * m_walls.getMVs().size();
	unsigned int corners_size = m_corner_mesh.getAmountVertexData() * m_corners.getMVs().size();
	unsigned int roof_size = m_roof_mesh.getAmountVertexData() * m_roof.getMVs().size();
	unsigned int deco_size = m_decoration.getAmountVertexData() * m_walls.getMVs().size();
	unsigned int window_size = m_window.getAmountVertexData() * m_walls.getMVs().size();

	m_vertices.resize( (walls_size + corners_size + deco_size + window_size)*m_height + roof_size);
	m_normals.resize( (walls_size + corners_size + deco_size + window_size)*m_height + roof_size);
	combinearrays(m_wall_mesh, dynamic_cast<Object*>(&m_walls), Floor::ALL);
	combinearrays(m_corner_mesh, dynamic_cast<Object*>(&m_corners), Floor::ALL);
	combinearrays(m_decoration, dynamic_cast<Object*>(&m_walls), Floor::NOT_BOTTOM);
	combinearrays(m_window, dynamic_cast<Object*>(&m_walls), Floor::NOT_BOTTOM);
	combinearrays(m_roof_mesh, dynamic_cast<Object*>(&m_roof), Floor::TOP);
}

void Building::generateBase()
{
	// for now I just have a few rules,
	// maybe at some point I will actually generate them randomly
	m_rule = "ruld";
	m_rule = "rrrruuulldld";
	m_rule = "rrrurruuuuldlluldddddd";
	m_rule = "rurdruuulllllddd";
}

void Building::combinearrays(Mesh & _mesh, Object * _object, Floor _floor)
{
	unsigned int floor_begin;
	unsigned int floor_end;
	switch (_floor)
	{
		case(Floor::ALL) : floor_begin = 0; floor_end = m_height;
			break;
		case(Floor::BOTTOM) : floor_begin = 0; floor_end = 1;
			break;
		case(Floor::TOP) : floor_begin = m_height; floor_end = m_height+1;
			break;
		case(Floor::NOT_BOTTOM) : floor_begin = 1; floor_end = m_height;
			break;
	}

	static std::vector<float>::iterator vertices_it = m_vertices.begin();
	static std::vector<float>::iterator normals_it = m_normals.begin();
	for (unsigned int floor = floor_begin; floor < floor_end; ++floor)
	{
		for (auto mv : _object->getMVs())
		{
			glm::mat4 tmp_mv = glm::translate(mv, glm::vec3(0,(float)floor/1,0));
			for (unsigned int i = 0; i < _mesh.getAmountVertexData()/3; ++i)
			{
				glm::vec4 tmp_vertices(_mesh.getVertices()[i*3], _mesh.getVertices()[i*3+1], _mesh.getVertices()[i*3+2], 1);
				glm::vec4 tmp_normals(_mesh.getNormals()[i*3], _mesh.getNormals()[i*3+1], _mesh.getNormals()[i*3+2], 1);
				tmp_vertices = tmp_mv * tmp_vertices;
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
}

