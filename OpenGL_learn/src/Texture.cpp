#include "Texture.h"
#include "vender/stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	:RendererId_(0), FilePath_(path), LocalBuffer_(nullptr), 
	width_(0), height_(0), BPP_(0)
{
	stbi_set_flip_vertically_on_load(1);
	LocalBuffer_ = stbi_load(path.c_str(), &width_, &height_, &BPP_, 4);

	GLCall(glGenTextures(1, &RendererId_));
	GLCall(glBindTexture(GL_TEXTURE_2D, RendererId_));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, LocalBuffer_));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (LocalBuffer_)
		stbi_image_free(LocalBuffer_);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &RendererId_));
}

void const Texture::Bind(unsigned int slot /*= 0*/) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, RendererId_));
}

void const Texture::UnBind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
