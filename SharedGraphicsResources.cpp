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

void GLTextureConteiner::AddGLTexture(const std::string& path)
{
	GLTexture glTex(LinkedTexCont.GetTextureRef(path));
	GLTextures.insert(std::make_pair(path, std::move(glTex)));
}

GLTexture& GLTextureConteiner::GetGLTextureRef(const std::string& identifier)
{
	if (GLTextures.find(identifier) == GLTextures.end())
	{
		AddGLTexture(identifier);
	}
	return GLTextures.at(identifier);
}

void GLTextureConteiner::AddGLCubemap(const std::string& path)
{
	std::ifstream file(path);
	std::string str;
	if (!file)
	{
		throw std::exception("File of cube,ap doesnt open");
	}
	std::vector<Texture*> textures;
	while (std::getline(file, str))
	{
		std::vector<std::string> param = TextTools::SplitAndDelSpace(str, '=');
		textures.push_back(&LinkedTexCont.GetTextureRef(param[1]));
	}
	GLCubemapTexture glCubemap(textures);
	GLCubemaps.insert(std::make_pair(path, std::move(glCubemap)));
}

GLCubemapTexture& GLTextureConteiner::GetGLCubemapRef(const std::string& identifier)
{
	if (GLCubemaps.find(identifier) == GLCubemaps.end())
	{
		AddGLCubemap(identifier);
	}
	return GLCubemaps.at(identifier);
}

void MaterialConteiner::AddMaterial(const std::string& path)
{
	std::ifstream file(path.c_str());
	std::string str;
	std::getline(file, str);
	
	if (!file)
	{
		throw std::exception("File of material doesnt open");
	}
	std::vector<std::string> param;
	GLShader* shader = nullptr;
	std::map<std::string, int> ParametersInt;
	std::map<std::string, float> ParametersFloat;
	std::map<std::string, glm::vec3> ParametersVec3;
	std::map<std::string, GLTexture*> Textures;
	while (file)
	{
		param = TextTools::SplitAndDelSpace(str, ' ');
		if (param[0] == "shader_path")
		{
			shader = &LinkedShaderCont.GetShaderRef(param[1]);
		}
		if (param[0] == "int")
		{
			ParametersInt.insert(std::pair(param[1], std::stoi(param[2])));
		}
		if (param[0] == "float")
		{
			ParametersFloat.insert(std::pair(param[1], (float)std::atof(param[2].c_str())));
		}
		if (param[0] == "vec3")
		{
			std::vector<std::string> vec = TextTools::SplitAndDelSpace(param[2], ',');
			ParametersVec3.insert(std::pair(param[1], glm::vec3((float)std::atof(vec[0].c_str()),
				(float)std::atof(vec[1].c_str()), (float)std::atof(vec[2].c_str()))));
		}
		if (param[0] == "texture")
		{
			Textures.insert(std::pair(param[1], &(LinkedGLTexCont.GetGLTextureRef(param[2]))));
		}
		
		std::getline(file, str);
	}
	Materials.try_emplace(path, *shader); // TODO конструктор и для параметров
	Materials.at(path).ParametersInt = ParametersInt;
	Materials.at(path).ParametersFloat = ParametersFloat;
	Materials.at(path).ParametersVec3 = ParametersVec3;
	Materials.at(path).Textures = Textures;
}

GLMaterial MaterialConteiner::GetMaterial(const std::string& path)
{
	if (Materials.find(path) == Materials.end())
	{
		AddMaterial(path);
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

GLShader& ShaderConteiner::GetShaderRef(std::string path)
{
	if (Shaders.find(path) == Shaders.end())
	{
		AddShader(path);
	}
	return Shaders.at(path);
}

void ShaderConteiner::AddShader(const std::string& path)
{
	std::ifstream file(path);
	std::string str;
	std::getline(file, str);
	std::vector<std::string> param;

	if (!file)
	{
		throw std::exception("File of shader doesnt open");
	}

	bool DepthTest = true;
	bool CullFace = true;
	bool Blend = false;
	int FrontFaceMode = GL_CCW;
	int CullMode = GL_BACK;
	int BlendSourceFunc = GL_SRC_ALPHA;
	int BlendDestinationFunc = GL_ONE_MINUS_SRC_ALPHA;
	int DepthFunc = GL_LEQUAL;
	std::string pathVert;
	std::string pathGeom = "";
	std::string pathFrag;
	while (file)
	{
		param = TextTools::SplitAndDelSpace(str, '=');
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
		else if (param[0] == "DepthTest")
		{
			DepthTest = StringToShaderConst.at(param[1]);
		}
		else if (param[0] == "CullFace")
		{
			CullFace = StringToShaderConst.at(param[1]);
		}
		else if (param[0] == "Blend")
		{
			Blend = StringToShaderConst.at(param[1]);
		}
		else if (param[0] == "FrontFaceMode")
		{
			FrontFaceMode = StringToShaderConst.at(param[1]);
		}
		else if (param[0] == "CullMode")
		{
			CullMode = StringToShaderConst.at(param[1]);
		}
		else if (param[0] == "BlendSourceFunc")
		{
			BlendSourceFunc = StringToShaderConst.at(param[1]);
		}
		else if (param[0] == "BlendDestinationFunc")
		{
			BlendDestinationFunc = StringToShaderConst.at(param[1]);
		}
		else if (param[0] == "DepthFunc")
		{
			DepthFunc = StringToShaderConst.at(param[1]);
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
	GLShader& shader = Shaders.at(path); // Добавить конструктор для параметров
	shader.DepthTest = DepthTest;
	shader.CullFace = CullFace;
	shader.Blend = Blend;
	shader.FrontFaceMode = FrontFaceMode;
	shader.CullMode = CullMode;
	shader.BlendSourceFunc = BlendSourceFunc;
	shader.BlendDestinationFunc = BlendDestinationFunc;
	shader.DepthFunc = DepthFunc;
}

template<>
void SingletonFill<SharedGraphicsResources>(Singleton<SharedGraphicsResources>& a)
{
	static SharedGraphicsResources x;
	a.SetData(&x);
}

template<>
void SingletonFill<FontStorage>(Singleton<FontStorage>& a)
{
	static FontStorage x;
	a.SetData(&x);
}