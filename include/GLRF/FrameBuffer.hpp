#pragma once
#include <glad/glad.h>
#include <stdexcept>
#include <optional>
#include <iostream>

namespace GLRF {
    struct ScreenResolution;
    struct FrameBufferConfiguration;
    class FrameBuffer;
}

/**
 * @brief A screen resolution consisting of width and height parameters.
 * 
 */
struct GLRF::ScreenResolution {
	unsigned int width = 800;
	unsigned int height = 600;
	ScreenResolution(unsigned int width, unsigned int height) {
		this->width = width;
		this->height = height;
	}
	ScreenResolution() {
		ScreenResolution(800, 600);
	}
};

struct GLRF::FrameBufferConfiguration
{
    GLenum color_profile = GL_RGB;
    GLenum color_type = GL_RGB;
    GLenum data_type = GL_UNSIGNED_BYTE;
    bool use_depth_buffer = true;
};


class GLRF::FrameBuffer
{
public:
    FrameBuffer(FrameBufferConfiguration & config, ScreenResolution & screen_res);
    ~FrameBuffer();

    void use();
    GLuint getID();
    GLuint getColorBufferID();
private:
    GLuint ID;
    GLuint texture_color_buffer_ID;
    std::optional<GLuint> RBO = std::nullopt;
};
