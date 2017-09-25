//
//  Segment.hpp
//  SceneGraphGL
//
//  Created by mutexre on 11/01/16.
//  Copyright © 2016 mutexre. All rights reserved.
//

namespace SG::GL
{
     class Mesh : public SG::Mesh
     {
     private:
         GLuint vao = GL_NONE;

         struct Buf {
             GLuint id = GL_NONE;
             size_t size = 0;
             GLenum accessHint = GL_STATIC_DRAW;
         };

         struct {
             Buf indices, coords, normals, uvs;
         }
         buffers;

         GLenum mode;
         GLenum indicesType = GL_UNSIGNED_INT;

     private:
         void setupGlObjects();
         void updateBuffer(Buf&, const void* data, size_t size);

         static GLenum convertPrimitivesType(PrimitivesType);

     public:
         Mesh();
         virtual ~Mesh();

         virtual void init() override;

         virtual Mesh& setPrimitivesType(PrimitivesType) override;

         virtual Mesh& enableNormalsArray(bool = true) override;
         virtual Mesh& enableUVsArray(bool = true) override;

         virtual void update() override;
         virtual void bind() override;
         virtual void draw() override;
     };
}
