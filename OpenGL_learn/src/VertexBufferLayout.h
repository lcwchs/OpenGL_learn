#pragma once

#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H
#endif
#include <vector>
#include <GL/glew.h>
#include "Render.h"

struct VertexBufferElement 
{
	unsigned int type;
	unsigned int count;
	unsigned int normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> Elements_;
	unsigned int Stride_;
public:
	VertexBufferLayout()
		:Stride_(0) {}
	~VertexBufferLayout();

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false, "message");
	}
	template<>
	void Push<float>(unsigned int  count)
	{
		Elements_.push_back({ GL_FLOAT, count, GL_FALSE });
		Stride_ += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		Elements_.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		Stride_ += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		Elements_.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		Stride_ += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}
	
	inline const std::vector<VertexBufferElement> GetElements() const { return Elements_; }
	inline unsigned int GetStride() const { return Stride_; }
};




