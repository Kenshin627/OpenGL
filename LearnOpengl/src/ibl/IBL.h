#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <array>
#include <unordered_map>
#include "../hdr/HDRTexture.h"

class Shader;
class BoxMesh;

class IBL
{
public:
	IBL(const std::string& path, unsigned cubeMapWidth = 512, unsigned cubeMapHeight = 512, unsigned irradianceWidth = 32, unsigned irradianceHeight = 32, float sampleDelta = 0.025);
	~IBL();
	void buildIrradianceMap();
	unsigned getENVCubemap() const { return envCubemap; };
	unsigned getIrradiancemap() const { return irradiancemap; };
private:
	unsigned cubeMapWidth;
	unsigned cubeMapHeight;
	unsigned irradianceWidth;
	unsigned irradianceHeight;
	float sampleDelta;
	std::string path;
	std::shared_ptr<HDRTexture> hdrTexture;
	unsigned captureFBO;
	unsigned captureRBO;
	unsigned envCubemap;
	unsigned irradiancemap;
	std::shared_ptr<BoxMesh> cube;
	glm::mat4x4 captureProjection;
	std::vector<glm::mat4x4> captureViews;
	std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
};