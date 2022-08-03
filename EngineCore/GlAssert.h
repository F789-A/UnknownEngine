#pragma once
#include <string>
#include <iostream>

#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include <cassert> 

void GlAssert(bool cond, std::string descr);