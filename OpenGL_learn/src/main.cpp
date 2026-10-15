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
#include "Texture.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

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
	window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
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
			-50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f, 1.0f, 0.0f,
			 50.0f,  50.0f, 1.0f, 1.0f,
			-50.0f,  50.0f, 0.0f, 1.0f
		};

		unsigned int indices[]{
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray vertex_array;
		VertexBuffer vertex_buffer(position, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		vertex_array.AddBuffer(vertex_buffer,layout);

		IndexBuffer index_buffer(indices, 6);

		glm::mat4 proj = glm::ortho(0.0f,1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		Shader shader("./res/shaders/Basic.shader");
		shader.Bind();
		std::string Unifrom_name = "u_Color";
		shader.setUniform4f(Unifrom_name, 0.5f, 0.5f, 0.5f, 1.0f);

		Texture texture("./res/textures/ItTakesTwo.png");
		texture.Bind(0);
		Unifrom_name = "u_Texture";
		shader.setUniform1i(Unifrom_name, 0);

		/*glUseProgram(0);
		GLCall(glBindVertexArray(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));*/

		float color_r = 0.0f;
		float color_increment = 0.05f;
		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		glm::vec3 translateA = glm::vec3(200, 200, 0);
		glm::vec3 translateB = glm::vec3(400, 400, 0);

		//float mat_1 = 1.0f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			ImGui_ImplGlfwGL3_NewFrame();

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translateA);
				glm::mat4 mvp = proj * view * model;
				shader.Bind();
				Unifrom_name = "u_MVP";
				shader.setUniformMat4f(Unifrom_name, mvp);
				renderer.Draw(vertex_array, index_buffer, shader);
			}

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translateB);
				glm::mat4 mvp = proj * view * model;
				shader.Bind();
				Unifrom_name = "u_MVP";
				shader.setUniformMat4f(Unifrom_name, mvp);
				renderer.Draw(vertex_array, index_buffer, shader);
			}

			if (color_r > 1.0f)
				color_increment = -0.05f;
			else if (color_r < 0.0f)
				color_increment = +0.05f;

			color_r += color_increment;

			{
				static float f = 0.0f;
				static int counter = 0;
				ImGui::SliderFloat3("Translation A", &translateA.x, 0.0f, 1920.0f);
				ImGui::SliderFloat3("Translation B", &translateB.x, 0.0f, 1920.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
				ImGui::Text("counter = %d", counter);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}