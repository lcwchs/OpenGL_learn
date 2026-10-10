#include "Shader.h"
#include <fstream>
#include <sstream>
#include <functional>
#include "Render.h"
#include <iostream>

Shader::Shader(const std::string& filepath)
	:filepath_(filepath), Render_id_(0)
{
	ShaderSource shader_source =  ParseShader(filepath);
	Render_id_=  CreateShader(shader_source.vertexShader, shader_source.fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(Render_id_);
}

void const Shader::Bind() const
{
	glUseProgram(Render_id_);
}

void const Shader::UBind() const
{
	glUseProgram(0);
}

ShaderSource Shader::ParseShader(const std::string shaderPath) {
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
			else if (line.find("fragment") != std::string::npos)
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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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

int Shader::GetUniformLocation(const std::string& name)
{
	if (location_cache.find(name) != location_cache.end())
		return location_cache[name];
	GLCall( int location = glGetUniformLocation(Render_id_, name.c_str()));
	if (location == -1)
	{
		std::cout << "waring: uiform " << name << "doesn't exist !" << std::endl;
	}
	location_cache[name] = location;
	return location;
}

void Shader::setUniform1i(std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::setUniform4f(std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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
