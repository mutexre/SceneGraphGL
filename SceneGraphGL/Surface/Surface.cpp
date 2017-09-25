//
//  Created by mutexre on 18/10/2016.
//  Copyright © 2016 Some Organization. All rights reserved.
//

#include <SceneGraphGL/SceneGraphGL.hpp>

using namespace SG::GL;

bool Surface::checkFramebufferCompleteness(GLenum target) const {
    auto status = glCheckFramebufferStatus(target);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGW("framebuffer is incomplete: 0x%x\n", status);
        return false;
    }
    
    return true;
}

Surface::Surface() {
    glGenFramebuffers(1, &framebuffer);
}

Surface::Surface(GLint framebuffer)
{
    this->framebuffer = framebuffer;
    external = true;
}

Surface::~Surface()
{
    if (!external)
        glDeleteFramebuffers(1, &framebuffer);
}

void Surface::attach(AttachmentPoint point,
                     unsigned index,
                     const shared_ptr<SG::Texture>& texture,
                     unsigned level,
                     unsigned cubeMapSide)
{
    auto glTexture = static_pointer_cast<GL::Texture>(texture);
    GLenum attachment = convertAttachmentPoint(point) + index;
    
    GLenum texTarget;
    if (texture->getType() == Texture::Type::cubemap)
        texTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubeMapSide;
    else
        texTarget = Texture::convertType(texture->getType());
    
#ifdef TEXTURE_DEBUG
    printf("attach texture %d to framebuffer %d\n", glTexture->getObjectId(), framebuffer);
#endif
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texTarget, glTexture->getObjectId(), level);
    
    checkFramebufferCompleteness(GL_FRAMEBUFFER);
}

void Surface::attach(AttachmentPoint point,
                     unsigned index,
                     GLuint renderbuffer)
{
    GLenum attachment = convertAttachmentPoint(point) + index;
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer);
    checkFramebufferCompleteness(GL_FRAMEBUFFER);
}

void Surface::attach(AttachmentPoint point,
                     unsigned index,
                     const shared_ptr<SG::Renderbuffer>& renderbuffer)
{
    auto glRenderbuffer = dynamic_pointer_cast<SG::GL::Renderbuffer>(renderbuffer);
    attach(point, index, glRenderbuffer->getObjectId());
}

void Surface::detach(AttachmentPoint point, unsigned index) {
    attach(point, index, GL_NONE);
}

void Surface::makeActive(Target target)
{
//    LOGI("Surface::makeActive: %d", framebuffer);
    glBindFramebuffer(convertTarget(target), framebuffer);
    
#ifdef NDEBUG
    bool isComplete = checkFramebufferCompleteness(convertTarget(target));
    if (!isComplete) {
        throw std::runtime_error(std::string("make active can not be performed on incomplete framebuffer: " + std::to_string(framebuffer)));
    }
#endif
}

void Surface::makeActive() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void Surface::read(unsigned x, unsigned y, unsigned w, unsigned h,
                   ColorComponents colorComponents,
                   PixelDataType pixelDataType,
                   void* data)
{
    auto glColorComponents = Texture::convertImageComponentsType(colorComponents);
    auto glPixelDataType = Texture::convertImageDataType(pixelDataType);
    glReadPixels(x, y, w, h, glColorComponents, glPixelDataType, data);
}

void Surface::invalidate(Target target)
{
#ifdef __APPLE__
#if TARGET_OS_IPHONE || TARGET_OS_SIMULATOR

    GLenum attachments[] = { GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT };
    glInvalidateFramebuffer(convertTarget(target), sizeof(attachments) / sizeof(GLenum), attachments);

#endif
#endif
}

void Surface::invalidate()
{
#ifdef __APPLE__
#if TARGET_OS_IPHONE || TARGET_OS_SIMULATOR

    GLenum attachments[] = { GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT };
    glInvalidateFramebuffer(GL_FRAMEBUFFER, sizeof(attachments) / sizeof(GLenum), attachments);

#endif
#endif
}

GLenum Surface::convertAttachmentPoint(AttachmentPoint attachment)
{
    switch (attachment)
    {
        case AttachmentPoint::color:
            return GL_COLOR_ATTACHMENT0;
    
        case AttachmentPoint::depth:
            return GL_DEPTH_ATTACHMENT;
        
        case AttachmentPoint::stencil:
            return GL_STENCIL_ATTACHMENT;

        default:
            throw "unknown attachment point type";
    }
    
    return GL_NONE;
}

GLenum Surface::convertTarget(Target target)
{
    switch (target)
    {
        case Target::read:
            return GL_READ_FRAMEBUFFER;
    
        case Target::draw:
            return GL_DRAW_FRAMEBUFFER;
        
        default:
            throw "unknown framebuffer target";
    }
    
    return GL_NONE;
}

void Surface::setObjectId(GLuint objectId)
{
    if (!external)
        glDeleteFramebuffers(1, &framebuffer);

    framebuffer = objectId;
    external = true;
}
