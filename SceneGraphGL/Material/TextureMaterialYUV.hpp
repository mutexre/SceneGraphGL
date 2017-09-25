//
//  Created by mutexre on 05/11/15.
//  Copyright © 2015 mutexre. All rights reserved.
//

namespace SG::GL
{
    class TextureMaterialYUV : public SG::TextureMaterialYUV
    {
    public:
        TextureMaterialYUV(const shared_ptr<Context>&);
        
        TextureMaterialYUV(const shared_ptr<Context>&,
                           const shared_ptr<Program>&,
                           const shared_ptr<Texture>& y,
                           const shared_ptr<Texture>& uv,
                           mat3 colorConversion,
                           mat3 yTransform = mat3(1),
                           mat3 uvTransform = mat3(1));
        
        TextureMaterialYUV(const shared_ptr<Context>&,
                           const shared_ptr<Program>&,
                           const shared_ptr<Texture>& y,
                           const shared_ptr<Texture>& u,
                           const shared_ptr<Texture>& v,
                           mat3 colorConversion,
                           mat3 yTransform = mat3(1),
                           mat3 uTransform = mat3(1),
                           mat3 vTransform = mat3(1));
        
        TextureMaterialYUV& setY(GLuint);
        TextureMaterialYUV& setU(GLuint);
        TextureMaterialYUV& setV(GLuint);
        TextureMaterialYUV& setUV(GLuint);
    };
}
