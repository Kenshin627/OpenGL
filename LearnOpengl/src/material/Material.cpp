#include "Material.h"

Material::Material(ShaderType type, const X_Renderer& r) :type(type), renderer(r), program(renderer.getShader(type)) { }

Material::~Material() {}

