//
//  Created by mutexre on 20/04/2017.
//  Copyright © 2017 mutexre. All rights reserved.
//

#ifdef __APPLE__

    #include "TargetConditionals.h"

    #if TARGET_OS_IPHONE || TARGET_OS_SIMULATOR

        #include <OpenGLES/ES3/gl.h>
        #include <OpenGLES/ES3/glext.h>

        #define SGGL_TEX_STORAGE_SUPPORTED 1

    #else

        #include <OpenGL/gl3.h>
        
        #define SGGL_TEX_STORAGE_SUPPORTED 0

    #endif

#elif defined _WIN32 || defined _WIN64

    #include <gl/glew.h>
    #include <windows.h>
    #include <stdint.h>

    #define SGGL_TEX_STORAGE_SUPPORTED 1

#elif defined __ANDROID__

	#include <GLES3/gl3.h>
	#include <GLES/glext.h>
	#include <EGL/egl.h>

    #ifdef NDEBUG
        #define DEBUG 1
    #else
        #define DEBUG 0
    #endif

    #define SGGL_TEX_STORAGE_SUPPORTED 1

#endif
