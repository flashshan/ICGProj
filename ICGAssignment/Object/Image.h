#pragma once

#include "Graphics/ImageLoader/lodepng.h"
#include <assert.h>

class Image {
public:
	static Image* LoadFromPath(const char *filePath)
	{
		unsigned char *data = nullptr;
		unsigned int width, height;
		
		if (lodepng_decode32_file(&data, &width, &height, filePath))
		{
			printf_s("Load image %s error!\n", filePath);
			assert(0);
		}

		return new Image(data, width, height);
	}

	unsigned char* GetData()
	{
		return data_;
	}

	unsigned int GetWidth()
	{
		return width_;
	}

	unsigned int GetHeight()
	{
		return height_;
	}

private:
	Image(unsigned char *data, unsigned int width, unsigned int height)
		: data_(data), width_(width), height_(height)
	{
	}

	unsigned char *data_;
	unsigned int width_, height_;
};

