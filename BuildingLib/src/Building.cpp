#include "Building.h"
#include <dirent.h>
#include <fstream>
#include <random>

Building::Building() /*: m_walls(m_rule), m_corners(m_walls)*/
{
	generateRule();
}

//----------------------------------------------------------------------------------------------------------------------

void Building::generateRule()
{
	// for now I just have a few rules,
	// maybe at some point I will actually generate them randomly
	m_rule = "ruld";
	m_rule = "rrrruuulldld";
//	m_rule = "rrrurruuuuldlluldddddd";
	m_rule = "rurdruuulllllddd";
}

//----------------------------------------------------------------------------------------------------------------------

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
			glm::vec3 i_scale(1.0f);
			glm::quat i_rotation;
			glm::vec3 i_translation(1.0f);
			glm::vec3 i_skew(1.0f);
			glm::vec4 i_perspective(1.0f);
			glm::decompose(mv, i_scale, i_rotation, i_translation, i_skew, i_perspective);

			glm::mat4 tmp_mv = glm::translate(mv, glm::vec3(0,(float)floor/1,0));
			for (unsigned int i = 0; i < _mesh.getAmountVertexData()/3; ++i)
			{
				glm::vec4 tmp_vertices(_mesh.getVertices()[i*3], _mesh.getVertices()[i*3+1], _mesh.getVertices()[i*3+2], 1);
				glm::vec4 tmp_normals(_mesh.getNormals()[i*3], _mesh.getNormals()[i*3+1], _mesh.getNormals()[i*3+2], 1);
				tmp_vertices = tmp_mv * tmp_vertices;
				tmp_normals = tmp_normals*i_rotation;

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

//----------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------

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
//				std::cout << element->d_name << "\n";
			}
		}
		closedir (dir);
	}
	return files;
}

//----------------------------------------------------------------------------------------------------------------------

void Building::makeBase()
{
	m_walls = Walls(m_rule);
	m_corners = Corner(m_walls);
	Mesh wall_mesh = Mesh("models/my_Building/Walls/Plane.obj", "wall");
	Mesh corner_mesh = Mesh(selectFolder(m_mode, element::CORNER), "corner");

	// load the exception of the same corner
	std::string corner_exception_address;
	for (unsigned int i = 0; i < 3; ++i)
		corner_exception_address += corner_mesh.getAddress()[i] + "/";
	std::ifstream file;
	file.open(corner_exception_address);
	if (file.is_open())
	{
		file.close();
		corner_exception_address += "Exceptions/" + corner_mesh.getAddress()[3];
	}
	else
		corner_exception_address = "models/my_Building/Corners/cutCorner.obj";

	// split the matrices of concave and covex corners in two different vectors
	std::vector<glm::mat4> exceptionMatrices;
	std::vector<glm::mat4> cornerMatrices;
	splitCorners(&cornerMatrices, &exceptionMatrices);
	Corner exception = Corner(exceptionMatrices);
	Corner corners = Corner(cornerMatrices);

	Mesh corner_exception = Mesh(corner_exception_address, "exception");
	unsigned int walls_size = wall_mesh.getAmountVertexData() * m_walls.getMVs().size();
	unsigned int corners_size = corner_mesh.getAmountVertexData() * m_corners.getMVs().size();
	unsigned int corner_exception_size = corner_exception.getAmountVertexData() * m_corners.getMVs().size();

	unsigned int tot = m_vertices.size() + ((walls_size + corners_size + corner_exception_size) * m_height);
	m_vertices.resize(tot);
	m_normals.resize(tot);
	combinearrays(wall_mesh, dynamic_cast<Object*>(&m_walls), Floor::ALL);
	combinearrays(corner_mesh, dynamic_cast<Object*>(&corners), Floor::ALL);
	combinearrays(corner_exception, dynamic_cast<Object*>(&exception), Floor::ALL);
}

//----------------------------------------------------------------------------------------------------------------------

void Building::makeDecorations()
{
	Mesh decoration = Mesh(selectFolder(m_mode, element::DECORATION), "deco");
	unsigned int deco_size = decoration.getAmountVertexData() * m_walls.getMVs().size();
	m_vertices.resize(m_vertices.size() + (deco_size*(m_height-1)));
	m_normals.resize(m_normals.size() + (deco_size*(m_height-1)));
	combinearrays(decoration, dynamic_cast<Object*>(&m_walls), Floor::NOT_BOTTOM);
}

//----------------------------------------------------------------------------------------------------------------------

void Building::makeWindows()
{
	Mesh window = Mesh(selectFolder(m_mode, element::WINDOW), "win");
	unsigned int window_size = window.getAmountVertexData() * m_walls.getMVs().size();
	m_vertices.resize(m_vertices.size() + (window_size*(m_height-1))); // - 1 because it doesn't include the ground floor
	m_normals.resize(m_normals.size() + (window_size*(m_height-1)));
	combinearrays(window, dynamic_cast<Object*>(&m_walls), Floor::NOT_BOTTOM);
}

//----------------------------------------------------------------------------------------------------------------------

void Building::makeRoof()
{
	Roof roof = Roof(m_walls, m_corners);
	Mesh inside = Mesh("models/cube.obj", "cube");
	Mesh cornerException = Mesh("models/roof_corner_exception.obj", "exception");
	Mesh corner = Mesh("models/roof_corner.obj", "corner");
	Mesh edge = Mesh(selectFolder(m_mode, element::ROOF), "roofEdge");
	// need to construct the corners of the roof, I will create a temporary instance
	// of the corner class, I will first construct it with the exceptions
	// the reconstruct it with the normal cases
	// this will let me have different meshes for the corners
	std::vector<glm::mat4> exceptionMatrices;
	std::vector<glm::mat4> cornerMatrices;
	splitCorners(&cornerMatrices, &exceptionMatrices);
	Corner exception = Corner(exceptionMatrices);
	Corner corners = Corner(cornerMatrices);

	unsigned int cornerException_size = cornerException.getAmountVertexData() * exception.getMVs().size();
	unsigned int corner_size = corner.getAmountVertexData() * corners.getMVs().size();
	unsigned int roof_size = inside.getAmountVertexData() * roof.getMVs().size();
	unsigned int roofEdges_size = edge.getAmountVertexData() * m_walls.getMVs().size();

	unsigned int tot = m_vertices.size() + roof_size + roofEdges_size + cornerException_size + corner_size;
	m_vertices.resize(tot);
	m_normals.resize(tot);
	combinearrays(inside, dynamic_cast<Object*>(&roof), Floor::TOP); // fills the roof
	combinearrays(edge, dynamic_cast<Object*>(&m_walls), Floor::TOP); // place the roof only at the edges
	combinearrays(cornerException, dynamic_cast<Object*>(&exception), Floor::TOP);
	combinearrays(corner, dynamic_cast<Object*>(&corners), Floor::TOP);
}

//----------------------------------------------------------------------------------------------------------------------

void Building::splitCorners(std::vector<glm::mat4> * _standard, std::vector<glm::mat4> * _exception)
{
	for (unsigned int i = 0; i < m_corners.getExceptions().size(); ++i)
	{
		if (m_corners.getExceptions()[i] == 1)
			_exception->push_back(m_corners.getMVs()[i]);
		else
			_standard->push_back(m_corners.getMVs()[i]);
	}
}
