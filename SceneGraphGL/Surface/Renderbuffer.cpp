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
