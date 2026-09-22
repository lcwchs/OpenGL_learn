#pragma once
class IndexBuffer
{
private:
	unsigned int RendererID_;
	unsigned int count_;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;
};

