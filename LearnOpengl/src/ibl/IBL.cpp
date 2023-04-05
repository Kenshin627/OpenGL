#include <glad/gl.h>
#include "IBL.h"
#include "../mesh/BasicMeshes/Box/Box.h"

IBL::IBL(const std::string& path, unsigned cubeMapWidth, unsigned cubeMapHeight, unsigned irradianceWidth, unsigned irradianceHeight, float sampleDelta):
cubeMapWidth(cubeMapWidth),
cubeMapHeight(cubeMapHeight),
irradianceWidth(irradianceWidth),
irradianceHeight(irradianceHeight),
sampleDelta(sampleDelta),
path(path),
hdrTexture(std::make_shared<HDRTexture>(path)),
captureFBO(0),
captureRBO(0),
envCubemap(0),
irradiancemap(0),
prefiltermap(0),
brdfLUTTexture(0),
quadVAO(0),
cube(std::make_shared<BoxMesh>("cube", 2.0f, 2.0f, 2.0f, nullptr)),
captureViews({}),
shaders({}),
captureProjection(glm::perspective(glm::half_pi<float>(), 1.0f, 0.1f, 10.0f))
{
    captureViews.push_back(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    captureViews.push_back(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    captureViews.push_back(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    captureViews.push_back(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    captureViews.push_back(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    captureViews.push_back(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

    //captureFramebuffer
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, cubeMapWidth, cubeMapHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    //envCubemap
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, cubeMapWidth, cubeMapHeight, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //compile shaders
    shaders.insert({ "equirectangularToCubemap", std::make_shared<Shader>(std::vector<std::string>({ "shader/ibl/envCubemap/vertex.glsl", "shader/ibl/envCubemap/fragment.glsl"}))});
    shaders.insert({ "cubemaptoirradiancemap", std::make_shared<Shader>(std::vector<std::string>({ "shader/ibl/irradiancemap/vertex.glsl", "shader/ibl/irradiancemap/fragment.glsl"}))});
    shaders.insert({ "prefiltermap", std::make_shared<Shader>(std::vector<std::string>({ "shader/ibl/prefiltermap/vertex.glsl", "shader/ibl/prefiltermap/fragment.glsl"})) });
    shaders.insert({ "brdfLutmap", std::make_shared<Shader>(std::vector<std::string>({ "shader/ibl/brdfLut/vertex.glsl", "shader/ibl/brdfLut/fragment.glsl"})) });

    //irradianceamap
    glGenTextures(1, &irradiancemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradiancemap);
    for (unsigned i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X +i, 0, GL_RGB16F, irradianceWidth, irradianceHeight, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    //prefiltermap
    glGenTextures(1, &prefiltermap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefiltermap);
    for (unsigned i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    //brdfLUT
    glGenTextures(1, &brdfLUTTexture);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //BRDF Quad
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // setup plane VAO
    unsigned quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
}

IBL::~IBL() { }

void IBL::buildIrradianceMap()
{
    #pragma region equirectangularmap -> cubemap
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    std::shared_ptr<Shader> equirectangularToCubeMap = shaders.find("equirectangularToCubemap")->second;
    equirectangularToCubeMap->bind();
    equirectangularToCubeMap->setMatrix44("captureProjection", captureProjection);
    hdrTexture->bind(0);
    equirectangularToCubeMap->setInt("equirectangularMap", 0);

    glViewport(0, 0, cubeMapWidth, cubeMapHeight);
    
    for (unsigned i = 0; i < 6; i++)
    {
        cube->getVAO()->bind();
        equirectangularToCubeMap->setMatrix44("captureView", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        cube->getVAO()->unbind();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    #pragma endregion

    #pragma region cubemap -> irradiancemap
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, irradianceWidth, irradianceHeight);
    std::shared_ptr<Shader> cubemaptoirradiancemapShader = shaders.find("cubemaptoirradiancemap")->second;
    cubemaptoirradiancemapShader->bind();
    cubemaptoirradiancemapShader->setInt("environmentMap", 0);
    cubemaptoirradiancemapShader->setMatrix44("captureProjection", captureProjection);
    cubemaptoirradiancemapShader->setFloat("sampleDelta", sampleDelta);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    glViewport(0, 0, irradianceWidth, irradianceHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned i = 0; i < 6; i++)
    {
        cube->getVAO()->bind();
        cubemaptoirradiancemapShader->setMatrix44("captureview", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiancemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        cube->getVAO()->unbind();
    }    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    #pragma endregion

    #pragma region cubemap -> prefiltermap
    std::shared_ptr<Shader> prefilterShader = shaders.find("prefiltermap")->second;
    prefilterShader->bind();
    prefilterShader->setInt("environmentMap", 0);
    prefilterShader->setMatrix44("captureProjection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned maxMipLevels = 5;
    for (unsigned mip = 0; mip < maxMipLevels; mip++)
    {
        unsigned mipWidth = static_cast<unsigned>(128 * powf(0.5, mip));
        unsigned mipHeight = static_cast<unsigned>(128 * powf(0.5, mip));
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefilterShader->setFloat("roughness", roughness);
        for (unsigned i = 0; i < 6; i++)
        {
            cube->getVAO()->bind();
            prefilterShader->setMatrix44("captureView", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefiltermap, mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            cube->getVAO()->unbind();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    #pragma endregion

    #pragma region brdfLUTTexture    
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

    glViewport(0, 0, 512, 512);

    std::shared_ptr<Shader> brdfShader = shaders.find("brdfLutmap")->second;
    brdfShader->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //render quad
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    #pragma endregion
}