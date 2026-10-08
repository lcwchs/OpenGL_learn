#include "VertexArray.h"
#include "Render.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &RenderID_));
	//GLCall(glBindVertexArray(RenderID_));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &RenderID_));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layoout)
{
	Bind();
	vb.Bind();
	const auto& elements = layoout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layoout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(RenderID_));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

