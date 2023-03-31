#pragma once
#include <vector>

#define GL_FLOAT 0x1406
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_INT 0x1404
#define GL_UNSIGNED_BYTE 0x1401


struct Attribute
{
	unsigned count;
	unsigned type;
	unsigned char normalized;
	unsigned int offset;
	static unsigned getTypeSize(unsigned type)
	{
		switch (type)
		{
		case GL_FLOAT:         return 4;
		case GL_INT:           return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		return 0;
	}
};

class VertexDataLayout
{
public:
	typedef std::vector<Attribute> Layout_Container_Type;
	VertexDataLayout();
	~VertexDataLayout();
	const Layout_Container_Type& getElements() const { return m_Elements; };
	const unsigned int getStride() const { return m_Stride; };
	template<typename T>
	VertexDataLayout& push(unsigned int count)
	{
		return false;
	};

	template<>
	VertexDataLayout& push<float>(unsigned int count)
	{
		m_Elements.push_back({ count, GL_FLOAT, GL_FALSE, m_Stride });
		calcOffset(count, GL_FLOAT);
		return *this;
	}

	template<>
	VertexDataLayout& push<int>(unsigned int count)
	{
		m_Elements.push_back({ count, GL_INT, GL_FALSE, m_Stride });
		calcOffset(count, GL_INT);
		return *this;
	}

	template<>
	VertexDataLayout& push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE, m_Stride });
		calcOffset(count, GL_UNSIGNED_BYTE);
		return *this;
	}

private:
	void calcOffset(unsigned int count, unsigned int type);
	Layout_Container_Type m_Elements;
	unsigned int m_Stride;
};