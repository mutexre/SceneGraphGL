//
// Created by Sergey Vasilenko on 25.10.16.
//

#pragma once

#include <SceneGraph/SceneGraph.hpp>
#include <SceneGraphGL/SceneGraphGL.hpp>

namespace SG::GL
{
    class ScreenBufferAndroid : public Renderbuffer
    {
    private:
        uvec2 fetchSize() const;

    public:
        bool bindBuffer(GLuint renderbuffer);
    };
}
