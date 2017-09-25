//
//  Created by mutexre on 08/01/16.
//  Copyright © 2016 mutexre. All rights reserved.
//

#pragma once

namespace SG::GL
{
    class Context;
    class Object;
    class Scene;
    class Node;
    class MeshNode;
    class Light;
    class Camera;
    class Mesh;
    class Texture;
    class Program;
    class Renderbuffer;
    class OffScreenBuffer;
    class Surface;
    
#ifdef __ANDROID__
    class ScreenBufferAndroid;
#endif
}

#include <SceneGraph/SceneGraph.hpp>
#include <SceneGraphGL/Platform.hpp>

#if DEBUG
    #include <SceneGraphGL/Debug/DebugHelper.hpp>
#endif

#include <SceneGraphGL/Helper.hpp>
#include <SceneGraphGL/Context.hpp>
#include <SceneGraphGL/Object.hpp>
#include <SceneGraphGL/Surface/Texture.hpp>
#include <SceneGraphGL/Scene.hpp>
#include <SceneGraphGL/Node.hpp>
#include <SceneGraphGL/MeshNode.hpp>
#include <SceneGraphGL/Light.hpp>
#include <SceneGraphGL/Camera.hpp>
#include <SceneGraphGL/Mesh.hpp>
#include <SceneGraphGL/Program.hpp>
#include <SceneGraphGL/Surface/Surface.hpp>
#include <SceneGraphGL/Surface/Renderbuffer.hpp>
#include <SceneGraphGL/Surface/OffScreenBuffer.hpp>
#include <SceneGraphGL/Material/TextureMaterialYUV.hpp>

#if defined(__APPLE__) && defined(__OBJC__)
    #if TARGET_OS_IPHONE || TARGET_OS_SIMULATOR
        #include <SceneGraphGL/Surface/ScreenbufferIOS.hpp>
    #endif
#elif defined(__ANDROID__)
    #include <SceneGraphGL/Surface/ScreenBufferAndroid.hpp>
#endif
