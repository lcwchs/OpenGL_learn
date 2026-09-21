#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

void main()
{
	FragColor = vec4(0.4, 1, 0.4, 1);
}