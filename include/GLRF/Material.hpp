#pragma once
#include <iterator>
#include <map>
#include <variant>
#include <glm/glm.hpp>

#include <GLRF/Texture.hpp>

/**
 * A single property (like color or roughness) of a material.
 * The generic type T defines the value type of the property (int, float, glm::vec3).
 */
template <typename T>
class MaterialProperty {
public:
	/**
	 * Defines whether to use the texture.
	 * When no texture is used, use the default value.
	 * Will be set to true automatically when loading a new Texture.
	 */
	bool use_texture;
	
	/**
	 * The default value of the property.
	 * Fallback to this value, if no texture can or should be used.
	 */
	T value_default;

	/**
	 * The texture for the property.
	 */
	Texture texture;

	/**
	 * Creates a new MaterialProperty.
	 */
	MaterialProperty();

	/**
	 * Loads a single texture with the following parameters:
	 * - the library path (the folder, where the texture is stored, relative to the executable e.g. '../textures/')
	 * - the name of the used image (e.g. 'tiles_marble')
	 * - the name of the property (e.g. 'albedo')
	 * - the file type (e.g. '.png')
	 * - the separator that separates the properties from the name of the image (e.g. '_')
	 * The specified example would evaluate to the image at the path '../textures/tiles_marble_albedo.png'
	 */
	void loadTexture(std::string library, std::string texture_name, std::string separator, std::string property_name, std::string fileType);

	/**
	 * Loads a single texture with the following parameters:
	 * - the name of the used image (e.g. 'tiles_marble')
	 * - the name of the property (e.g. 'albedo')
	 * - the file type (e.g. '.png')
	 * - the separator that separates the properties from the name of the image (e.g. '_')
	 * The specified example would evaluate to the image at the path '${DEFAULT_LIB_PATH}/tiles_marble_albedo.png'
	 */
	void loadTexture(std::string texture_name, std::string separator, std::string property_name, std::string fileType);
private:
	static const char period = '.';
};

/**
 * A collection of properties that define the characteristics of the corresponding SceneObject.
 */
class Material {
public:
	MaterialProperty<glm::vec3> albedo;
	MaterialProperty<glm::vec3> normal;
	MaterialProperty<float> roughness;
	MaterialProperty<float> metallic;
	MaterialProperty<float> ao;
	MaterialProperty<float> height;

	/**
	 * The scaling of height for parallax-occlusion and displacement-mapping.
	 */
	float height_scale;

	/**
	 * Creates a new Material with all properties initialized to default values.
	 */
	Material();

	/**
	 * Loads all existing textures with the following parameters:
	 * - the library path (the folder, where the texture is stored, relative to the executable e.g. '../textures/')
	 * - the name of the used image (e.g. 'tiles_marble')
	 * - the name of the property (e.g. 'albedo')
	 * - the file type (e.g. '.png')
	 * - the separator that separates the properties from the name of the image (e.g. '_')
	 * The specified example would evaluate to the image at the path '../textures/tiles_marble_albedo.png'
	 */
	void loadTextures(std::string library, std::string name, std::string separator, std::string fileType);

	/**
	 * Loads all existing textures with the following parameters:
	 * - the name of the used image (e.g. 'tiles_marble')
	 * - the name of the property (e.g. 'albedo')
	 * - the file type (e.g. '.png')
	 * - the separator that separates the properties from the name of the image (e.g. '_')
	 * The specified example would evaluate to the image at the path '../textures/tiles_marble_albedo.png'
	 */
	void loadTextures(std::string name, std::string separator, std::string fileType);

	/**
	 * Binds all textures to OpenGL texture units.
	 * The textures are located in the range of the specified beginning to the beginning + number of textures.
	 */
	void bindTextures(unsigned int textureUnitsBegin);
};