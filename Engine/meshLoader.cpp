#include "meshLoader.h"

namespace MeshLoader
{
	bool LoadMesh(std::string filename, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs)
	{
		std::vector<glm::vec3> verticesTemp;
		std::vector<glm::vec3> normalsTemp;
		std::vector<glm::vec2> uvsTemp;


		std::ifstream myfile;
		myfile.open(filename, std::ios::binary);
		if (!myfile.is_open())
		{
			std::cout << "Failed to Load File" << std::endl;
			return nullptr;
		}

		std::string line;
		while (std::getline(myfile, line))
		{
			if (line.substr(0, 2) == "v ")
			{
				std::istringstream s(line.substr(2));

				glm::vec3 v;
				s >> v.x;
				s >> v.y;
				s >> v.z;

				verticesTemp.push_back(v);
			}
			if (line.substr(0,3) == "vn ")
			{
				std::istringstream s(line.substr(3));

				glm::vec3 v;
				s >> v.x;
				s >> v.y;
				s >> v.z;

				normalsTemp.push_back(v);
			}
			if (line.substr(0,3) == "vt ")
			{
				std::istringstream s(line.substr(3));

				glm::vec3 v;
				s >> v.x;
				s >> v.y;

				uvsTemp.push_back(v);
			}
			if (line.substr(0, 2) == "f ")
			{
				std::istringstream s(line.substr(2));
				std::string pointString;
				while (std::getline(s, pointString, ' '))
				{
					size_t i = 0;
					unsigned int indices[3];
					std::istringstream pointStream(pointString);
					std::string valueString;
					while (std::getline(pointStream, valueString, '/'))
					{
						std::istringstream valueStream(valueString);
						valueStream >> indices[i++];
					}

					glm::vec3 vertex = verticesTemp[indices[0] - 1];
					vertices.push_back(vertex);

					glm::vec2 uv = uvsTemp[indices[1] - 1];
					uvs.push_back(uv);

					glm::vec3 normal = normalsTemp[indices[2] - 1];
					normals.push_back(normal);
				}
			}

		}

		return true;
	}
}