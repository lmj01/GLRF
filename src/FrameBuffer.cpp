#include <GLRF/FrameBuffer.hpp>

using namespace GLRF;

FrameBuffer::FrameBuffer(FrameBufferConfiguration & config, ScreenResolution & screen_res)
{
    glGenFramebuffers(1, &(this->ID));
    glBindFramebuffer(GL_FRAMEBUFFER, this->ID);

    glGenTextures(1, &(this->texture_color_buffer_ID));
    glBindTexture(GL_TEXTURE_2D, this->texture_color_buffer_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, config.color_profile, screen_res.width, screen_res.height, 0, config.color_type, config.data_type, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture_color_buffer_ID, 0);

    if (config.use_render_buffer)
    {
        GLuint RBO;
        glGenRenderbuffers(1, &RBO);
        this->RBO = RBO;
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_res.width, screen_res.height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "ERROR::cannot initialize framebuffer" << std::endl;
        throw std::runtime_error("cannot initialize framebuffer");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &(this->ID));
    glDeleteTextures(1, &(this->texture_color_buffer_ID));
    if (this->RBO.has_value()) glDeleteRenderbuffers(1, &(this->RBO.value()));
}

void FrameBuffer::use()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->ID);
}

GLuint FrameBuffer::getID()
{
    return this->ID;
}

GLuint FrameBuffer::getColorBufferID()
{
    return this->texture_color_buffer_ID;
}