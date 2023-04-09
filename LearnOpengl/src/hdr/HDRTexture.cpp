#include "kspch.h"
#include <glad/gl.h>
#include "stb_image.h"
#include "HDRTexture.h"

HDRTexture::HDRTexture(const std::string& path, bool flip_vertically):
m_RendererID(0),
width(0),
height(0),
nChannels(0),
path(path)
{
	stbi_set_flip_vertically_on_load(flip_vertically);
	float* data = stbi_loadf(path.c_str(), &width, &height, &nChannels, 0);
	if (data)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else {
		std::cout << "Failed to load HDRTexture:" << path << std::endl;
	}
}

HDRTexture::~HDRTexture()
{
	//glDeleteTextures(1, &m_RendererID);
}

void HDRTexture::bind(unsigned slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void HDRTexture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

