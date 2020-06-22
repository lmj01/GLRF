#include <GLRF/FrameBuffer.hpp>

using namespace GLRF;

FrameBuffer::FrameBuffer(FrameBufferConfiguration & config, ScreenResolution & screen_res)
{
    glGenFramebuffers(1, &(this->ID));
    glBindFramebuffer(GL_FRAMEBUFFER, this->ID);

    this->texture_color_buffer_IDs.reserve(config.num_color_buffers);
    for (GLuint i = 0; i < texture_color_buffer_IDs.capacity(); ++i)
    {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, config.color_profile, screen_res.width, screen_res.height, 0, config.color_type, config.data_type, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureID, 0);
        this->texture_color_buffer_IDs.push_back(textureID);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(config.num_color_buffers, attachments);

    if (config.use_depth_buffer)
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
    for (GLuint i = 0; i < this->texture_color_buffer_IDs.size(); ++i)
    {
        glDeleteTextures(1, &(this->texture_color_buffer_IDs[i]));
    }
    this->texture_color_buffer_IDs.clear();
    this->texture_color_buffer_IDs.shrink_to_fit();
    if (this->RBO.has_value()) glDeleteRenderbuffers(1, &(this->RBO.value()));
}

void FrameBuffer::use()
{
    GLint id;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &id);
    if (id != this->ID)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, this->ID);
    }
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &id);
}

GLuint FrameBuffer::getID()
{
    return this->ID;
}

GLuint FrameBuffer::getColorBufferID(size_t idx)
{
    return this->texture_color_buffer_IDs[idx];
}

void FrameBuffer::setDebugName(const std::string name)
{
    this->debug_name = name;
}

std::string FrameBuffer::getDebugName()
{
    return this->debug_name;
}