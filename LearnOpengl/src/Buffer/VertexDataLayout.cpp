#include "VertexDataLayout.h"

VertexDataLayout::VertexDataLayout():m_Stride(0) {}
VertexDataLayout::~VertexDataLayout() {}

void VertexDataLayout::calcOffset(unsigned int count, unsigned int type)
{
	m_Stride += count * Attribute::getTypeSize(type);
}
