#pragma once

#include <string>
#include <vector>
#include <map>

#include "Assets\Model.h"
#include "Assets\ModelImporter.h"
#include "Assets\Texture.h"

#include "Graphics\GLMesh.h"
#include "Graphics\GLMaterial.h"
#include "Graphics\GLTexture.h"
#include "Graphics\GLShader.h"
#include "Graphics\FontInfo.h"

#include "Utils\Singleton.h"
#include "Utils\SimpleTextProcessor.h"

#include "StringToGLFWKey.h"


class ModelConteiner
{
private:
	std::map <std::string, Model> Models;

public:
	void AddModel(std::string path);
	Model& GetModelRef(std::string ind);
};

class TextureConteiner
{
private:
	std::map<std::string, Texture> Textures;

public:
	void AddTexture(std::string path);
	Texture& GetTextureRef(std::string path);
};

class GLTextureConteiner
{
private:
	TextureConteiner& LinkedTexCont;
	std::map <std::string, GLTexture> GLTextures;
	std::map <std::string, GLCubemapTexture> GLCubemaps;

public:
	GLTextureConteiner(TextureConteiner& texCont) : LinkedTexCont(texCont) {};

	void AddGLTexture(const std::string& path);
	GLTexture& GetGLTextureRef(const std::string& ind);

	void AddGLCubemap(const std::string& ind);
	GLCubemapTexture& GetGLCubemapRef(const std::string& ind);
};

class ShaderConteiner
{
private:
	std::map<std::string, GLShader> Shaders;

public:
	GLShader& GetShaderRef(std::string name);
	void AddShader(const std::string& path);
};

//хранит образцы материалов
class MaterialConteiner
{
private:
	ShaderConteiner& LinkedShaderCont;
	GLTextureConteiner& LinkedGLTexCont;

	std::map<std::string, GLMaterial> Materials;

public:
	MaterialConteiner(ShaderConteiner& shaderCont, GLTextureConteiner& glTexCont) : LinkedShaderCont(shaderCont), LinkedGLTexCont(glTexCont){};

	void AddMaterial(const std::string& path);
	GLMaterial GetMaterial(const std::string& path);
};

class SharedGraphicsResources
{
public:
	ModelConteiner ModelCont;
	TextureConteiner TextureCont;
	GLTextureConteiner GLTextureCont;
	MaterialConteiner MaterialCont;
	ShaderConteiner ShaderCont;

public:

	SharedGraphicsResources() : 
		ModelCont(), TextureCont(), GLTextureCont(TextureCont), ShaderCont(), MaterialCont(ShaderCont, GLTextureCont){}

	GLShader& GetShaderRef(std::string path)
	{
		return ShaderCont.GetShaderRef(path);
	}

	GLMaterial GetMaterial(std::string path)
	{
		return MaterialCont.GetMaterial(path);
	}

	Texture& GetTextureRef(std::string path)
	{
		return TextureCont.GetTextureRef(path);
	}

	GLTexture& GetGLTextureRef(std::string path)
	{
		return GLTextureCont.GetGLTextureRef(path);
	}
	GLCubemapTexture& GetGLCubemapRef(std::string path)
	{
		return GLTextureCont.GetGLCubemapRef(path);
	}
};

class FontStorage
{
public:
	std::map<std::string, FontInfo> loadedFonts;

	FontInfo& Get(const std::string& path, int size)
	{
		if (loadedFonts.find(path) == loadedFonts.end())
		{
			LoadFont(path, size);
		}
		return loadedFonts.at(path);
	}

	void LoadFont(const std::string& path, int size)
	{
		loadedFonts.emplace(path, FontInfo(path, size));
	}
};

template<>
void SingletonFill<SharedGraphicsResources>(Singleton<SharedGraphicsResources>& a);

template<>
void SingletonFill<FontStorage>(Singleton<FontStorage>& a);