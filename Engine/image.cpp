#include "image.h"
#include <fstream>

namespace Image
{
	const unsigned char* LoadBMP(const std::string& filename, int& width, int& height, int& bpp)
	{
		char header[54];
		
		std::fstream myfile;
		myfile.open(filename, std::ios::binary);
		if (myfile.is_open())
		{
			std::cout << "Failed to Load File" << std::endl;
			return nullptr;
		}
		myfile.read(header, 54);

		width = *(int*)&(header[18]);
		height = *(int*)&(header[22]);
		bpp = *(int*)&(header[28]);
		int size = (int)&(header[34]);

		unsigned char* data = new unsigned char[size];
		myfile.read((char*)data, size);

		return data;
	}
}