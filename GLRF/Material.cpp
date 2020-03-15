#include "Material.hpp"

void Material::loadTextures(std::string name, std::string separator, std::string fileType)
{
	this->textureAlbedo = Texture(name + separator + "albedo." + fileType);
	this->useTextureAlbedo = this->textureAlbedo.isSuccessfullyLoaded();

	this->textureNormal = Texture(name + separator + "normal." + fileType);
	this->useTextureNormal = this->textureNormal.isSuccessfullyLoaded();

	this->textureRoughness = Texture(name + separator + "roughness." + fileType);
	this->useTextureRoughness = this->textureRoughness.isSuccessfullyLoaded();

	this->textureMetallic = Texture(name + separator + "metallic." + fileType);
	this->useTextureMetallic = this->textureMetallic.isSuccessfullyLoaded();

	this->textureAo = Texture(name + separator + "ao." + fileType);
	this->useTextureAo = this->textureAo.isSuccessfullyLoaded();

	this->textureHeight = Texture(name + separator + "height." + fileType);
	this->useTextureHeight = this->textureHeight.isSuccessfullyLoaded();
}

void Material::bindTextures(unsigned int textureUnitsBegin)
{
	this->textureAlbedo.bind(GL_TEXTURE0 + textureUnitsBegin);
	this->textureNormal.bind(GL_TEXTURE0 + textureUnitsBegin + 1);
	this->textureRoughness.bind(GL_TEXTURE0 + textureUnitsBegin + 2);
	this->textureMetallic.bind(GL_TEXTURE0 + textureUnitsBegin + 3);
	this->textureAo.bind(GL_TEXTURE0 + textureUnitsBegin + 4);
	this->textureHeight.bind(GL_TEXTURE0 + textureUnitsBegin + 5);
}
