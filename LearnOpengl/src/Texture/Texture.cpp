#include "kspch.h"
#include "stb_image.h"
#include <glad/gl.h>
#include "Texture.h"

Texture::Texture(const std::string& image, TEXTURE_TYPE type, bool flip_vertically):width(0), height(0), nrChannels(0),m_RendererID(0), path(image), type(type)
{
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(flip_vertically);
	unsigned char* data = stbi_load(image.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		unsigned format = getFormat();
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load Texture:" << image << std::endl;
	}
	unbind();
	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::bind(unsigned slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned Texture::getFormat() const
{
	switch (nrChannels)
	{
	case 1: return GL_RED;
	case 2:	return GL_RG16;
	case 3: return GL_RGB;
	case 4: return GL_RGBA;
	}
	return 0;
}