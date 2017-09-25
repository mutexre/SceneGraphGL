//
//  Created by mutexre on 18/10/2016.
//  Copyright © 2016 Some Organization. All rights reserved.
//

namespace SG::GL
{
    class Surface : public SG::Surface,
                    public Object
    {
    protected:
        GLuint framebuffer;
        bool external = false;

    protected:
        bool checkFramebufferCompleteness(GLenum target) const;
        void makeActive(GLuint framebuffer);

        static GLenum convertAttachmentPoint(AttachmentPoint);
        static GLenum convertTarget(Target);

        void attach(AttachmentPoint,
                    unsigned attachmentIndex,
                    GLuint renderbuffer);

    public:
        Surface();
        Surface(GLint framebuffer);
        virtual ~Surface();

        virtual void attach(AttachmentPoint,
                            unsigned attachmentIndex,
                            const shared_ptr<SG::Texture>&,
                            unsigned level = 0,
                            unsigned cubeMapSide = 0) override;

        virtual void attach(AttachmentPoint,
                            unsigned attachmentIndex,
                            const shared_ptr<SG::Renderbuffer>&) override;

        virtual void detach(AttachmentPoint, unsigned attachmentIndex = 0) override;

        virtual void makeActive(Target) override;
        virtual void makeActive() override;

        virtual void read(unsigned x, unsigned y, unsigned w, unsigned h,
                          ColorComponents, PixelDataType,
                          void *data) override;

        virtual void invalidate(Target) override;
        virtual void invalidate() override;

        void setObjectId(GLuint);
    };
}
