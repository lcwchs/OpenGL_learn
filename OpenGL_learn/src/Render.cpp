
#include "Render.h"

#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* filePath, int line)
{
	while (unsigned int error = glGetError()) {
		std::cout << "[OpenGL Error](" << error << ") " << function << " " << filePath << ":" << line << std::endl;
		return false;
	}
	return true;
}