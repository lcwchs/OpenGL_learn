#pragma once

#include <string>
#include <unordered_map>

enum ShaderType {
	INVALID = -1,
	VERTEX_SHADER = 0,
	FRAGMENT_SHADER = 1
};

struct ShaderSource {
	std::string vertexShader;
	std::string fragmentShader;
};

class Shader
{
public:
	unsigned int Render_id_;
	std::string filepath_;
	std::unordered_map<std::string, int> location_cache;
	
public:
	Shader(const std::string& fliepath);
	~Shader();

	void const Bind() const;
	void const UBind() const;

	void setUniform4f(std::string& name, float v0, float v1, float v2, float v3);
	int GetUniformLocation(const std::string& name);

	ShaderSource ParseShader(const std::string fliepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

private:
	

};
