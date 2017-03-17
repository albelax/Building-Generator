#include "Building.h"
#include <dirent.h>
#include <random>

Building::Building() /*: m_walls(m_rule), m_corners(m_walls)*/
{
//	create();
	generateRule();
////	m_height = 5;
////	m_mode = MIX;
////	makeBase();
////	makeDecorations();
}

void Building::generateRule()
{
	// for now I just have a few rules,
	// maybe at some point I will actually generate them randomly
	m_rule = "ruld";
	m_rule = "rrrruuulldld";
//	m_rule = "rrrurruuuuldlluldddddd";
//	m_rule = "rurdruuulllllddd";
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

	// the iterators keep track of the positions of the last elements added,
	// I decided to use iterators because of the ease of use,
	// unfortunately iterators break in case the vector is resized,
	// this is why I added other ints to recover the indices in case the vector is resized
	static std::vector<float>::iterator vertices_it = m_vertices.begin();
	static std::vector<float>::iterator normals_it = m_normals.begin();
	static unsigned int previousSize = m_vertices.size();
	static int lastIndex;

	if (m_vertices.size() > previousSize)
	{
		vertices_it = m_vertices.begin() + lastIndex;
		normals_it = m_normals.begin() + lastIndex;
	}
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
	lastIndex = vertices_it - m_vertices.begin();
}


std::string Building::selectFolder(GenerationMode _MODE, element _ELEMENT)
{
	/// select the assets from a specific style, if the mode is MIX the style will keep changing
	/// otherwise it won't
	std::vector<std::string> directories;
	std::vector<std::string> files;
	srandom (time(NULL));
	directories = Building::ls("models/", fileType::DIRECTORY);

	static std::string buildingFolder = directories[(random() % directories.size())];
	switch (_MODE)
	{
		case GenerationMode::MIX: buildingFolder = directories[(random() % directories.size())]; break;
		default:break;
	}

	std::string address = "models/" + buildingFolder;
	switch (_ELEMENT)
	{
		case element::WALL: address += "/Walls/"; break;
		case element::CORNER: address += "/Corners/"; break;
		case element::DECORATION : address += "/Decorations/"; break;
		case element::WINDOW : address += "/Windows/"; break;
		case element::ROOF : address += "/Roofs/"; break;
	}
	files = Building::ls(address, fileType::OBJ_FILE);
	address = address + files[(random() % files.size())];
	return address;
}


std::vector<std::string> Building::ls(std::string _directory, fileType _TYPE)
{
	/// returns a std::vector containing all the obj files and all the folders
	/// in the current directry
	std::vector<std::string> files;
	DIR * dir;
	dirent * element;

	if ((dir = opendir(_directory.c_str())))
	{
		while ((element = readdir(dir)))
		{
			// goes through each element of the current directory
			// looking for obj files and folders
			std::string name = (char*) element->d_name;
			bool isFile = false;
			if (name.length() > 4 && name.substr((name.length()-4),name.length()) == ".obj")
				isFile = true;
			if (isFile && _TYPE == fileType::OBJ_FILE)
				files.push_back(element->d_name);

			else if (_TYPE == fileType::DIRECTORY)
			{
				// ignore other files and folders containign the .
				bool isDirectory = true;
				for(auto i: name)
				{
					if(i == '.')
						isDirectory = false;
				}
				if (isDirectory)
					files.push_back(element->d_name);
			}
		}
		closedir (dir);
	}
	return files;
}

void Building::create()
{
	m_mode = MIX;
	generateRule();
	m_height = 5;
	Walls m_walls = Walls(m_rule);
	Corner m_corners = Corner(m_walls);
	Roof m_roof = Roof(m_walls, m_corners);
	Mesh m_wall_mesh = Mesh("models/my_Building/Walls/Plane.obj", "wall");
	Mesh m_corner_mesh = Mesh("models/my_Building/Corners/b_oriented_cut_corner.obj", "corner");
	Mesh m_roof_mesh = Mesh("models/my_Building/Roofs/cube.obj", "cube");
	Mesh m_roofEdges_mesh = Mesh("models/my_Building/Roofs/Roof_edge.obj", "roofEdge");
	Mesh m_decoration = Mesh(selectFolder(m_mode, element::DECORATION), "deco");
	Mesh m_window = Mesh(selectFolder(m_mode, element::WINDOW), "win");
	unsigned int walls_size = m_wall_mesh.getAmountVertexData() * m_walls.getMVs().size();
	unsigned int corners_size = m_corner_mesh.getAmountVertexData() * m_corners.getMVs().size();
	unsigned int roof_size = m_roof_mesh.getAmountVertexData() * m_roof.getMVs().size();
	unsigned int roofEdges_size = m_roofEdges_mesh.getAmountVertexData() * m_walls.getMVs().size();
	unsigned int deco_size = m_decoration.getAmountVertexData() * m_walls.getMVs().size();
	unsigned int window_size = m_window.getAmountVertexData() * m_walls.getMVs().size();

	m_vertices.resize( (walls_size + corners_size + deco_size + window_size)*m_height + roof_size + roofEdges_size);
	m_normals.resize( (walls_size + corners_size + deco_size + window_size)*m_height + roof_size + roofEdges_size);
	combinearrays(m_wall_mesh, dynamic_cast<Object*>(&m_walls), Floor::ALL);
	combinearrays(m_corner_mesh, dynamic_cast<Object*>(&m_corners), Floor::ALL);
	combinearrays(m_decoration, dynamic_cast<Object*>(&m_walls), Floor::NOT_BOTTOM);
	combinearrays(m_window, dynamic_cast<Object*>(&m_walls), Floor::NOT_BOTTOM);
	combinearrays(m_roof_mesh, dynamic_cast<Object*>(&m_roof), Floor::TOP); // fills the roof
//	combinearrays(m_roof_mesh, dynamic_cast<Object*>(&m_corners), Floor::TOP); // fills the roof
	combinearrays(m_roofEdges_mesh, dynamic_cast<Object*>(&m_walls), Floor::TOP); // place the roof only at the edges
}

void Building::makeBase()
{
	m_walls = Walls(m_rule);
	m_corners = Corner(m_walls);
	Mesh wall_mesh = Mesh("models/my_Building/Walls/Plane.obj", "wall");
	Mesh corner_mesh = Mesh(selectFolder(m_mode, element::CORNER), "corner");
	unsigned int walls_size = wall_mesh.getAmountVertexData() * m_walls.getMVs().size();
	unsigned int corners_size = corner_mesh.getAmountVertexData() * m_corners.getMVs().size();
	m_vertices.resize(m_vertices.size() + (walls_size*m_height) + (corners_size*m_height));
	m_normals.resize(m_normals.size() + (walls_size*m_height) + (corners_size*m_height));
	combinearrays(wall_mesh, dynamic_cast<Object*>(&m_walls), Floor::ALL);
	combinearrays(corner_mesh, dynamic_cast<Object*>(&m_corners), Floor::ALL);
}

void Building::makeDecorations()
{
	Mesh decoration = Mesh(selectFolder(m_mode, element::DECORATION), "deco");
	unsigned int deco_size = decoration.getAmountVertexData() * m_walls.getMVs().size();
	m_vertices.resize(m_vertices.size() + (deco_size*(m_height-1)));
	m_normals.resize(m_normals.size() + (deco_size*(m_height-1)));
	combinearrays(decoration, dynamic_cast<Object*>(&m_walls), Floor::NOT_BOTTOM);
}

void Building::makeWindows()
{
	Mesh window = Mesh(selectFolder(m_mode, element::WINDOW), "win");
	unsigned int window_size = window.getAmountVertexData() * m_walls.getMVs().size();
	m_vertices.resize(m_vertices.size() + (window_size*(m_height-1))); // - 1 because it doesn't include the ground floor
	m_normals.resize(m_normals.size() + (window_size*(m_height-1)));
	combinearrays(window, dynamic_cast<Object*>(&m_walls), Floor::NOT_BOTTOM);
}


void Building::makeRoof()
{
	Roof roof = Roof(m_walls, m_corners);
	Mesh roof_mesh = Mesh("models/cube.obj", "cube");
	Mesh roofEdges_mesh = Mesh(selectFolder(m_mode, element::ROOF), "roofEdge");
	unsigned int roof_size = roof_mesh.getAmountVertexData() * roof.getMVs().size();
	unsigned int roofEdges_size = roofEdges_mesh.getAmountVertexData() * m_walls.getMVs().size();
	m_vertices.resize(m_vertices.size() + roof_size + roofEdges_size);
	m_normals.resize(m_normals.size() + roof_size + roofEdges_size);
	combinearrays(roof_mesh, dynamic_cast<Object*>(&roof), Floor::TOP); // fills the roof
	combinearrays(roofEdges_mesh, dynamic_cast<Object*>(&m_walls), Floor::TOP); // place the roof only at the edges
}
