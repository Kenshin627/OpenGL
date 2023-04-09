#include "kspch.h"
#include <glad/gl.h>
#include "../../../vendor/stb_image/stb_image.h"
#include "SkyBox.h"

SkyBox::SkyBox(const std::vector<std::string>& faces, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
#pragma region MyRegion

#pragma endregion

	m_VAO = std::make_unique<VertexArray>();
	m_VAO->bind();
    std::vector<float> vertices = {
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
    };
	m_VBO = std::make_unique<VertexBuffer>(
        vertices.data(), vertices.size() * sizeof(float)
		);
    m_VBO->bind();
    m_Layout = std::make_unique<VertexDataLayout>();
    m_Layout->push<float>(3);
    m_VAO->AddBuffer(*m_VBO, *m_Layout);
    m_VAO->unbind();

    //textures
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
    int width, height, nChannels;
    for (unsigned i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nChannels, 0);
        auto format = getFormat(nChannels);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    //program
    program = std::make_unique<Shader>(std::vector<std::string>{ vertexShaderPath, fragmentShaderPath });
}

void SkyBox::bind()
{
    m_VAO->bind();
}

void SkyBox::unbind()
{
    m_VAO->unbind();
}

void SkyBox::draw(const Camera& camera, unsigned slot)
{
    glDepthFunc(GL_LEQUAL);
    program->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
    program->setInt("skyBox", 0);
    program->setMatrix44("projection", camera.projMatrix());
    program->setMatrix44("skyBoxView", glm::mat4x4(glm::mat3x3(camera.viewMatrix())));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
}

unsigned SkyBox::getTextureID() const {
    return m_TextureID;
}

void SkyBox::setTextureID(unsigned id) 
{
    m_TextureID = id;
}

unsigned SkyBox::getFormat(unsigned nrChannels) const
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