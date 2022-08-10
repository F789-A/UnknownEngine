#include "SharedGraphicsResources.h"

void ModelConteiner::AddModel(std::string path)
{
	Model mod = ImportModel(path);
	Models.insert(std::make_pair(path, std::move(mod)));
}

Model& ModelConteiner::GetModelRef(std::string identifier)
{
	if (Models.find(identifier) == Models.end())
	{
		AddModel(identifier);
	}
	return Models.at(identifier);
}

GLShader& ShaderConteiner::GetShaderRef(std::string path)
{
	if (Shaders.find(path) == Shaders.end())
	{
		AddShader(path);
	}
	return Shaders.at(path);
}

std::vector<std::string> GetParameter(std::string str)
{
	std::vector<std::string> res(2);
	size_t pos = str.find("=");;
	res[0] = str.substr(0, pos - 1);
	res[1] = str.substr(pos + 2, str.size());
	return res;
}

void ShaderConteiner::AddShader(std::string path)
{
	std::ifstream file(path.c_str());
	std::string str;
	std::getline(file, str);
	std::vector<std::string> param;
	if (!file)
	{
		throw std::exception("Файл шейдера не открыт");
	}
	std::string pathVert;
	std::string pathGeom = "";
	std::string pathFrag;
	while (file)
	{
		param = GetParameter(str);
		if (param[0] == "shader_path_vert")
		{
			pathVert = param[1];
		}
		else if (param[0] == "shader_path_geom")
		{
			pathGeom = param[1];
		}
		else if (param[0] == "shader_path_frag")
		{
			pathFrag = param[1];
		}
		std::getline(file, str);
	}
	if (pathGeom.size() == 0)
	{
		Shaders.try_emplace(path, pathVert.c_str(), pathFrag.c_str());
	}
	else
	{
		Shaders.try_emplace(path, pathVert.c_str(), pathFrag.c_str(), pathGeom.c_str());
	}
}


void MaterialConteiner::AddMaterial(std::string path, ShaderConteiner& shaderCont)
{
	std::ifstream file(path.c_str());
	std::string str;
	std::getline(file, str);
	std::vector<std::string> param;

	if (!file)
	{
		throw std::exception("Файл материала не открыт");
	}
	
	GLShader* shader = nullptr;
	while (file)
	{
		param = GetParameter(str);
		if (param[0] == "shader_path")
		{
			shader = &shaderCont.GetShaderRef(param[1]);
		}
		
		std::getline(file, str);
	}
	Materials.try_emplace(path, *shader);
}

GLMaterial MaterialConteiner::GetMaterial(std::string path, ShaderConteiner& shaderCont)
{
	if (Materials.find(path) == Materials.end())
	{
		AddMaterial(path, shaderCont);
	}
	return Materials.at(path);
}

void TextureConteiner::AddTexture(std::string path)
{
	Textures.try_emplace(path, Texture(path));
}
Texture& TextureConteiner::GetTextureRef(std::string path)
{
	if (Textures.find(path) == Textures.end())
	{
		AddTexture(path);
	}
	return Textures.at(path);
}

template<>
void SingletonFill<SharedGraphicsResources>(Singleton<SharedGraphicsResources>& a)
{
	static SharedGraphicsResources x;
	a.SetData(&x);
}