#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <glad/gl.h>
#include <iostream>

#include "Texture.h"

Texture::Texture(const std::string& image, unsigned unit):image_width(0), image_height(0), image_nrChannels(0), unit(GL_TEXTURE0 + unit)
{
	glGenTextures(1, &m_RendererID);
	bind();
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(image.c_str(), &image_width, &image_height, &image_nrChannels, 0);
	if (data)
	{
		unsigned format = getFormat();
		glTexImage2D(GL_TEXTURE_2D, 0, format, image_width, image_height, 0, format, GL_UNSIGNED_BYTE, data);
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

void Texture::bind() const
{
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}