#pragma once
#include <string>

enum TEXTURE_TYPE
{
	DIFFUSE,
	SPECULAR,
	AMBIENT,
	NORMALMAP
};

class Texture
{
public:
	Texture(const std::string& image, TEXTURE_TYPE type, bool flip_vertically = false);
	~Texture();
	void bind(unsigned slot = 0) const;
	void unbind() const;
	inline unsigned int getFormat() const
	{
		switch (nrChannels)
		{
			case 3: return GL_RGB;
			case 4: return GL_RGBA;		
		}
		return 0;
	}
	inline unsigned ID() const
	{
		return m_RendererID;
	}
private:
	int width;
	int height;
	int nrChannels;
	unsigned m_RendererID;
	std::string path;
	TEXTURE_TYPE type;
};