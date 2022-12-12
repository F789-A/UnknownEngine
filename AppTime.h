#pragma once

class _AppTime
{
public:

	static _AppTime& GetInstance();
	float GetTime();

	float GetDeltaTime();
};

namespace AppTime
{
	float GetTime();

	float GetDeltaTime();
}
