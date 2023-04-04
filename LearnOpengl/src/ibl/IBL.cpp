#include <glad/gl.h>
#include "IBL.h"
#include "../mesh/BasicMeshes/Box/Box.h"
#include "../vendor/stb_image/stb_image.h"

IBL::IBL(const std::string& path, unsigned cubeMapwidth, unsigned cubeMapheight, unsigned irradiancewidth, unsigned irradianceheight, float sampleDelta):
cubeMapWidth(cubeMapwidth),
cubeMapHeight(cubeMapheight),
irradianceWidth(irradiancewidth),
irradianceHeight(irradianceheight),
sampleDelta(sampleDelta),
path(path),
hdrTexture(std::make_shared<HDRTexture>(path)),
captureFBO(0),
captureRBO(0),
envCubemap(0),
irradiancemap(0),
cube(std::make_shared<BoxMesh>("cube", 2.0f, 2.0f, 2.0f, nullptr)),
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
    shaders.insert({ "equirectangularToCubemap", std::make_shared<Shader>(std::vector<std::string>({ "shader/ibl/envCubemap/vertex.glsl", "shader/ibl/envCubemap/fragment.glsl"})) });
    shaders.insert({ "cubemaptoirradiancemap", std::make_shared<Shader>(std::vector<std::string>({ "shader/ibl/irradiancemap/vertex.glsl", "shader/ibl/irradiancemap/fragment.glsl"})) });

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //irradianceamap
    /*glGenTextures(1, &irradiancemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradiancemap);
    for (unsigned i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X +i, 0, GL_RGB16F, irradianceWidth, irradianceHeight, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

   
}

IBL::~IBL()
{
    
}



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

  /*  #pragma region cubemap -> irradiancemap
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
        cubemaptoirradiancemapShader->setMatrix44("captureview", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X + i, irradiancemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    cube->getVAO()->unbind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
    #pragma endregion
}