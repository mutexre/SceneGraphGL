//
//  Created by mutexre on 07/01/16.
//  Copyright © 2016 mutexre. All rights reserved.
//

namespace SG::GL
{
    class Scene : public SG::Scene,
                  public Object
    {
    public:
        Scene(const shared_ptr<SG::Context>&);
        
        virtual void init() override;
        virtual void setLightsOnGPU(const shared_ptr<SG::Program> &, const mat4 &viewMatrix) override;
    };
}
