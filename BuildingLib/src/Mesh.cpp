#include "Mesh.h"

Mesh::Mesh()
{

}

//----------------------------------------------------------------------------------------------------------------------

Mesh::Mesh(std::string _address, std::string _name)
{
	m_name = _name;
	std::vector<float> tempVertices;
	std::vector<float> tempNormals;
	std::vector<std::string> tempFaces;
	std::string line;
	std::vector<std::string> tempData;

#ifdef _WIN32
	char separator = '\';
	m_address = Mesh::split(_address,'\');
#else
	char separator = '/';
#endif
	m_address = Mesh::split(_address, separator);

	std::ifstream Source;

	Source.open(_address);
//	assert(Source.is_open());
	if(Source.is_open() == false)
	{
		std::cerr << "the obj does not exist \n";
		return;
	}
	Source.seekg(0, std::ios::end); // starts from the beginning and goes to the end of the file to know the lenght of it
	int len = (int)Source.tellg();
	Source.seekg(0);

	m_minX = 500;
	m_minY = 500;
	m_minZ = 500;
	m_maxX = 0;
	m_maxY = 0;
	m_maxZ = 0;

	m_vertices.resize(3600);
	m_normals.resize(3600);
	tempVertices.resize(3600);
	tempNormals.resize(3600);

	std::size_t i_tempVertices = 0;
	std::size_t i_tempNormals = 0;
	std::size_t iVertices = 0;
	std::size_t iNormals = 0;

	while (len > 0)
	{
		getline(Source, line);
		if (line != "")
		{
			tempData = Mesh::split(line);

			if (tempData[0] == "v")
			{
				if (i_tempVertices == tempVertices.size())
				{
						tempVertices.resize(tempVertices.size()+3600);
					//std::cout << "size " << tempVertices.size() << '\n';
				}
				tempVertices[i_tempVertices++] = std::stof(tempData[1]);
				tempVertices[i_tempVertices++] = std::stof(tempData[2]);
				tempVertices[i_tempVertices++] = std::stof(tempData[3]);

				//std::cout <<"i: " << i_tempVertices << " size: " << tempVertices.size() << '\n';
			}
			else if (tempData[0] == "vn")
			{
				if (i_tempNormals == tempNormals.size())
				{
					tempNormals.resize(tempNormals.size()+3600);
					//std::cout << "size " << tempNormals.size() << '\n';
				}
				tempNormals[i_tempNormals++] = std::stof(tempData[1]);
				tempNormals[i_tempNormals++] = std::stof(tempData[2]);
				tempNormals[i_tempNormals++] = std::stof(tempData[3]);
				//std::cout <<"i: " << i_tempNormals << " size: " << tempNormals.size() << '\n';
			}

			else if (tempData[0] == "f")
			{
				for (int i = 0; i < static_cast<int>(tempData.size()-1); ++i) // -1 to avoid overflow
				{
					tempFaces = Mesh::split(tempData[i+1], separator);

					if (iVertices == m_vertices.size())
					{
						m_vertices.resize(m_vertices.size()+3600);
					}
					m_vertices[iVertices++] = tempVertices[(std::stoi(tempFaces[0])-1)*3];
					m_vertices[iVertices++] = tempVertices[((std::stoi(tempFaces[0])-1)*3)+1];
					m_vertices[iVertices++] = tempVertices[((std::stoi(tempFaces[0])-1)*3)+2];

					if (iNormals == m_normals.size())
					{
						m_normals.resize(m_normals.size()+3600);
					}
					m_normals[iNormals++] = tempNormals[(std::stoi(tempFaces[2])-1)*3];
					m_normals[iNormals++] = tempNormals[((std::stoi(tempFaces[2])-1)*3)+1];
					m_normals[iNormals++] = tempNormals[((std::stoi(tempFaces[2])-1)*3)+2];


					if (tempVertices[(std::stoi(tempFaces[0])-1)*3] < m_minX)
						m_minX = tempVertices[(std::stoi(tempFaces[0])-1)*3];
					else if (tempVertices[(std::stoi(tempFaces[0])-1)*3] > m_maxX)
						m_maxX = tempVertices[(std::stoi(tempFaces[0])-1)*3];
					if (tempVertices[((std::stoi(tempFaces[0])-1)*3)+1] < m_minY)
						m_minY = tempVertices[((std::stoi(tempFaces[0])-1)*3)+1];
					else if (tempVertices[((std::stoi(tempFaces[0])-1)*3)+1] > m_maxY)
						m_maxY = tempVertices[((std::stoi(tempFaces[0])-1)*3)+1];
					if(tempVertices[((std::stoi(tempFaces[0])-1)*3)+2] < m_minZ)
						m_minZ = tempVertices[((std::stoi(tempFaces[0])-1)*3)+2];
					else if(tempVertices[((std::stoi(tempFaces[0])-1)*3)+2] > m_maxZ)
						m_maxZ = tempVertices[((std::stoi(tempFaces[0])-1)*3)+2];
					tempFaces.clear();
				}
			}
		}
		--len;
		tempData.clear();
		line.clear();
	}
	Source.close();
	if (iVertices < m_vertices.size())
	{
		m_vertices.resize(iVertices);
	}
	if (iNormals < m_normals.size())
	{
		m_normals.resize(iNormals);
	}
	//std::cout << m_name << "\t" << m_normals.size() << "\t" << m_vertices.size() <<std::endl;
}

//----------------------------------------------------------------------------------------------------------------------

void Mesh::draw() const
{

}

//----------------------------------------------------------------------------------------------------------------------

std::vector<std::string> Mesh::split(std::string _stringIn, char _splitChar)
{
	int count = 0;
	std::string tempString;
	std::vector<std::string> retVector;
	for (int i = 0; i < static_cast<int>(_stringIn.length()+1); i++)
	{
		if(tempString.length() < 1 && _stringIn[i] == ' ') continue;
		else if(_stringIn[i] != _splitChar)
		{
			tempString += _stringIn[i];
		}
		else
		{
			retVector.resize(retVector.size() + 1);
			retVector[count] = tempString;
			tempString.clear();
			count++;
		}

		if(i == static_cast<int>(_stringIn.length()))
		{
			retVector.resize(retVector.size()+1);
			retVector[count] = tempString;
			tempString.clear();
			count++;
		}
	}
	return retVector;
}

//----------------------------------------------------------------------------------------------------------------------

void Mesh::write(std::string _destination)
{
	std::ofstream out;
	out.open( _destination);
	out.clear();
	for(unsigned int i = 0; i < m_vertices.size(); i+=3)
	{
		out << "v " << m_vertices[i] << " " << m_vertices[i+1] << " " << m_vertices[i+2] << "\n";
	}
	for(unsigned int i = 0; i < m_normals.size(); i+=3)
	{
		out << "vn " << m_normals[i] << " " << m_normals[i+1] << " " << m_normals[i+2] << "\n";
	}

	for(unsigned int i = 0; i < m_vertices.size()/9; ++i)
	{
		out << "f " << (i*3)+1 << "//"<< (i*3)+1 << " " << (i*3)+2 << "//" << (i*3)+2 << " " << (i*3)+3 << "//" << (i*3)+3 << "\n";
	}
}

//----------------------------------------------------------------------------------------------------------------------

void Mesh::write(std::vector<float> const & _vertices, std::vector<float>const & _normals, std::string _destination)
{
	std::ofstream out;
	out.open(_destination);
	out.clear();
	for(unsigned int i = 0; i < _vertices.size(); i+=3)
	{
		out << "v " << _vertices[i] << " " << _vertices[i+1] << " " << _vertices[i+2] << "\n";
	}
	for(unsigned int i = 0; i < _normals.size(); i+=3)
	{
		out << "vn " << _normals[i] << " " << _normals[i+1] << " " << _normals[i+2] << "\n";
	}

	for(unsigned int i = 0; i < _vertices.size()/9; ++i)
	{
		out << "f " << (i*3)+1 << "//"<< (i*3)+1 << " " << (i*3)+2 << "//" << (i*3)+2 << " " << (i*3)+3 << "//" << (i*3)+3 << "\n";
	}
}
//----------------------------------------------------------------------------------------------------------------------

