#include "ObjImporter.h"

namespace ObjFileIdentifier
{
	constexpr const char* MtlLib = "mtllib";
	constexpr const char* Vert = "v";
	constexpr const char* VertCoord = "vt";
	constexpr const char* VertNorm = "vn";
	constexpr const char* Object = "o";
	constexpr const char* Face = "f";

	//constexpr const char* NewMatMtl = "newmtl";
}

//заменить pushback

std::vector<int> SplitToNum(std::string word)
{
	std::vector<int> result;
	std::string str;
	for (int i = 0; i < word.length(); i++)
	{
		if (word[i] == '/')
		{
			int res = std::stoi(str);
			result.push_back(res);
			str = "";
		}
		else
		{
			str += word[i];
		}
	}
	int res = std::stoi(str);
	result.push_back(res);
	return result;
}

Model ImportObj(std::filesystem::path path)
{
	std::map<std::string, Mesh> meshes;
	std::map<std::string, std::string> texture;
	std::string name;
	std::string mtlLibFile;
	int test = 0;
	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> tex;
	std::vector<glm::vec3> norm;

	std::ifstream objFile(path);

	if (!objFile)
	{
		throw std::exception("ћодель не найдена");
	}

	std::string str;

	while (std::getline(objFile, str))
	{
		std::istringstream ist(str);
		std::string word;
		ist >> word;

		if (word == ObjFileIdentifier::MtlLib)
		{
			ist >> mtlLibFile;
		}

		else if (word == ObjFileIdentifier::Vert)
		{
			glm::vec3 vPos;
			for (int i = 0; i < 3; i++)
			{
				ist >> vPos[i];
			}
			pos.push_back(vPos);
		}
		else if (word == ObjFileIdentifier::VertCoord)
		{
			glm::vec3 v_tex;
			for (int i = 0; i < 3; i++)
			{
				ist >> v_tex[i];
			}
			tex.push_back(v_tex);
		}
		else if (word == ObjFileIdentifier::VertNorm)
		{
			glm::vec3 vNorm;
			for (int i = 0; i < 3; i++)
			{
				ist >> vNorm[i];
			}
			norm.push_back(vNorm);
		}
		else if (word == ObjFileIdentifier::Object)
		{
			ist >> name;
			meshes.insert({ name, Mesh() });
			meshes[name].Name = name;
		}
		else if (word == "usemtl")
		{
			//std::string name_tex;
			//ist >> name_tex;
			//texture.insert({ name_tex, name });
			//Ќа следующие полигоны исп опред материал
		}
		else if (word == ObjFileIdentifier::Face)
		{
			std::list<unsigned int> indecesOfPolygon;
			
			for (; ist >> word;)
			{
				Vertex vert;
				std::vector<int> index = SplitToNum(word);
				vert.position = pos[index[0] - 1]; // TODO норм split сделать
				vert.texCoords = tex[index[1] - 1];
				vert.normal = norm[index[2] - 1];
				meshes[name].Vertices.push_back(vert);

				//indecesOfPolygon.push_back(index[0] - 1);
				indecesOfPolygon.push_back(test);
				test++;
			}

			//std::vector<unsigned int> triangulateIndeces = Triangulate(pos, indecesOfPolygon);

			meshes[name].Indices.insert(meshes[name].Indices.end(), indecesOfPolygon.begin(), indecesOfPolygon.end());

			//for (int i = 0; i < triangulateIndeces.size(); i++)
			//{
			//	meshes[name].Indices.push_back(triangulateIndeces[i]);
			//}
		}
	}

	//
	std::array<glm::vec3, 3> posT;
	std::array<glm::vec2, 3> uvT;
	for (int i = 0; i < meshes[name].Indices.size(); i += 3)
	{
		posT[0] = meshes[name].Vertices[meshes[name].Indices[i]].position;
		posT[1] = meshes[name].Vertices[meshes[name].Indices[i + 1]].position;
		posT[2] = meshes[name].Vertices[meshes[name].Indices[i + 2]].position;

		uvT[0] = meshes[name].Vertices[meshes[name].Indices[i]].texCoords;
		uvT[1] = meshes[name].Vertices[meshes[name].Indices[i + 1]].texCoords;
		uvT[2] = meshes[name].Vertices[meshes[name].Indices[i + 2]].texCoords;

		glm::vec3 tangent = CalculateTangent(posT, uvT);

		meshes[name].Vertices[meshes[name].Indices[i + 0]].tangentVec = tangent;
		meshes[name].Vertices[meshes[name].Indices[i + 1]].tangentVec = tangent;
		meshes[name].Vertices[meshes[name].Indices[i + 2]].tangentVec = tangent;
	}


	/*mtllib = (std::filesystem::path(path).parent_path()).string() + "//" + mtllib;

	std::ifstream mtlFile(mtllib);

	while (std::getline(mtlFile, str))
	{
		std::istringstream ist(str);
		std::string word;
		ist >> word;

		if (word == "newmtl")
		{
			std::string a;
			ist >> a;
			name = texture[a];
		}
		else if (word == "map_Kd")
		{
			std::string a;
			ist >> a;
			Texture mesh_t;
			mesh_t.id = TextureFromFile(a.c_str());
			mesh_t.name = "diffuse";
			mesh_t.path = a;
			meshes[name].textures.push_back(mesh_t);
		}
		else if (word == "map_Norm")
		{
			std::string a;
			ist >> a;
			Texture mesh_t;
			mesh_t.id = TextureFromFile(a.c_str());
			mesh_t.name = "normal";
			mesh_t.path = a;
			meshes[name].textures.push_back(mesh_t);
		}
		else if (word == "map_Heig")
		{
			std::string a;
			ist >> a;
			Texture mesh_t;
			mesh_t.id = TextureFromFile(a.c_str());
			mesh_t.name = "height";
			mesh_t.path = a;
			meshes[name].textures.push_back(mesh_t);
		}
	}*/

	//Move from map to vector

	std::vector<Mesh> _meshes;
	for (std::map<std::string, Mesh>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		_meshes.push_back(std::move(it->second));
	}

	//CreateModel

	Model resultModel;
	resultModel.Directory = path.string();
	resultModel.Meshes = std::move(_meshes);
	return resultModel;
}