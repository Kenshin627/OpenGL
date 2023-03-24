class VertexArray;
class IndexBuffer;
class VertexBuffer;
class VertexDataLayout;

class Grid
{
public:
	Grid();
	~Grid();
	void bind();
	void unbind();
private:
	std::shared_ptr<VertexArray>  m_VAO;
	std::shared_ptr<IndexBuffer>  m_IBO;
	std::shared_ptr<VertexBuffer> m_VBO;
	std::shared_ptr<VertexDataLayout> layout;
};