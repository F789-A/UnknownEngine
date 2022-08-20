#include "Texture.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "AlienHeadersLib\stb_image.h"
#endif

Texture::Texture(std::string path)
{
	unsigned char* data = stbi_load(path.c_str(), &Width, &Height, &NumberComponents, 0);
	Data = std::vector<ubyte>(data, data + Width * Height * NumberComponents);
	if (!data)
	{
		throw std::exception("Texture failed to load");
		stbi_image_free(data);
	}
}