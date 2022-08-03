#include "Component.h"

int generateComponentType()
{
	static unsigned int counter = 0;
	return counter++;
}