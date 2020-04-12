#include <GLRF/Texture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(std::string library, std::string relativePath) {
	create(library, relativePath);
}

Texture::Texture(std::string relativePath) {
	create(defaultLibrary, relativePath);
}

Texture::Texture() {
	create(defaultLibrary, defaultRelativePath);
}

void Texture::create(std::string library, std::string relativePath) {
	this->library = library;
	this->relativePath = relativePath;
	glGenTextures(1, &(this->ID));
	glBindTexture(GL_TEXTURE_2D, this->ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	load();
}

void Texture::load() {
	std::string fullPath_string = this->library + this->relativePath;
	const char * fullPath = (fullPath_string).data();
	this->data = stbi_load(fullPath, &(this->width), &(this->height), &(this->nrChannels), STBI_rgb);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->data);
		glGenerateMipmap(GL_TEXTURE_2D);
		this->successfullyLoaded = true;
	} else {
		std::cout << "Failed to load texture \"" << fullPath_string << "\"" << std::endl;
		this->successfullyLoaded = false;
	}

	stbi_image_free(this->data);
}

void Texture::bind(GLenum textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

bool Texture::isSuccessfullyLoaded() {
	return this->successfullyLoaded;
}
