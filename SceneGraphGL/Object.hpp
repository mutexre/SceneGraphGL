//
//  Created by mutexre on 23/02/2017.
//  Copyright © 2017 Some Organization. All rights reserved.
//

namespace SG::GL
{
    class Object : public virtual SG::Object
    {
    public:
        shared_ptr<GL::Context> getGlContext() const;
        
        API getAPI() const;
        int getVersion(int) const;
    
        bool isCoreProfileOrES3() const;
        bool isWrapRSupported() const;
    };
}
