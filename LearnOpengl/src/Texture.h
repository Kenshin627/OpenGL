#pragma once
#include <string>

class Texture
{
public:
	Texture(const std::string& image);
	~Texture();
	void bind() const;
	void unbind() const;
private:
	int image_width;
	int image_height;
	int image_nrChannels;
	unsigned m_RendererID;
};