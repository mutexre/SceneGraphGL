//
// Created by Sergey Vasilenko on 25.10.16.
//

#pragma once
#ifndef ANDROID_SCREENBUFFERANDROID_HPP
#define ANDROID_SCREENBUFFERANDROID_HPP

#include <SceneGraph/SceneGraph.hpp>
#include <SceneGraphGL/SceneGraphGL.hpp>

namespace SG {
    namespace GL {
        class ScreenBufferAndroid : public Renderbuffer {
        private:
            uvec2 fetchSize() const;

        public:
            bool bindBuffer(GLuint renderbuffer);
        };
    }
}


#endif //ANDROID_SCREENBUFFERANDROID_HPP
