//
//  Created by mutexre on 21/10/2016.
//  Copyright © 2016 Some Organization. All rights reserved.
//

#include <SceneGraphGL/SceneGraphGL.hpp>

using namespace SG::GL;

Renderbuffer::Renderbuffer() {
    glGenRenderbuffers(1, &renderbuffer);
}

Renderbuffer::~Renderbuffer() {
    glDeleteRenderbuffers(1, &renderbuffer);
}

void Renderbuffer::makeActive() {
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
}

GLuint Renderbuffer::getObjectId() const {
    return renderbuffer;
}

void Renderbuffer::allocStorage(int w, int h, PixelFormat format, int samples)
{
    GLint currentRenderbuffer;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &currentRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    
    GLenum glFormat = Texture::convertPixelFormat(format);
    if (samples < 2)
        glRenderbufferStorage(GL_RENDERBUFFER, glFormat, w, h);
    else
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, glFormat, w, h);

    glBindRenderbuffer(GL_RENDERBUFFER, currentRenderbuffer);
}
