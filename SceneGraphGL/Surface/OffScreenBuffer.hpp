//
//  Created by mutexre on 21/10/2016.
//  Copyright © 2016 Some Organization. All rights reserved.
//

namespace SG::GL
{
    class OffScreenBuffer : public Renderbuffer,
                            public SG::OffScreenBuffer
    {
    public:
        virtual void allocStorage(int w, int h,
                                  PixelFormat = PixelFormat::rgba8,
                                  int samples = 1) override;
    };
}
