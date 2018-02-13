//
//  Created by mutexre on 13/10/2016.
//  Copyright © 2016 Some Organization. All rights reserved.
//

#include <SceneGraphGL/SceneGraphGL.hpp>

using namespace SG::GL;

Context::Context(OS os, API api, int majorVersion, int minorVersion)
    : SG::Context(os)
{
    this->api = api;
    
    version[0] = majorVersion;
    version[1] = minorVersion;
    
    if (isCoreProfileOrES3())
        fetchMaxMSAASamples();
}

API Context::getAPI() const {
    return api;
}

int Context::getVersion(int i) const {
    return version[i];
}

bool Context::isCoreProfileOrES3() const {
    return (api == API::CoreProfile || (api == API::ES && version[0] >= 3));
}

void Context::fetchMaxMSAASamples()
{
    GLint maxSamples = 1;
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
    maxMSAASamples = maxSamples;
}

bool Context::isWrapRSupported() const {
    return isCoreProfileOrES3();
}

#pragma mark - Programs

shared_ptr<SG::Program>
Context::loadProgram(const string& vertexShaderName,
                     const string& fragmentShaderName)
{
    LOGI("loadProgram: %s, %s\n", vertexShaderName.c_str(), fragmentShaderName.c_str());
    
    auto programSrc = readProgramSource(vertexShaderName, fragmentShaderName);
    if (!programSrc)
        return nullptr;

    auto program = make_shared<SG::GL::Program>(programSrc.value);
    LOGI("program loaded: %s, %s\n", vertexShaderName.c_str(), fragmentShaderName.c_str());
    
    return program;
}

shared_ptr<SG::Program> Context::cloneProgram(const char* name)
{
    const auto& parent = *(static_pointer_cast<SG::GL::Program>(programs[name]).get());
    Program* clone = new Program(parent);
    return shared_ptr<Program>(clone);
}

#pragma mark - Factory Methods

shared_ptr<SG::Scene> Context::createScene()
{
    auto scene = make_shared<GL::Scene>(shared_from_this());
    scene->setContext(shared_from_this());
    return scene;
}

shared_ptr<SG::Node> Context::createNode()
{
    auto node = make_shared<GL::Node>();
    node->setContext(shared_from_this());
    return node;
}

shared_ptr<SG::MeshNode> Context::createMeshNode()
{
    auto node = make_shared<GL::MeshNode>();
    node->setContext(shared_from_this());
    return node;
}

shared_ptr<SG::Light> Context::createLight()
{
    auto light = make_shared<GL::Light>();
    light->setContext(shared_from_this());
    return light;
}

shared_ptr<SG::Camera> Context::createCamera()
{
    auto camera = make_shared<GL::Camera>();
    camera->setContext(shared_from_this());
    return camera;
}

shared_ptr<SG::Mesh> Context::createMesh()
{
    auto mesh = make_shared<GL::Mesh>();
    mesh->setContext(shared_from_this());
    return mesh;
}

shared_ptr<SG::Texture> Context::createTexture()
{
    auto texture = make_shared<GL::Texture>();
    texture->setContext(shared_from_this());
    return texture;
}

shared_ptr<SG::Surface> Context::createSurface()
{
    auto surface = make_shared<GL::Surface>();
    surface->setContext(shared_from_this());
    return surface;
}

shared_ptr<SG::Renderbuffer> Context::createRenderbuffer()
{
    auto renderbuffer = make_shared<GL::Renderbuffer>();
    renderbuffer->setContext(shared_from_this());
    return renderbuffer;
}

shared_ptr<SG::OffScreenBuffer> Context::createOffScreenBuffer()
{
    auto screenbuffer = make_shared<GL::OffScreenBuffer>();
    screenbuffer->setContext(shared_from_this());
    return screenbuffer;
}

#pragma mark - Settings Fetch

void Context::resetErrorFlags()
{
    while (glGetError() != GL_NO_ERROR)
    {}
}

SG::Settings Context::fetchCurrentSettings()
{
    Settings s;
    
    resetErrorFlags();
    
    s.depthTest = glIsEnabled(GL_DEPTH_TEST);

#ifdef GL_DEPTH_CLAMP
    s.depthClamp = glIsEnabled(GL_DEPTH_CLAMP);
#endif

    GLboolean writeDepth;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &writeDepth);
    s.writeDepth = writeDepth;

#ifdef GL_TEXTURE_CUBE_MAP_SEAMLESS
    s.seamlessCubemaps = glIsEnabled(GL_TEXTURE_CUBE_MAP_SEAMLESS);
#endif

#ifdef GL_POINT_SMOOTH
    s.smoothPoints = glIsEnabled(GL_POINT_SMOOTH);
#endif

    vec4 viewport;
    glGetIntegerv(GL_VIEWPORT, (GLint*)&viewport);
    s.viewport = viewport;

    s.cullFace = fetchCullFaceSettings();
    s.blend = fetchBlendSettings();
    s.scissor = fetchScissorSettings();
    
    return s;
}

SG::Settings::CullFace Context::fetchCullFaceSettings()
{
    Settings::CullFace x;
    GLint mode;
    
    bool enabled = glIsEnabled(GL_CULL_FACE);
    if (glGetError() == GL_NO_ERROR) {
        x.enabled = enabled;
    }
    
    glGetIntegerv(GL_CULL_FACE_MODE, &mode);
    if (glGetError() == GL_NO_ERROR) {
        x.mode = convertCullFaceMode(mode);
    }
    
    return x;
}

SG::Settings::Blend Context::fetchBlendSettings()
{
    Settings::Blend blend;
    GLint value;
    
    bool enabled = glIsEnabled(GL_BLEND);
    if (glGetError() == GL_NO_ERROR) {
        blend.enabled = enabled;
    }
    
    glGetIntegerv(GL_BLEND_SRC_RGB, &value);
    if (glGetError() == GL_NO_ERROR) {
        blend.func.srcRGB = convertBlendFunc(GLenum(value));
    }
    
    glGetIntegerv(GL_BLEND_DST_RGB, &value);
    if (glGetError() == GL_NO_ERROR) {
        blend.func.dstRGB = convertBlendFunc(GLenum(value));
    }
    
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &value);
    if (glGetError() == GL_NO_ERROR) {
        blend.func.srcAlpha = convertBlendFunc(GLenum(value));
    }
    
    glGetIntegerv(GL_BLEND_DST_ALPHA, &value);
    if (glGetError() == GL_NO_ERROR) {
        blend.func.dstAlpha = convertBlendFunc(GLenum(value));
    }
    
    glGetIntegerv(GL_BLEND_EQUATION_RGB, &value);
    if (glGetError() == GL_NO_ERROR) {
        blend.equation.rgb = convertBlendEquation(value);
    }
    
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &value);
    if (glGetError() == GL_NO_ERROR) {
        blend.equation.alpha = convertBlendEquation(value);
    }
    
    vec4 color;
    glGetFloatv(GL_BLEND_COLOR, (GLfloat*)&color);
    if (glGetError() == GL_NO_ERROR) {
        blend.color = color;
    }
    
    return blend;
}

SG::Settings::Scissor Context::fetchScissorSettings()
{
    Settings::Scissor s;
    uvec4 box;
    
    bool enabled = glIsEnabled(GL_SCISSOR_TEST);
    if (glGetError() == GL_NO_ERROR) {
        s.enabled = enabled;
    }

    glGetIntegerv(GL_SCISSOR_BOX, (GLint*)&box);
    if (glGetError() == GL_NO_ERROR) {
        s.rect = box;
    }
    
    return s;
}

#pragma mark - Settings Apply

bool Context::applyDepthTestSettings(bool value)
{
    if (value)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    
    return true;
}

bool Context::applyDepthClampSettings(bool value)
{
#ifdef GL_DEPTH_CLAMP
    if (value)
        glEnable(GL_DEPTH_CLAMP);
    else
        glDisable(GL_DEPTH_CLAMP);
#endif
    
    return true;
}

bool Context::applyWriteDepthSettings(bool value) {
    glDepthMask(value ? GL_TRUE : GL_FALSE);
    return true;
}

bool Context::applySeamlessCubemapsSettings(bool value)
{
#ifdef GL_TEXTURE_CUBE_MAP_SEAMLESS
    if (value)
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    else
        glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
#endif

    return true;
}

bool Context::applySmoothPointsSettings(bool value)
{
#ifdef GL_POINT_SMOOTH
    if (value)
        glEnable(GL_POINT_SMOOTH);
    else
        glDisable(GL_POINT_SMOOTH);
#endif

    return true;
}

bool Context::applyCullFaceSettings(const Settings::CullFace& cullFace)
{
    if (settings.cullFace)
    {
        if (cullFace.enabled && settings.cullFace->enabled != cullFace.enabled)
        {
            settings.cullFace->enabled = cullFace.enabled;
            
            if (cullFace.enabled.value)
                glEnable(GL_CULL_FACE);
            else
                glDisable(GL_CULL_FACE);
        }
    
        if (cullFace.mode && settings.cullFace->mode != cullFace.mode)
        {
            settings.cullFace->mode = cullFace.mode;
            glCullFace(convertCullFaceMode(cullFace.mode.value));
        }
    }
    else
    {
        settings.cullFace = cullFace;
    
        if (cullFace.enabled)
        {
            if (cullFace.enabled.value)
                glEnable(GL_CULL_FACE);
            else
                glDisable(GL_CULL_FACE);
        }
    
        if (cullFace.mode)
            glCullFace(convertCullFaceMode(cullFace.mode.value));
    }
    
    return true;
}

bool Context::applyBlendSettings(const Settings::Blend& blend)
{
    if (settings.blend)
    {
        if (blend.enabled && settings.blend->enabled != blend.enabled)
        {
            settings.blend->enabled = blend.enabled;
            
            if (blend.enabled.value)
                glEnable(GL_BLEND);
            else
                glDisable(GL_BLEND);
        }
        
        if ((blend.func.srcRGB && settings.blend->func.srcRGB != blend.func.srcRGB) ||
            (blend.func.srcAlpha && settings.blend->func.srcAlpha != blend.func.srcAlpha) ||
            (blend.func.dstRGB && settings.blend->func.dstRGB != blend.func.dstRGB) ||
            (blend.func.dstAlpha && settings.blend->func.dstAlpha != blend.func.dstAlpha))
        {
            settings.blend->func.srcRGB = blend.func.srcRGB;
            settings.blend->func.dstRGB = blend.func.dstRGB;
            
            if (!blend.separate)
            {
                glBlendFunc(convertBlendFunc(blend.func.srcRGB.value),
                            convertBlendFunc(blend.func.dstRGB.value));

                settings.blend->func.srcAlpha = blend.func.srcRGB;
                settings.blend->func.dstAlpha = blend.func.dstRGB;
            }
            else
            {
                glBlendFuncSeparate(convertBlendFunc(blend.func.srcRGB.value),
                                    convertBlendFunc(blend.func.dstRGB.value),
                                    convertBlendFunc(blend.func.srcAlpha.value),
                                    convertBlendFunc(blend.func.dstAlpha.value));

                settings.blend->func.srcAlpha = blend.func.srcAlpha;
                settings.blend->func.dstAlpha = blend.func.dstAlpha;
            }
        }

        if ((blend.equation.rgb && settings.blend->equation.rgb != blend.equation.rgb) ||
            (blend.equation.alpha && settings.blend->equation.alpha != blend.equation.alpha))
        {
            settings.blend->equation.rgb = blend.equation.rgb;
            
            if (!blend.separate)
            {
                glBlendEquation(convertBlendEquation(blend.equation.rgb.value));
                settings.blend->equation.alpha = blend.equation.rgb;
            }
            else
            {
                glBlendEquationSeparate(convertBlendEquation(blend.equation.rgb.value),
                                        convertBlendEquation(blend.equation.alpha.value));
                
                settings.blend->equation.alpha = blend.equation.alpha;
            }
        }
        
        if (blend.color && settings.blend->color != blend.color)
        {
            glBlendColor(blend.color->r, blend.color->g, blend.color->b, blend.color->a);
            settings.blend->color = blend.color;
        }
    }
    else
    {
        settings.blend = blend;
        
        if (blend.enabled)
        {
            if (blend.enabled.value)
                glEnable(GL_BLEND);
            else
                glDisable(GL_BLEND);
        }
        
        if (blend.func.srcRGB || blend.func.srcAlpha || blend.func.dstRGB || blend.func.dstAlpha)
        {
            if (!blend.separate) {
                glBlendFunc(convertBlendFunc(blend.func.srcRGB.value),
                            convertBlendFunc(blend.func.dstRGB.value));
            }
            else
            {
                glBlendFuncSeparate(convertBlendFunc(blend.func.srcRGB.value),
                                    convertBlendFunc(blend.func.dstRGB.value),
                                    convertBlendFunc(blend.func.srcAlpha.value),
                                    convertBlendFunc(blend.func.dstAlpha.value));
            }
        }

        if (blend.equation.rgb || blend.equation.alpha)
        {
            if (!blend.separate) {
                glBlendEquation(convertBlendEquation(blend.equation.rgb.value));
            }
            else
            {
                glBlendEquationSeparate(convertBlendEquation(blend.equation.rgb.value),
                                        convertBlendEquation(blend.equation.alpha.value));
            }
        }
        
        if (blend.color)
            glBlendColor(blend.color->r, blend.color->g, blend.color->b, blend.color->a);
    }
    
    return true;
}

bool Context::applyViewportSettings(ivec4 rect)
{
    glViewport(rect.x, rect.y, rect.z, rect.w);
    return true;
}

bool Context::applyScissorSettings(const Settings::Scissor& scissor)
{
    if (settings.scissor)
    {
        if (scissor.enabled && settings.scissor->enabled != scissor.enabled)
        {
            settings.scissor->enabled = scissor.enabled;
            
            if (scissor.enabled.value)
                glEnable(GL_SCISSOR_TEST);
            else
                glDisable(GL_SCISSOR_TEST);
        }
        
        if (scissor.rect && settings.scissor->rect != scissor.rect) {
            settings.scissor->rect = scissor.rect;
            glScissor(scissor.rect->x, scissor.rect->y, scissor.rect->z, scissor.rect->w);
        }
    }
    else
    {
        settings.scissor = scissor;
        
        if (scissor.enabled)
        {
            if (scissor.enabled.value)
                glEnable(GL_SCISSOR_TEST);
            else
                glDisable(GL_SCISSOR_TEST);
        }
        
        if (scissor.rect)
            glScissor(scissor.rect->x, scissor.rect->y, scissor.rect->z, scissor.rect->w);
    }
    
    return true;
}

bool Context::applyColorMaskSettings(const Settings::ColorMask& mask)
{
    glColorMask(mask.r, mask.g, mask.b, mask.a);
    return true;
}
