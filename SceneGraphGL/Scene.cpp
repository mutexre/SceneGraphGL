//
//  Created by mutexre on 07/01/16.
//  Copyright © 2016 mutexre. All rights reserved.
//

#include <SceneGraphGL/SceneGraphGL.hpp>

using namespace SG::GL;

Scene::Scene(const shared_ptr<SG::Context>& context) : SG::Scene(context)
{}

void Scene::init()
{}

void Scene::setLightsOnGPU(const shared_ptr<SG::Program>& program, const mat4& viewMatrix)
{
    auto iter = lights.begin();
    int i = 0;
    auto lightCount = std::min(size_t(3), lights.size());
    
    while (iter != lights.end() && i < lightCount)
    {
        auto& light = *iter;
        if (light->isEnabled())
        {
            auto worldView = viewMatrix * light->getWorldMatrix();
            vec4 pos = worldView * vec4(0.f, 0.f, 0.f, 1.f);
            vec3 dir = normalize(mat3(worldView) * light->getDirection());
            light->setOnGPU(i, program, vec3(pos), dir);
            i++;
        }
        advance(iter, 1);
    }
    
    if (program->isUniformActive("lightCount"))
        program->set("lightCount", i);
}
