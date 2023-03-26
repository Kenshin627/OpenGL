#include <glad/gl.h>
#include "../../../vendor/stb_image/stb_image.h"
#include "SkyBox.h"

SkyBox::SkyBox(const std::vector<std::string>& faces, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    //buffers
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
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
 
    //glGenTextures(1, &m_TextureID);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

    //int width, height, nrChannels;
    //for (unsigned int i = 0; i < faces.size(); i++)
    //{
    //    unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    //    if (data)
    //    {
    //        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
    //            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
    //        );
    //        stbi_image_free(data);
    //    }
    //    else
    //    {
    //        std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
    //        stbi_image_free(data);
    //    }
    //}
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    //program
    program = std::make_unique<Shader>(vertexShaderPath, fragmentShaderPath);
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