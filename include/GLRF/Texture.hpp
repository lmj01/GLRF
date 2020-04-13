#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>

static std::string defaultLibrary = "./textures/";
static std::string defaultRelativePath = "missingTexture.png";

/**
 * An image that can be bound to an OpenGL texture unit.
 */
class Texture {
public:
	/**
	 * Loads a texture from the specified library path and following relative path.
	 */
	Texture(std::string library, std::string relativePath);

	/**
	 * Loads a texture from the default library path and following, specified relative path.
	 */
	Texture(std::string relativePath);

	/**
	 * Loads a default texture from the default library path and following relative path.
	 * Results in a texture for missing textures.
	 */
	Texture();

	/**
	 * Loads a texture from a previously specified path.
	 */
	void load();

	/**
	 * Binds the current texture to an OpenGL texture unit.
	 */
	void bind(GLenum textureUnit);

	/**
	 * Returns whether the texture was loaded into memory successfully.
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
