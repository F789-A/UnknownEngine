#pragma once
#include "EngineCore/WindowApp.h"

#include "Graphics/GraphicCore.h"
#include "Input/Input.h"

#include <vector>

#include "EntityLoader.h"
#include "EntityManager.h"

#include "ILoopUpdate.h"

#include "EntityLoader.h"

class GameLoop
{
private:
	float DeltaTime = 0;

	void ConstructScene();
	void Loop();
public:
	

	GameLoop() = default;
	GameLoop(const GameLoop&) = delete;
	GameLoop operator=(const GameLoop&) = delete;
	~GameLoop() = default;

	static GameLoop& GetInstance();

	float GetDeltaTime();

	void RunLoop();
};