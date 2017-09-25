//
//  Created by mutexre on 19/10/2016.
//  Copyright © 2016 Some Organization. All rights reserved.
//

#include <OpenGLES/EAGLDrawable.h>

namespace SG::GL
{
    class ScreenbufferIOS : public Renderbuffer
    {
    private:
        uvec2 fetchSize() const;
    
    public:
        ScreenbufferIOS();
        bool bindStorage(EAGLContext*, id<EAGLDrawable>);
    };
}
