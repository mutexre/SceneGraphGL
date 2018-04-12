//
//  Created by mutexre on 21/10/2016.
//  Copyright © 2016 Some Organization. All rights reserved.
//

namespace SG::GL
{
    class Renderbuffer : public SG::Renderbuffer
    {
    protected:
        GLuint renderbuffer;

    public:
        Renderbuffer();
        virtual ~Renderbuffer();

        virtual void makeActive() override;

        GLuint getObjectId() const;
        
        virtual void allocStorage(int w, int h,
                                  PixelFormat format,
                                  int samples) override;
    };
}
