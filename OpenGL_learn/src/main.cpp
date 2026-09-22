#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>

#include "Render.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

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
	unsigned int program = glCreateProgram();
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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	   
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    //GLCall(glfwSetWindowMonitor(window, monitor, 0, 0, 640, 480, 60));

	//set Interval
	GLCall(glfwSwapInterval(1));

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));
	{
		float position[]{
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
		};
		VertexBuffer vertex_buffer(position, 2 * 4 * sizeof(float));

		unsigned int indices[]{
			0, 1, 2,
			2, 3, 0
		};
		IndexBuffer index_buffer(indices, 6);

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

		ShaderSource shader_source = ParseShader("./res/shaders/Basic.shader");
		std::cout << "vertex shader" << std::endl;
		std::cout << shader_source.vertexShader << std::endl;
		std::cout << "fragment shader" << std::endl;
		std::cout << shader_source.fragmentShader << std::endl;

		unsigned int shader = CreateShader(shader_source.vertexShader, shader_source.fragmentShader);
		glUseProgram(shader);

		GLCall(int location = glGetUniformLocation(shader, "u_Color"));
		ASSERT(location != -1);
		glUniform4f(location, 0.5f, 0.5f, 0.5f, 1.0f);

		glUseProgram(0);
		GLCall(glBindVertexArray(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		float color_r = 0.0f;
		float color_increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shader);
			glUniform4f(location, color_r, 0.5f, 0.5f, 1.0f);
			GLCall(glBindVertexArray(vao));
			index_buffer.Bind();

			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			if (color_r > 1.0f)
				color_increment = -0.05f;
			else if (color_r < 0.0f)
				color_increment = +0.05f;

			color_r += color_increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		glDeleteProgram(shader);
	}
	glfwTerminate();
	return 0;
}