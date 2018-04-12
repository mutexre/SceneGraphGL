//
//  Created by mutexre on 08/01/16.
//  Copyright © 2016 mutexre. All rights reserved.
//

namespace SG::GL
{
    class Texture : public SG::Texture
    {
    private:
        GLuint texture;

    public:
        static GLenum convertType(Type);
        static GLint convertPixelFormat(PixelFormat);
        static GLint convertPixelFormatES2(PixelFormat);
        static GLenum convertImageComponentsType(ColorComponents);
        static GLenum convertImageDataType(PixelDataType);
        static GLint convertMinFilter(MinFilter);
        static GLint convertMagFilter(MagFilter);
        static GLint convertWrap(Wrap);

    protected:
        virtual void setImages(const std::vector<std::shared_ptr<Image>> &) override;
        virtual void setImages(const std::vector<std::shared_ptr<ImageStack>> &) override;

        void setImage(unsigned level, PixelFormat, const std::shared_ptr<Image> &);
        void setImage(unsigned level, const std::shared_ptr<Image> &);
        void setCubemapImage(unsigned side, unsigned level, const std::shared_ptr<Image> &);

        void setParameters();

    public:
        Texture();
        Texture(Type, PixelFormat);
        virtual ~Texture();

        virtual void init(Texture::Type, PixelFormat) override;

        virtual void allocStorage(unsigned width,
                                  unsigned height,
                                  unsigned depth = 0,
                                  unsigned level = 0,
                                  unsigned layer = 0) override;

        virtual void allocImmutableStorage(unsigned width, unsigned height, unsigned depth,
                                           unsigned levelsCount = 1,
                                           unsigned layersCount = 1) override;

        virtual void setImage(const void *data,
                              unsigned width, unsigned height, unsigned depth,
                              ColorComponents imageColorComponents,
                              PixelDataType imagePixelDataType,
                              unsigned level = 0,
                              unsigned layer = 0) override;

        virtual void setSubImage(const void *data,
                                 unsigned x,
                                 unsigned y,
                                 unsigned z,
                                 unsigned width,
                                 unsigned height,
                                 unsigned depth,
                                 ColorComponents = ColorComponents::rgba,
                                 PixelDataType = PixelDataType::u8,
                                 unsigned level = 0,
                                 unsigned layer = 0) override;

        virtual void bind(unsigned short target = 0) override;
        virtual void generateMipmap() override;

        GLuint getObjectId() const;
        void setObjectId(GLuint);
    };
}
