#pragma once


namespace ec
{
	class EntityManagerBase
	{
	protected:
		virtual int EntityCount() const = 0;

		virtual ~EntityManagerBase() {};
	};
}