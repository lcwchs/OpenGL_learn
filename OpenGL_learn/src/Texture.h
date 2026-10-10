#pragma once

#include "Render.h"
#include <string>

class Texture
{
private:
	unsigned int RendererId_;
	std::string FilePath_;
	unsigned char* LocalBuffer_;
	int width_, height_, BPP_;

public:
	Texture(const std::string& path);
	~Texture();

	void const Bind(unsigned int slot = 0) const;
	void const UnBind() const;

	inline int GetWidth() const { return width_; }
	inline int GetHeight() const { return height_; }
};
