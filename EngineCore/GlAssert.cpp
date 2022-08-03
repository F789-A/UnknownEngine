#include "GlAssert.h"

void GlAssert(bool cond, std::string descr)
{
	if (!cond)
	{
		std::cout << descr;
		glfwTerminate();
		assert(false);
	}
}