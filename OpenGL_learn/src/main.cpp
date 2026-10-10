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
#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "Shader.h"

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

	{
		float position[]{
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
		};

		unsigned int indices[]{
			0, 1, 2,
			2, 3, 0
		};

		VertexArray vertex_array;
		VertexBuffer vertex_buffer(position, 2 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		vertex_array.AddBuffer(vertex_buffer,layout);

		IndexBuffer index_buffer(indices, 6);

		//GLCall(glEnableVertexAttribArray(0));
		//GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

		Shader shader("./res/shaders/Basic.shader");
		shader.Bind();
		std::string Unifrom_name = "u_Color";
		shader.setUniform4f(Unifrom_name, 0.5f, 0.5f, 0.5f, 1.0f);
		/*glUseProgram(0);
		GLCall(glBindVertexArray(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));*/

		float color_r = 0.0f;
		float color_increment = 0.05f;
		Renderer renderer;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.Bind();
			shader.setUniform4f(Unifrom_name, color_r, 0.5f, 0.5f, 1.0f);

			renderer.Draw(vertex_array, index_buffer, shader);

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
	}
	glfwTerminate();
	return 0;
}