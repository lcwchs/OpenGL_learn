
#include "TestTexture2D.h"
#include "Render.h"
#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include <string>


namespace test {

	TestTexture2D::TestTexture2D()
		:m_TranslateA(200,200,0), m_TranslateB(400, 400, 0)
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

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(position, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		m_Proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		m_shader = std::make_unique<Shader>("./res/shaders/Basic.shader");
		m_shader->Bind();
		std::string Unifrom_name = "u_Color";
		m_shader->setUniform4f(Unifrom_name, 0.5f, 0.5f, 0.5f, 1.0f);

		m_Texture = std::make_unique<Texture>("./res/textures/ItTakesTwo.png");
		m_Texture->Bind(0);
		Unifrom_name = "u_Texture";
		m_shader->setUniform1i(Unifrom_name, 0);
	}

	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{

	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslateA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_shader->Bind();
			std::string Unifrom_name = "u_MVP";
			m_shader->setUniformMat4f(Unifrom_name, mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslateB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_shader->Bind();
			std::string Unifrom_name = "u_MVP";
			m_shader->setUniformMat4f(Unifrom_name, mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_shader);
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		static float f = 0.0f;
		static int counter = 0;
		ImGui::SliderFloat3("Translation A", &m_TranslateA.x, 0.0f, 1920.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslateB.x, 0.0f, 1920.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::Text("counter = %d", counter);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}