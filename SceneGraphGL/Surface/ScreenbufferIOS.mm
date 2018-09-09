//
//  Created by mutexre on 19/10/2016.
//  Copyright © 2016 Some Organization. All rights reserved.
//

#if TARGET_OS_IPHONE || TARGET_OS_SIMULATOR

#include <SceneGraphGL/SceneGraphGL.hpp>

using namespace SG::GL;
using namespace glm;

ScreenbufferIOS::ScreenbufferIOS() {
    format = PixelFormat::rgba8;
}

uvec2 ScreenbufferIOS::fetchSize() const
{
    GLint w, h;
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &w);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &h);
    
    return uvec2(w, h);
}

bool ScreenbufferIOS::bindStorage(EAGLContext* context, id<EAGLDrawable> drawable)
{
    GLint currentRenderbuffer;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &currentRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    
    if (![context renderbufferStorage:GL_RENDERBUFFER fromDrawable:drawable]) {
        printf("failed to bind CAEAGLLayer storage to renderbuffer");
        glBindRenderbuffer(GL_RENDERBUFFER, currentRenderbuffer);
        return false;
    }

    size = fetchSize();
    
    glBindRenderbuffer(GL_RENDERBUFFER, currentRenderbuffer);
    
    return true;
}

void ScreenbufferIOS::present(EAGLContext* context)
{
    makeActive();
    [context presentRenderbuffer:GL_RENDERBUFFER]
}

#endif
