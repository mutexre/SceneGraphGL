//
//  Created by mutexre on 10/01/16.
//  Copyright © 2016 mutexre. All rights reserved.
//

#pragma once

namespace SG::GL
{
    void checkGlErrors();
    void checkGlErrors(const char*);
    void checkGlErrors(const char* file, int line);
    
    GLint getCurrentVertexArrayBinding();
    GLint getCurrentBufferBinding(GLenum target);
    GLint getCurrentProgram();
    void printGlBindings();

    GLbitfield convertBuffersToClear(int buffersToClear);

    GLenum convertBlendFunc(Settings::Blend::Func);
    Settings::Blend::Func convertBlendFunc(GLenum);

    GLenum convertBlendEquation(Settings::Blend::Equation);
    Settings::Blend::Equation convertBlendEquation(GLenum);

    GLenum convertCullFaceMode(Settings::CullFace::Mode);
    Settings::CullFace::Mode convertCullFaceMode(GLenum);
}
