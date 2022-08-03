#pragma once

#include <string>
#include <vector>
#include <map>
#include "GLShader.h"
#include "Model.h"
#include "GLMesh.h"
#include "GLMaterial.h"
#include "GLTexture.h"
#include "Texture.h"
#include "ModelImporter.h"

//хранит модели
class ModelConteiner
{
private:
	std::map <std::string, Model> Models;

public:
	void AddModel(std::string path);
	Model& GetModelRef(std::string ind);
};

//хранит шейдеры
class ShaderConteiner
{
private:
	std::map<std::string, GLShader> Shaders;

public:
	GLShader& GetShaderRef(std::string name);
	void AddShader(std::string path);
};

//хранит образцы материалов
class MaterialConteiner
{
private:
	std::map<std::string, GLMaterial> Materials;

public:
	void AddMaterial(std::string path, ShaderConteiner& shaderCont);
	GLMaterial GetMaterial(std::string path, ShaderConteiner& shaderCont);
};

class TextureConteiner
{
private:
	std::map<std::string, Texture> Textures;

public:
	void AddTexture(std::string path);
	Texture& GetTextureRef(std::string path);
};

class SharedGraphicsResources
{
public:
	ModelConteiner ModelCont;
	ShaderConteiner ShaderCont;
	MaterialConteiner MaterialCont;
	TextureConteiner TextureCont;

public:

	SharedGraphicsResources() : ModelCont(), ShaderCont(), MaterialCont(), TextureCont(){}

	GLShader& GetShaderRef(std::string path)
	{
		return ShaderCont.GetShaderRef(path);
	}

	GLMaterial GetMaterial(std::string path)
	{
		return MaterialCont.GetMaterial(path, ShaderCont);
	}

	Texture& GetTextureRef(std::string path)
	{
		return TextureCont.GetTextureRef(path);
	}
};

