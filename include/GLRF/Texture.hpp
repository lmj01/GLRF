#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>

namespace GLRF {
	static std::string defaultLibrary = "./textures/";
	static std::string defaultRelativePath = "missingTexture.png";

	class Texture;
}

/**
 * @brief An image that can be bound to an OpenGL texture unit.
 * 
 */
class GLRF::Texture {
public:
	/**
	 * @brief Construct a new Texture object.
	 * 
	 * @param library the relative path to multiple textures
	 * @param relativePath the relative path to a single texture inside the library
	 * 
	 * Loads a texture from the specified library path and following relative path.
	 */
	Texture(std::string library, std::string relativePath);

	/**
	 * @brief Construct a new Texture object.
	 * 
	 * @param relativePath the relative path to a single texture inside the library
	 * 
	 * Loads a texture from the default library path and following relative path.
	 */
	Texture(std::string relativePath);

	/**
	 * @brief Construct a new Texture object.
	 * 
	 * Loads a default texture.
	 */
	Texture();

	/**
	 * @brief Loads a texture from a previously specified path.
	 * 
	 */
	void load();

	/**
	 * @brief Binds the current texture to an OpenGL texture unit.
	 * 
	 * @param textureUnit the texture unit to bind the texture to
	 */
	void bind(GLenum textureUnit);

	/**
	 * @brief Returns whether the texture was loaded into memory successfully.
	 * 
	 * @return true if the texture was loaded successfully
	 * @return false else
	 */
	bool isSuccessfullyLoaded();
private:
	GLuint ID;
	int width, height, nrChannels;
	unsigned char * data;
	std::string library, relativePath;
	bool successfullyLoaded = false;
	void create(std::string library, std::string relativePath);
};
