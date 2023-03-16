#pragma once
#include <string>

class Texture
{
public:
	Texture(const std::string& image, unsigned unit);
	~Texture();
	void bind() const;
	void unbind() const;
	inline unsigned getTextureUnit() const
	{
		return unit;
	}
	inline unsigned int getFormat() const
	{
		switch (image_nrChannels)
		{
		case 3: return GL_RGB;
		case4: return GL_RGBA;		
		}
		return 0;
	}
	inline unsigned ID() const
	{
		return m_RendererID;
	}
private:
	int image_width;
	int image_height;
	int image_nrChannels;
	unsigned m_RendererID;
	unsigned unit;
};