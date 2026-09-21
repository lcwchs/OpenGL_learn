#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() 
{
	while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* function, std::string filePath, int line) 
{
	while (unsigned int error = glGetError()) {
		std::cout << "[OpenGL Error](" << error << ") " << function << " " << filePath << ":" << line << std::endl;
		return false;
	}
	return true;
}

enum ShaderType{
	INVALID = -1,
	VERTEX_SHADER = 0,
	FRAGMENT_SHADER = 1
};

struct ShaderSource{
	std::string vertexShader;
	std::string fragmentShader;
};

static  ShaderSource ParseShader(const std::string shaderPath) {
	std::ifstream file;
	file.open(shaderPath.c_str());
	std::string line;
	int shader_type = ShaderType::INVALID;
	std::stringstream shader_stream[2];
	ShaderSource shader_source;
	while (std::getline(file, line))
	{
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				shader_type = ShaderType::VERTEX_SHADER;
			else if(line.find("fragment") != std::string::npos)
				shader_type = ShaderType::FRAGMENT_SHADER;
		}
		else {
			shader_stream[shader_type] << line << '\n';
		}
	}
	file.close();
	shader_source.vertexShader = shader_stream[VERTEX_SHADER].str();
	shader_source.fragmentShader = shader_stream[FRAGMENT_SHADER].str();
	return shader_source;
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(size_t(length * sizeof(char)));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "Shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id; 
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
	int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	   
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	float position[]{
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
	};
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, position, GL_STATIC_DRAW);

	unsigned int indices[]{
		0, 1, 2,
		2, 3, 0
	};
	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);

	ShaderSource shader_source = ParseShader("./res/shaders/Basic.shader");
	std::cout << "vertex shader" << std::endl;
	std::cout << shader_source.vertexShader << std::endl;
	std::cout << "fragment shader" << std::endl;
	std::cout << shader_source.fragmentShader << std::endl;

	unsigned int shader = CreateShader(shader_source.vertexShader, shader_source.fragmentShader);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}