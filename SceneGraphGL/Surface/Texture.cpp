//
//  Created by mutexre on 08/01/16.
//  Copyright © 2016 mutexre. All rights reserved.
//

#include <SceneGraphGL/SceneGraphGL.hpp>

using namespace SG::GL;
using namespace std;
using namespace glm;

Texture::Texture() {
    glGenTextures(1, &texture);
}

Texture::Texture(Type type, PixelFormat format)
    : SG::Texture(type, format)
{}

Texture::~Texture() {
    glDeleteTextures(1, &texture);
}

void Texture::setParameters()
{
    GLenum target = convertType(type);
    glBindTexture(target, texture);
    
    glTexParameterf(target, GL_TEXTURE_MIN_FILTER, convertMinFilter(filter.min));
    glTexParameterf(target, GL_TEXTURE_MAG_FILTER, convertMagFilter(filter.mag));

#if 0
    glTexParameterf(target, GL_TEXTURE_MIN_LOD, lod.min);
    glTexParameterf(target, GL_TEXTURE_MAX_LOD, lod.max);
    glTexParameterf(target, GL_TEXTURE_BASE_LEVEL, lod.baseLevel);
    glTexParameterf(target, GL_TEXTURE_MAX_LEVEL, lod.maxLevel);
#endif

    glTexParameterf(target, GL_TEXTURE_WRAP_S, convertWrap(wrap.s));
    glTexParameterf(target, GL_TEXTURE_WRAP_T, convertWrap(wrap.t));
    
    auto glContext = static_cast<GL::Context*>(context.get());
    if (glContext->isWrapRSupported())
        glTexParameterf(target, GL_TEXTURE_WRAP_R, convertWrap(wrap.r));

#ifdef GL_TEXTURE_BORDER_COLOR
    glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, (const GLfloat*)&borderColor);
#endif
}

void Texture::init(Texture::Type type, PixelFormat format)
{
    setType(type);
    setFormat(format);
    setParameters();
}

void Texture::setImages(const vector<shared_ptr<Image>>& images)
{
//    init();
    
    switch (type)
    {
        case Type::t1d:
        case Type::t2d:
            for (unsigned level = 0; level < images.size(); level++)
                setImage(level, images[level]);
        break;

        default:
            throw runtime_error("invalid texture type");
        break;
    }
}

void Texture::allocStorage(unsigned width, unsigned height, unsigned depth,
                           unsigned level, unsigned layer)
{
    ColorComponents components;
    PixelDataType dataType;
    getPixelFormatComponentsAndDataType(format, components, dataType);
    
    setImage(nullptr, width, height, depth, components, dataType);
    //glTexStorage2D(GL_TEXTURE_2D, 1, <#GLenum internalformat#>, <#GLsizei width#>, <#GLsizei height#>)
}

void Texture::allocImmutableStorage(unsigned width, unsigned height, unsigned depth,
                                    unsigned levelsCount, unsigned layersCount)
{
#if SGGL_TEX_STORAGE_SUPPORTED

    GLint internalFormat = convertPixelFormat(format);
    
    switch (type)
    {
#ifdef GL_TEXTURE_1D
        case Type::t1d:
            glTexStorage1D(GL_TEXTURE_1D,
                           levelsCount,
                           internalFormat,
                           width);
        break;
#endif

        case Type::t2d:
            glTexStorage2D(GL_TEXTURE_2D,
                           levelsCount,
                           internalFormat,
                           width, height);
        break;

#ifdef GL_TEXTURE_3D
        case Type::t3d:
            glTexStorage3D(GL_TEXTURE_3D,
                           levelsCount,
                           internalFormat,
                           width, height, depth);
        break;
#endif
        
        case Type::cubemap:
        break;

        default: break;
    }

#endif
}

void Texture::setImage(const void* data,
                       unsigned width, unsigned height, unsigned depth,
                       ColorComponents imageColorComponents,
                       PixelDataType imagePixelDataType,
                       unsigned level,
                       unsigned layer)
{
    GLint internalFormat;
    
    auto glContext = static_cast<GL::Context*>(context.get());
    if (glContext->isCoreProfileOrES3())
        internalFormat = convertPixelFormat(format);
    else
        internalFormat = convertPixelFormatES2(format);
    
    auto colorComponents = convertImageComponentsType(imageColorComponents);
    auto pixelDataType = convertImageDataType(imagePixelDataType);
    
    switch (type)
    {
#ifdef GL_TEXTURE_1D
        case Type::t1d:
            glTexImage1D(GL_TEXTURE_1D,
                         level,
                         internalFormat,
                         width,
                         0,
                         colorComponents,
                         pixelDataType,
                         data);
        break;
#endif

        case Type::t2d:
            glTexImage2D(GL_TEXTURE_2D,
                         level,
                         internalFormat,
                         width, height,
                         0,
                         colorComponents,
                         pixelDataType,
                         data);
        break;

#ifdef GL_TEXTURE_3D
        case Type::t3d:
            glTexImage3D(GL_TEXTURE_3D,
                         level,
                         internalFormat,
                         width, height, depth,
                         0,
                         colorComponents,
                         pixelDataType,
                         data);
        break;
#endif
        
        case Type::cubemap:
        break;

        default: break;
    }
}

void Texture::setSubImage(const void* data,
                          unsigned x, unsigned y, unsigned z,
                          unsigned width, unsigned height, unsigned depth,
                          ColorComponents imageColorComponents,
                          PixelDataType imagePixelDataType,
                          unsigned level,
                          unsigned layer)
{
    switch (type)
    {
#ifdef GL_TEXTURE_1D
        case Type::t1d:
            glTexSubImage1D(GL_TEXTURE_1D,
                            level,
                            x,
                            width,
                            convertImageComponentsType(imageColorComponents),
                            convertImageDataType(imagePixelDataType),
                            data);
        break;
#endif

        case Type::t2d:
            glTexSubImage2D(GL_TEXTURE_2D,
                            level,
                            x, y,
                            width, height,
                            convertImageComponentsType(imageColorComponents),
                            convertImageDataType(imagePixelDataType),
                            data);
        break;

#ifdef GL_TEXTURE_3D
        case Type::t3d:
            glTexSubImage3D(GL_TEXTURE_3D,
                            level,
                            x, y, z,
                            width, height, depth,
                            convertImageComponentsType(imageColorComponents),
                            convertImageDataType(imagePixelDataType),
                            data);
        break;
#endif
        
        case Type::cubemap:
        break;

        default: break;
    }
}

void Texture::setImage(unsigned level, const shared_ptr<Image>& image)
{
    setImage(image->getData(),
             image->getW(),
             image->getH(),
             0,
             image->getColorComponents(),
             image->getPixelDataType(),
             level);
}

void Texture::setImages(const vector<shared_ptr<ImageStack>>& images)
{
    GLenum target = convertType(type);
    glDeleteTextures(1, &texture);
    glGenTextures(1, &texture);
    glBindTexture(target, texture);

    switch (type) {
        case Type::cubemap:
            for (unsigned level = 0; level < images.size(); level++) {
                auto image3d = images[level];
                for (unsigned side = 0; side < 6; side++)
                    setCubemapImage(side, level, image3d->operator[](side));
            }
        break;

        default:
            throw runtime_error("invalid texture type");
        break;
    }

    glTexParameterf(target, GL_TEXTURE_MIN_FILTER, convertMinFilter(filter.min));
    glTexParameterf(target, GL_TEXTURE_MAG_FILTER, convertMagFilter(filter.mag));

#if 1
    glTexParameterf(target, GL_TEXTURE_MIN_LOD, lod.min);
    glTexParameterf(target, GL_TEXTURE_MAX_LOD, lod.max);
    glTexParameterf(target, GL_TEXTURE_BASE_LEVEL, lod.baseLevel);
    glTexParameterf(target, GL_TEXTURE_MAX_LEVEL, lod.maxLevel);
#endif
    
    glTexParameterf(target, GL_TEXTURE_WRAP_S, convertWrap(wrap.s));
    glTexParameterf(target, GL_TEXTURE_WRAP_T, convertWrap(wrap.t));
    glTexParameterf(target, GL_TEXTURE_WRAP_R, convertWrap(wrap.r));

#ifdef GL_TEXTURE_BORDER_COLOR
    glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, (const GLfloat*)&borderColor);
#endif
}

void Texture::setCubemapImage(unsigned side, unsigned level, const shared_ptr<Image>& image)
{
    auto internalFormat = convertPixelFormat(format);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side,
                 level,
                 internalFormat,
                 image->getW(), image->getH(),
                 0,
                 convertImageComponentsType(image->getColorComponents()),
                 convertImageDataType(image->getPixelDataType()),
                 image->getData());
}

void Texture::bind(unsigned short target) {
    glActiveTexture(GL_TEXTURE0 + target);
    glBindTexture(convertType(type), texture);
}

GLenum Texture::convertType(Type type)
{
    switch (type)
    {
#ifdef GL_TEXTURE_1D
        case Type::t1d:
            return GL_TEXTURE_1D;
#endif
        
        case Type::t2d:
            return GL_TEXTURE_2D;
        
        case Type::t3d:
            return GL_TEXTURE_3D;
        
        case Type::cubemap:
            return GL_TEXTURE_CUBE_MAP;

#ifdef GL_TEXTURE_RECTANGLE
        case Type::rectangle:
            return GL_TEXTURE_RECTANGLE;
#endif

#ifdef GL_TEXTURE_EXTERNAL_OES
        case Type::external:
            return GL_TEXTURE_EXTERNAL_OES;
#endif

        default: return GL_NONE;
    }
}

GLint Texture::convertPixelFormat(PixelFormat format)
{
    switch (format)
    {
        case PixelFormat::r8:
            return GL_R8;
        
        case PixelFormat::r8_snorm:
            return GL_R8_SNORM;
        
        case PixelFormat::r16f:
            return GL_R16F;
        
        case PixelFormat::r32f:
            return GL_R32F;
        
        case PixelFormat::r8i:
            return GL_R8I;
        
        case PixelFormat::r8ui:
            return GL_R8UI;
        
        case PixelFormat::r16i:
            return GL_R16I;
        
        case PixelFormat::r16ui:
            return GL_R16UI;
        
        case PixelFormat::r32i:
            return GL_R32I;
        
        case PixelFormat::r32ui:
            return GL_R32UI;
        
        case PixelFormat::rg8:
            return GL_RG8;
        
        case PixelFormat::rg8_snorm:
            return GL_RG8_SNORM;
        
        case PixelFormat::rg16f:
            return GL_RG16F;
        
        case PixelFormat::rg32f:
            return GL_RG32F;
        
        case PixelFormat::rg8i:
            return GL_RG8I;
        
        case PixelFormat::rg8ui:
            return GL_RG8UI;
        
        case PixelFormat::rg16i:
            return GL_RG16I;
        
        case PixelFormat::rg16ui:
            return GL_RG16UI;
        
        case PixelFormat::rg32i:
            return GL_RG32I;
        
        case PixelFormat::rg32ui:
            return GL_RG32UI;
        
        case PixelFormat::rgb8:
            return GL_RGB8;
        
        case PixelFormat::rgb8_snorm:
            return GL_RGB8_SNORM;
        
        case PixelFormat::rgb565:
            return GL_RGB565;
        
        case PixelFormat::srgb8:
            return GL_SRGB8;
        
        case PixelFormat::rgb16f:
            return GL_RGB16F;
        
        case PixelFormat::rgb32f:
            return GL_RGB32F;
        
        case PixelFormat::r11f_g11f_b10f:
            return GL_R11F_G11F_B10F;
        
        case PixelFormat::rgb9_e5:
            return GL_RGB9_E5;
        
        case PixelFormat::rgb8i:
            return GL_RGB8I;
        
        case PixelFormat::rgb8ui:
            return GL_RGB8UI;
        
        case PixelFormat::rgb16i:
            return GL_RGB16I;
        
        case PixelFormat::rgb16ui:
            return GL_RGB16UI;
        
        case PixelFormat::rgb32i:
            return GL_RGB32I;
        
        case PixelFormat::rgb32ui:
            return GL_RGB32UI;
        
        case PixelFormat::rgba4:
            return GL_RGBA4;
        
        case PixelFormat::rgb5_a1:
            return GL_RGB5_A1;
        
        case PixelFormat::rgba8:
            return GL_RGBA8;
        
        case PixelFormat::rgba8_snorm:
            return GL_RGBA8_SNORM;
        
        case PixelFormat::rgb10_a2:
            return GL_RGB10_A2;
        
        case PixelFormat::rgb10_a2_ui:
            return GL_RGB10_A2UI;
        
        case PixelFormat::srgb8_alpha8:
            return GL_SRGB8_ALPHA8;
        
        case PixelFormat::rgba16f:
            return GL_RGBA16F;
        
        case PixelFormat::rgba32f:
            return GL_RGBA32F;
        
        case PixelFormat::rgba8i:
            return GL_RGBA8I;
        
        case PixelFormat::rgba8ui:
            return GL_RGBA8UI;
        
        case PixelFormat::rgba16i:
            return GL_RGBA16I;
        
        case PixelFormat::rgba16ui:
            return GL_RGBA16UI;
        
        case PixelFormat::rgba32i:
            return GL_RGBA32I;
        
        case PixelFormat::rgba32ui:
            return GL_RGBA32UI;
        
        case PixelFormat::alpha:
            return GL_ALPHA;

#ifdef GL_LUMINANCE
        case PixelFormat::luminance:
            return GL_LUMINANCE;
#endif

#ifdef GL_LUMINANCE_ALPHA
        case PixelFormat::luminance_alpha:
            return GL_LUMINANCE_ALPHA;
#endif
        
        default: return GL_NONE;
    }
}

GLint Texture::convertPixelFormatES2(PixelFormat format)
{
    switch (format)
    {
        case PixelFormat::rgb8:
            return GL_RGB;
        
        case PixelFormat::rgb565:
            return GL_RGB565;
        
        case PixelFormat::rgba4:
            return GL_RGBA4;
        
        case PixelFormat::rgb5_a1:
            return GL_RGB5_A1;
        
        case PixelFormat::rgba8:
            return GL_RGBA;
        
        case PixelFormat::alpha:
            return GL_ALPHA;

#ifdef GL_LUMINANCE
        case PixelFormat::luminance:
            return GL_LUMINANCE;
#endif

#ifdef GL_LUMINANCE_ALPHA
        case PixelFormat::luminance_alpha:
            return GL_LUMINANCE_ALPHA;
#endif

        default: return GL_NONE;
    }
}

GLenum Texture::convertImageComponentsType(ColorComponents components)
{
    switch (components)
    {
        case ColorComponents::red:
            return GL_RED;
        
        case ColorComponents::green:
            return GL_GREEN;
        
        case ColorComponents::blue:
            return GL_BLUE;
        
        case ColorComponents::alpha:
            return GL_ALPHA;
        
        case ColorComponents::rg:
            return GL_RG;
        
        case ColorComponents::rgb:
            return GL_RGB;
        
#ifdef GL_BGR
        case ColorComponents::bgr:
            return GL_BGR;
#endif
        
        case ColorComponents::rgba:
            return GL_RGBA;
#ifdef GL_BGRA
        case ColorComponents::bgra:
            return GL_BGRA;
#endif

#ifdef GL_LUMINANCE
        case ColorComponents::luminance:
            return GL_LUMINANCE;
#endif

#ifdef GL_LUMINANCE_ALPHA
        case ColorComponents::luminance_alpha:
            return GL_LUMINANCE_ALPHA;
#endif
        
        case ColorComponents::stencil:
            return GL_STENCIL;
        
        case ColorComponents::depth:
            return GL_DEPTH;
        
        case ColorComponents::depth_stencil:
            return GL_DEPTH_STENCIL;
        
        default: return GL_NONE;
    }
}

GLenum Texture::convertImageDataType(PixelDataType dataType)
{
    switch (dataType)
    {
        case PixelDataType::u8:
            return GL_UNSIGNED_BYTE;
        
        case PixelDataType::s8:
            return GL_BYTE;
        
        case PixelDataType::u16:
            return GL_UNSIGNED_SHORT;
        
        case PixelDataType::s16:
            return GL_SHORT;
        
        case PixelDataType::u32:
            return GL_UNSIGNED_INT;
        
        case PixelDataType::s32:
            return GL_INT;
        
        case PixelDataType::f32:
            return GL_FLOAT;

#ifdef GL_UNSIGNED_BYTE_3_3_2
        case PixelDataType::u332:
            return GL_UNSIGNED_BYTE_3_3_2;
#endif

#ifdef GL_UNSIGNED_BYTE_2_3_3_REV
        case PixelDataType::u233_rev:
            return GL_UNSIGNED_BYTE_2_3_3_REV;
#endif

#ifdef GL_UNSIGNED_SHORT_5_6_5
        case PixelDataType::u565:
            return GL_UNSIGNED_SHORT_5_6_5;
#endif

#ifdef GL_UNSIGNED_SHORT_5_6_5_REV
        case PixelDataType::u565_rev:
            return GL_UNSIGNED_SHORT_5_6_5_REV;
#endif
        
        case PixelDataType::u4444:
            return GL_UNSIGNED_SHORT_4_4_4_4;

#ifdef GL_UNSIGNED_SHORT_4_4_4_4_REV
        case PixelDataType::u4444_rev:
            return GL_UNSIGNED_SHORT_4_4_4_4_REV;
#endif

#ifdef GL_UNSIGNED_SHORT_5_5_5_1
        case PixelDataType::u5551:
            return GL_UNSIGNED_SHORT_5_5_5_1;
#endif

#ifdef GL_UNSIGNED_SHORT_1_5_5_5_REV
        case PixelDataType::u1555_rev:
            return GL_UNSIGNED_SHORT_1_5_5_5_REV;
#endif

#ifdef GL_UNSIGNED_INT_8_8_8_8
        case PixelDataType::u8888:
            return GL_UNSIGNED_INT_8_8_8_8;
#endif

#ifdef GL_UNSIGNED_INT_8_8_8_8_REV
        case PixelDataType::u8888_rev:
            return GL_UNSIGNED_INT_8_8_8_8_REV;
#endif

#ifdef GL_UNSIGNED_INT_10_10_10_2
        case PixelDataType::u10_10_10_2:
            return GL_UNSIGNED_INT_10_10_10_2;
#endif

#ifdef GL_UNSIGNED_INT_2_10_10_10_REV
        case PixelDataType::u2_10_10_10_rev:
            return GL_UNSIGNED_INT_2_10_10_10_REV;
#endif
        default: return GL_NONE;
    }
}

GLint Texture::convertMinFilter(MinFilter filter)
{
    switch (filter)
    {
        case MinFilter::nearest:
            return GL_NEAREST;
        
        case MinFilter::nearest_MipmapNearest:
            return GL_NEAREST_MIPMAP_NEAREST;
        
        case MinFilter::nearest_MipmapLinear:
            return GL_NEAREST_MIPMAP_LINEAR;
        
        case MinFilter::linear:
            return GL_LINEAR;
        
        case MinFilter::linear_MipmapNearest:
            return GL_LINEAR_MIPMAP_NEAREST;
        
        case MinFilter::linear_MipmapLinear:
            return GL_LINEAR_MIPMAP_LINEAR;
    };
}

GLint Texture::convertMagFilter(MagFilter filter)
{
    switch (filter) {
        case MagFilter::nearest:
            return GL_NEAREST;
        
        case MagFilter::linear:
            return GL_LINEAR;
    };
}

GLint Texture::convertWrap(Wrap wrap)
{
    switch (wrap)
    {
        case Wrap::clamp_to_edge:
            return GL_CLAMP_TO_EDGE;

#ifdef GL_CLAMP_TO_BORDER
        case Wrap::clamp_to_border:
            return GL_CLAMP_TO_BORDER;
#endif

        case Wrap::repeat:
            return GL_REPEAT;
        
        default:
            return GL_NONE;
    }
}

void Texture::generateMipmap() {
    glGenerateMipmap(convertType(type));
}

GLuint Texture::getObjectId() const {
    return texture;
}

void Texture::setObjectId(GLuint id) {
    texture = id;
}
