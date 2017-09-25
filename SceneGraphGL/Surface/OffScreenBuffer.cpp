//
//  Created by mutexre on 21/10/2016.
//  Copyright © 2016 Some Organization. All rights reserved.
//

#include <SceneGraphGL/SceneGraphGL.hpp>

using namespace SG::GL;

void OffScreenBuffer::allocStorage(int w, int h, PixelFormat format, int samples)
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
