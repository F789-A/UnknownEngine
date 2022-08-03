#include "Texture.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

Texture::Texture(std::string path)
{
	unsigned char* data = stbi_load(path.c_str(), &Width, &Height, &NumberComponents, 0);
	Data = std::vector<ubyte>(data, data + Width * Height * NumberComponents);
	if (!data)
	{
		//std::cout << "Texture failed to load at path: " << path << std::endl;
		//stbi_image_free(data);
	}
}