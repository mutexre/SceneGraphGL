//
//  Created by mutexre on 23/02/2017.
//  Copyright © 2017 Some Organization. All rights reserved.
//

#include <SceneGraphGL/SceneGraphGL.hpp>

using namespace SG::GL;

shared_ptr<SG::GL::Context> Object::getGlContext() const {
    return static_pointer_cast<GL::Context>(context);
}

API Object::getAPI() const {
    return getGlContext()->getAPI();
}

int Object::getVersion(int index) const {
    return getGlContext()->getVersion(index);
}

bool Object::isCoreProfileOrES3() const {
    return getGlContext()->isCoreProfileOrES3();
}

bool Object::isWrapRSupported() const {
    return getGlContext()->isWrapRSupported();
}
