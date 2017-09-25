//
//  Created by mutexre on 05/11/15.
//  Copyright © 2015 mutexre. All rights reserved.
//

#include <SceneGraphGL/SceneGraphGL.hpp>

using namespace SG::GL;

TextureMaterialYUV::TextureMaterialYUV(const shared_ptr<Context>& context)
    : SG::TextureMaterialYUV(context)
{}

TextureMaterialYUV::TextureMaterialYUV(const shared_ptr<Context>& context,
                                       const shared_ptr<Program>& program,
                                       const shared_ptr<Texture>& y,
                                       const shared_ptr<Texture>& uv,
                                       mat3 colorConversion,
                                       mat3 yTransform,
                                       mat3 uvTransform)
    : SG::TextureMaterialYUV(context, program, y, uv, colorConversion, yTransform, uvTransform)
{}

TextureMaterialYUV::TextureMaterialYUV(const shared_ptr<Context>& context,
                                       const shared_ptr<Program>& program,
                                       const shared_ptr<Texture>& y,
                                       const shared_ptr<Texture>& u,
                                       const shared_ptr<Texture>& v,
                                       mat3 colorConversion,
                                       mat3 yTransform,
                                       mat3 uTransform,
                                       mat3 vTransform)
    : SG::TextureMaterialYUV(context, program, y, u, v, colorConversion, yTransform, uTransform, vTransform)
{}

TextureMaterialYUV& TextureMaterialYUV::setY(GLuint texture)
{
    static_pointer_cast<GL::Texture>(textures.y)->setObjectId(texture);
    return *this;
}

TextureMaterialYUV& TextureMaterialYUV::setU(GLuint texture)
{
    static_pointer_cast<GL::Texture>(textures.u)->setObjectId(texture);
    return *this;
}

TextureMaterialYUV& TextureMaterialYUV::setV(GLuint texture)
{
    static_pointer_cast<GL::Texture>(textures.v)->setObjectId(texture);
    return *this;
}

TextureMaterialYUV& TextureMaterialYUV::setUV(GLuint texture)
{
    static_pointer_cast<GL::Texture>(textures.uv)->setObjectId(texture);
    return *this;
}
