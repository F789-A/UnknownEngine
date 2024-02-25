#pragma once

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

	float GetDeltaTime() const;

	void RunLoop();
};