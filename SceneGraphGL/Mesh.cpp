//
//  Created by mutexre on 11/01/16.
//  Copyright © 2016 mutexre. All rights reserved.
//

#include <SceneGraphGL/SceneGraphGL.hpp>

using namespace SG::GL;

Mesh::Mesh() : SG::Mesh()
{}

void Mesh::init() {
    setupGlObjects();
}

void Mesh::setupGlObjects()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &buffers.indices.id);
    glGenBuffers(1, &buffers.coords.id);
    glGenBuffers(1, &buffers.normals.id);
    glGenBuffers(1, &buffers.colors.id);
    glGenBuffers(1, &buffers.uvs.id);

    glBindVertexArray(vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.indices.id);

    if (program->isVertexAttributeActive("coord"))
    {
        auto location = program->getVertexAttributeLocation("coord");
        glBindBuffer(GL_ARRAY_BUFFER, buffers.coords.id);
        glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(location);
    }

    if (program->isVertexAttributeActive("normal"))
    {
        auto location = program->getVertexAttributeLocation("normal");
        glBindBuffer(GL_ARRAY_BUFFER, buffers.normals.id);
        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(location);
    }
    
    if (program->isVertexAttributeActive("color"))
    {
        auto location = program->getVertexAttributeLocation("color");
        glBindBuffer(GL_ARRAY_BUFFER, buffers.colors.id);
        glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(location);
    }

    if (program->isVertexAttributeActive("uv"))
    {
        auto location = program->getVertexAttributeLocation("uv");
        glBindBuffer(GL_ARRAY_BUFFER, buffers.uvs.id);
        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(location);
    }
}

void Mesh::updateBuffer(Buf& buf, const void* data, size_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, buf.id);
    
    if (size != buf.size) {
        glBufferData(GL_ARRAY_BUFFER, size, data, buf.accessHint);
        buf.size = size;
    }
    else
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void Mesh::update()
{
    bind();
    
    if (!directMode.enabled && !valid.indices) {
        auto size = data.indices.size() * sizeof(unsigned);
        if (size != buffers.indices.size) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data.indices.data(), buffers.indices.accessHint);
            buffers.indices.size = size;
        }
        else
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data.indices.data());
        
        valid.indices = true;
    }
    
    if (!valid.coords) {
        updateBuffer(buffers.coords, data.coords.data(), data.coords.size() * sizeof(vec4));
        valid.coords = true;
    }
    
    if (enabled.normals && !valid.normals) {
        updateBuffer(buffers.normals, data.normals.data(), data.normals.size() * sizeof(vec3));
        valid.normals = true;
    }

    if (enabled.colors && !valid.colors) {
        updateBuffer(buffers.colors, data.colors.data(), data.colors.size() * sizeof(vec4));
        valid.colors = true;
    }

    if (enabled.uvs && !valid.uvs) {
        updateBuffer(buffers.uvs, data.uvs.data(), data.uvs.size() * sizeof(vec2));
        valid.uvs = true;
    }
}

void Mesh::bind() {
    glBindVertexArray(vao);
}

void Mesh::draw()
{
    if (!directMode.enabled)
        glDrawElements(mode, GLsizei(data.indices.size()), indicesType, nullptr);
    else
        glDrawArrays(mode, 0, data.coords.size());
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &buffers.indices.id);
    glDeleteBuffers(1, &buffers.coords.id);
    glDeleteBuffers(1, &buffers.normals.id);
    glDeleteBuffers(1, &buffers.colors.id);
    glDeleteBuffers(1, &buffers.uvs.id);
}

Mesh& Mesh::setPrimitivesType(PrimitivesType type)
{
    primitivesType = type;
    mode = convertPrimitivesType(primitivesType);
    return *this;
}

Mesh& Mesh::enableNormalsArray(bool enable)
{
    SG::Mesh::enableNormalsArray(enable);
    
    auto location = program->getVertexAttributeLocation("normal");
    
    bind();
    if (enable)
        glEnableVertexAttribArray(location);
    else
        glDisableVertexAttribArray(location);
    
    return *this;
}

Mesh& Mesh::enableColorsArray(bool enable)
{
    SG::Mesh::enableColorsArray(enable);
    
    auto location = program->getVertexAttributeLocation("color");
    
    bind();
    if (enable)
        glEnableVertexAttribArray(location);
    else
        glDisableVertexAttribArray(location);
    
    return *this;
}

Mesh& Mesh::enableUVsArray(bool enable)
{
    SG::Mesh::enableUVsArray(enable);
    
    auto location = program->getVertexAttributeLocation("uv");
    
    bind();
    if (enable)
        glEnableVertexAttribArray(location);
    else
        glDisableVertexAttribArray(location);
    
    return *this;
}

GLenum Mesh::convertPrimitivesType(PrimitivesType pType)
{
    switch (pType)
    {
        case PrimitivesType::points:
            return GL_POINTS;
        
        case PrimitivesType::lines:
            return GL_LINES;
        
        case PrimitivesType::triangles:
            return GL_TRIANGLES;
        
        default:
            return GL_NONE;
    }
}
