#pragma once
#include <vector>
#include <exception>

enum class UpdateType
{
	GameLoop,
	GraphicLoop
};

template<UpdateType TypeLoop>
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
			throw std::exception("Component already subscribed");
		}
	}

public:
	static std::vector<ILoopUpdate*> UpdateVector; // TODO: замерить производительность с другими контейнерами

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

	~ILoopUpdate() //TODO: неэффективное удаление, подумать над решением
	{
		for (int i = 0; i < UpdateVector.size(); i++)
		{
			if (UpdateVector[i]->Index = Index)
			{
				UpdateVector.erase(UpdateVector.begin() + i);
				break;
			}
		}
	}
};

template<UpdateType TypeLoop>
std::vector<ILoopUpdate<TypeLoop>*> ILoopUpdate<TypeLoop>::UpdateVector = std::vector<ILoopUpdate<TypeLoop>*>();