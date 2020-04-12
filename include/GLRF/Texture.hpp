#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>

static std::string defaultLibrary = "./textures/";
static std::string defaultRelativePath = "missingTexture.png";

class Texture {
public:
	Texture(std::string library, std::string relativePath);
	Texture(std::string relativePath);
	Texture();
	void load();
	void bind(GLenum textureUnit);
	bool isSuccessfullyLoaded();
private:
	GLuint ID;
	int width, height, nrChannels;
	unsigned char * data;
	std::string library, relativePath;
	bool successfullyLoaded = false;
	void create(std::string library, std::string relativePath);
};
