#include "Material.h"

Material::Material(ShaderType type, std::shared_ptr<X_Renderer> r) :type(type), renderer(r), program(renderer->getShader(type)) { }

Material::~Material() {}

