#pragma once
#include <iterator>
#include <map>
#include <variant>
#include <optional>
#include <glm/glm.hpp>

#include <GLRF/Texture.hpp>

namespace GLRF {
	template <typename T> class MaterialProperty;
	class Material;
}

/**
 * @brief A single property (like color or roughness) of a material.
 * 
 * @tparam T defines the value type of the property (int, float, glm::vec3)
 */
template <typename T>
class GLRF::MaterialProperty {
public:
	/**
	 * @brief The default value of the property.
	 * 
	 * Fallback to this value, if no texture can or should be used.
	 */
	T value_default;

	/**
	 * @brief The texture that defines the property.
	 * 
	 * Stores the image data that controls the property.
	 */
	std::optional<std::shared_ptr<Texture>> texture;

	/**
	 * @brief Construct a new MaterialProperty object.
	 * 
	 */
	MaterialProperty();

	/**
	 * @brief Loads a texture into memory that will be used for this property.
	 * 
	 * @param library the path, where the texture is stored (relative to the executable) e.g. '../textures/')
	 * @param texture_name the name of the used image (e.g. 'tiles_marble')
	 * @param separator separates the properties from the name of the image (e.g. '_')
	 * @param property_name the name of the property (e.g. 'albedo')
	 * @param fileType the type (e.g. '.png') of the file
	 * 
	 * The specified example would evaluate to the image at the path '../textures/tiles_marble_albedo.png'
	 */
	void loadTexture(std::string library, std::string texture_name, std::string separator, std::string property_name, std::string fileType);

	/**
	 * @brief Loads a texture into memory that will be used for this property.
	 * 
	 * @param texture_name the name of the used image (e.g. 'tiles_marble')
	 * @param separator separates the properties from the name of the image (e.g. '_')
	 * @param property_name the name of the property (e.g. 'albedo')
	 * @param fileType the type (e.g. '.png') of the file
	 * 
	 * The specified example would evaluate to the image at the path '${DEFAULT_LIB_PATH}/tiles_marble_albedo.png'
	 */
	void loadTexture(std::string texture_name, std::string separator, std::string property_name, std::string fileType);
private:
	static const char period = '.';
};

/**
 * @brief A collection of properties that define the characteristics of the corresponding objects inside the Scene.
 * 
 */
class GLRF::Material {
public:
	MaterialProperty<glm::vec3> albedo;
	MaterialProperty<glm::vec3> normal;
	MaterialProperty<float> roughness;
	MaterialProperty<float> metallic;
	MaterialProperty<float> ao;
	MaterialProperty<float> height;

	/**
	 * @brief A scaling factor for the height property that is used for parallax-occlusion- and displacement-mapping.
	 * 
	 */
	float height_scale;

	/**
	 * @brief Construct a new Material object.
	 * 
	 * Creates a new Material with all properties initialized to default values.
	 */
	Material();

	/**
	 * @brief Loads all textures into memory that will be used for this material.
	 * 
	 * @param library the path, where the texture is stored (relative to the executable) e.g. '../textures/')
	 * @param texture_name the name of the used image (e.g. 'tiles_marble')
	 * @param separator separates the properties from the name of the image (e.g. '_')
	 * @param property_name the name of the property (e.g. 'albedo')
	 * @param fileType the type (e.g. '.png') of the file
	 * 
	 * The specified example would evaluate to the image at the path '${DEFAULT_LIB_PATH}/tiles_marble_albedo.png'
	 */
	void loadTextures(std::string library, std::string name, std::string separator, std::string fileType);

	/**
	 * @brief Loads all textures into memory that will be used for this material.
	 * 
	 * @param texture_name the name of the used image (e.g. 'tiles_marble')
	 * @param separator separates the properties from the name of the image (e.g. '_')
	 * @param property_name the name of the property (e.g. 'albedo')
	 * @param fileType the type (e.g. '.png') of the file
	 * 
	 * The specified example would evaluate to the image at the path '${DEFAULT_LIB_PATH}/tiles_marble_albedo.png'
	 */
	void loadTextures(std::string name, std::string separator, std::string fileType);
	
	/**
	 * @brief Binds all textures to OpenGL texture units.
	 * 
	 * @param textureUnitsBegin the first texture unit that is currently free
	 */
	void bindTextures(unsigned int textureUnitsBegin);
};