#pragma once
#include "stdafx.h"
#include "glm\glm.hpp"
namespace
{
	bool LoadMesh(std::string filename, glm::vec3& positions, glm::vec3& normals, glm::vec2& uvs)
	{

		/*if (line.substr(0, 2) == "f ")
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
		}*/
	}
}