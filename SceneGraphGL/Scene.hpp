//
//  Created by mutexre on 07/01/16.
//  Copyright © 2016 mutexre. All rights reserved.
//

namespace SG::GL
{
    class Scene : public SG::Scene
//                  public Object
    {
    public:
        Scene(const std::shared_ptr<SG::Context>&);
        
        virtual void init() override;
        
        virtual void setLightsOnGPU(const std::shared_ptr<SG::Program> &,
                                    const glm::mat4 &viewMatrix) override;
    };
}
