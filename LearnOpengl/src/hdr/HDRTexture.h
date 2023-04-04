#pragma once
#include <string>

class HDRTexture
{
public:
	HDRTexture(const std::string& path, bool flip_vertically = true);
	~HDRTexture();
	void bind(unsigned slot = 0) const;
	void unbind() const;
	unsigned ID() const { return m_RendererID; };
private:
	unsigned m_RendererID;
	int width;
	int height;
	int nChannels;
	std::string path;
};