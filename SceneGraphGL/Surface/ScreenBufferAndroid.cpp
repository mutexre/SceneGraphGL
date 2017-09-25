//
// Created by Sergey Vasilenko on 25.10.16.
//

#include "ScreenBufferAndroid.hpp"

using namespace SG::GL;

uvec2 ScreenBufferAndroid::fetchSize() const
{
    GLint w, h;

    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &w);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &h);

    return uvec2(w, h);
}

bool ScreenBufferAndroid::bindBuffer(GLuint renderbuffer)
{
    LOGI("ScreenBufferAndroid -> bindBuffer %u", renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    size = fetchSize();

    return true;
}
