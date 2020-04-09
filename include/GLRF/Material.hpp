#pragma once
#include <iterator>
#include <map>
#include <variant>
#include <glm/glm.hpp>

#include <GLRF/Texture.hpp>

template <typename T>
class MaterialProperty {
public:
	bool use_texture;
	T value_default;
	Texture texture;

	MaterialProperty();
	void loadTexture(std::string texture_name, std::string separator, std::string value_name, std::string fileType);
private:
	static const char period = '.';
};

class Material {
public:
	MaterialProperty<glm::vec3> albedo;
	MaterialProperty<glm::vec3> normal;
	MaterialProperty<float> roughness;
	MaterialProperty<float> metallic;
	MaterialProperty<float> ao;
	MaterialProperty<float> height;

	float height_scale;

	Material();

	void loadTextures(std::string name, std::string separator, std::string fileType);
	void bindTextures(unsigned int textureUnitsBegin);
};