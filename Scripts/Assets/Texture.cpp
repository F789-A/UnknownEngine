#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "AlienHeadersLib\stb_image.h"

Texture::Texture(const std::string& path)
{
	unsigned char* data = stbi_load(path.c_str(), &Width, &Height, &NumberComponents, 0);
	if (!data)
	{
		throw std::exception("Texture failed to load");
	}
	Data = std::vector<ubyte>(data, data + Width * Height * NumberComponents);
	stbi_image_free(data);
}

Texture::Texture(int w, int h, int num) : Width(w), Height(h), NumberComponents(num), Data(w* h* num)
{}