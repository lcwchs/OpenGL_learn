#pragma once

class VertexBuffer
{
private:
	unsigned int RendererID_;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;
};

