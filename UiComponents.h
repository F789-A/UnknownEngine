#pragma once
#include "EcsSystem.h"
#include <glm/vec2.hpp>
#include "SimpleTextProcessor.h"
#include "GLMaterial.h"
#include "SharedGraphicsResources.h"
#include "FontInfo.h"

class RectTransform : public ecs::Component<RectTransform>
{
public:
	glm::vec2 pos;	// local
	glm::vec2 size; // global
	float priority;

	std::vector<RectTransform*> childs;
	RectTransform* parent = nullptr;

	glm::vec2 globalPos; // global

	void UpdateMatrix();

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& tr = em.GetComponent<RectTransform>(ent);

		tr.pos = TextTools::ReadVec3(res["pos"]);
		tr.size = TextTools::ReadVec3(res["size"]);
		tr.priority = std::stof(res["priority"]);

		auto childs = TextTools::SplitAndDelSpace(res["childs"], ',');
		for (auto& l : childs)
		{
			auto* ch = &em.GetComponent<RectTransform>(std::stoi(l));
			tr.childs.push_back(ch);
			ch->parent = &tr;
		}

	};
};

class Button : public ecs::Component<Button>
{
public:
	int identifier;

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& comp = em.GetComponent<Button>(ent);
		comp.identifier = std::stoi(res["identifier"]);
	};
};

struct Toggle : public ecs::Component<Toggle>
{
	bool active;

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& comp = em.GetComponent<Toggle>(ent);
	};
};

class Image : public ecs::Component<Image>
{
public:
	GLMaterial Material;

	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		auto& im = em.GetComponent<Image>(ent);
		Singleton<SharedGraphicsResources> singleRes;
		im.Material = singleRes->GetMaterial(res["Material"]);
	}
};

struct Text : public ecs::Component<Text>
{ 
	FontInfo* font; //Size //FontPath
	std::string text;
	
	static void Load(ecs::EntityManager& em, int ent, std::map<std::string, std::string>& res)
	{
		Singleton<FontStorage> resources;
		auto& t = em.GetComponent<Text>(ent);
		t.font = &resources->Get(res["FontPath"], std::stoi(res["Size"]));
		t.text = res["Text"];
	}
};