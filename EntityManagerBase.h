#pragma once


namespace ec
{
	class EntityManagerBase
	{
	protected:
		virtual int EntityCount() = 0;

		virtual ~EntityManagerBase() {};
	};
}