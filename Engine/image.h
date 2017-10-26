#pragma once
#include "stdafx.h"

namespace Image
{
	const unsigned char* LoadBMP(const std::string& filename, int& width, int& height, int& bpp);
}