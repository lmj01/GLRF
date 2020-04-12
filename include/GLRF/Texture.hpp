#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>

static const char * defaultLibrary = "../textures/";
static const char * defaultRelativePath = "missingTexture.png";

class Texture {
public:
	static void setLibraryPath(std::string libraryPath);
	Texture(std::string library, std::string relativePath);
	Texture(std::string relativePath);
	Texture();
	void load();
	void bind(GLenum textureUnit);
	bool isSuccessfullyLoaded();
private:
	static std::string library;
	GLuint ID;
	int width, height, nrChannels;
	unsigned char * data;
	std::string relativePath;
	bool successfullyLoaded = false;
	void create(std::string library, std::string relativePath);
};
