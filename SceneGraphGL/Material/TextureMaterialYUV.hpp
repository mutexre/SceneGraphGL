//
//  Created by mutexre on 05/11/15.
//  Copyright © 2015 mutexre. All rights reserved.
//

namespace SG::GL
{
    class TextureMaterialYUV : public SG::TextureMaterialYUV
    {
    public:
        TextureMaterialYUV(const std::shared_ptr<Context>&);
        
        TextureMaterialYUV(const std::shared_ptr<Context>&,
                           const std::shared_ptr<Program>&,
                           const std::shared_ptr<Texture>& y,
                           const std::shared_ptr<Texture>& uv,
                           const glm::mat3& colorConversion,
                           const glm::mat3& yTransform = glm::mat3(1),
                           const glm::mat3& uvTransform = glm::mat3(1));
        
        TextureMaterialYUV(const std::shared_ptr<Context>&,
                           const std::shared_ptr<Program>&,
                           const std::shared_ptr<Texture>& y,
                           const std::shared_ptr<Texture>& u,
                           const std::shared_ptr<Texture>& v,
                           const glm::mat3& colorConversion,
                           const glm::mat3& yTransform = glm::mat3(1),
                           const glm::mat3& uTransform = glm::mat3(1),
                           const glm::mat3& vTransform = glm::mat3(1));
        
        TextureMaterialYUV& setY(GLuint);
        TextureMaterialYUV& setU(GLuint);
        TextureMaterialYUV& setV(GLuint);
        TextureMaterialYUV& setUV(GLuint);
    };
}
