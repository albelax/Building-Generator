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

	std::ifstream Source;
	Source.open(_address);
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
			tempData = utilityFunctions::split(line);

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
					tempFaces = utilityFunctions::split(tempData[i+1], '/');

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
