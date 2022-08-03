#pragma once
#include <vector>

enum
{
	EGameLoop,
	EGraphicLoop
};

template<int TypeLoop>
class ILoopUpdate
{
private:
	bool IsSubscribed = false;
	int Index = -1;

	void Subscribe()
	{
		if (!IsSubscribed)
		{
			UpdateVector.push_back(this);
			Index = UpdateVector.size() - 1;
			IsSubscribed = true;
		}
		else
		{
			throw "уже подписан";
		}
	}

public:
	static std::vector<ILoopUpdate*> UpdateVector; // заменить на лист или шаблонный с размером

	virtual void Update() = 0;

	ILoopUpdate operator=(const ILoopUpdate&) = delete;
	ILoopUpdate(const ILoopUpdate&) = delete;
	ILoopUpdate& operator=(ILoopUpdate&&) = delete;
	ILoopUpdate(ILoopUpdate&&) = delete;

protected:
	ILoopUpdate()
	{
		Subscribe();
	}

	~ILoopUpdate()
	{
		UpdateVector.erase(UpdateVector.begin() + Index);
	}
};

template<int TypeLoop>
std::vector<ILoopUpdate<TypeLoop>*> ILoopUpdate<TypeLoop>::UpdateVector = std::vector<ILoopUpdate<TypeLoop>*>();