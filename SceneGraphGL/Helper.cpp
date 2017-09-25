//
//  Created by mutexre on 10/01/16.
//  Copyright © 2016 mutexre. All rights reserved.
//

#include <SceneGraphGL/SceneGraphGL.hpp>

#define glErrorToStringCase(glError) case glError: return #glError;

namespace SG
{
    namespace GL
    {
        const char* glErrorToString(GLenum error)
        {
            switch (error)
            {
                glErrorToStringCase(GL_NO_ERROR);
                glErrorToStringCase(GL_INVALID_ENUM);
                glErrorToStringCase(GL_INVALID_VALUE);
                glErrorToStringCase(GL_INVALID_OPERATION);
                glErrorToStringCase(GL_OUT_OF_MEMORY);
                glErrorToStringCase(GL_INVALID_FRAMEBUFFER_OPERATION);
                default: return "Unknown OpenGL error";
            };
        }

        void checkGlErrors()
        {
            GLenum error = glGetError();
            LOGE("%s\n", glErrorToString(error));
        }

        void checkGlErrors(const char* msg)
        {
            GLenum error = glGetError();
            #ifdef NDEBUG
            const char* state = "NDEBUG";
            #else
            const char* state = "no NDEBUG";
            #endif
            switch (error) {
                case GL_NO_ERROR:
                    break;
                case GL_INVALID_ENUM:
                    LOGE("OpenGL error: GL_INVALID_ENUM: %s\n", msg);
                #ifdef NDEBUG
                    throw runtime_error(string("OpenGL error: GL_INVALID_ENUM: ") + msg);
                #endif
                    break;
                case GL_INVALID_VALUE:
                    LOGE("OpenGL error: GL_INVALID_VALUE: %s\n", msg);
                #ifdef NDEBUG
                    throw runtime_error(string("OpenGL error: GL_INVALID_VALUE: ") + msg);
                #endif
                    break;
                case GL_INVALID_OPERATION:
                    LOGE("OpenGL error: GL_INVALID_OPERATION: %s\n", msg);
                #ifdef NDEBUG
                    throw runtime_error(string("OpenGL error: GL_INVALID_OPERATION: ") + msg);
                #endif
                    break;
                case GL_OUT_OF_MEMORY:
                    LOGE("OpenGL error: GL_OUT_OF_MEMORY: %s\n", msg);
                #ifdef NDEBUG
                    throw runtime_error(string("OpenGL error: GL_OUT_OF_MEMORY: ") + msg);
                #endif
                    break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    LOGE("OpenGL error: GL_INVALID_FRAMEBUFFER_OPERATION: %s, %s\n", msg, state);
                #ifdef NDEBUG
                    throw runtime_error(string("OpenGL error: GL_INVALID_FRAMEBUFFER_OPERATION: ") + msg);
                #endif
                    break;
                default:
                    LOGE("OpenGL error %u: %s\n", error, msg);
                #ifdef NDEBUG
                    throw runtime_error(string("OpenGL error: ") + std::to_string(error) + string(" ") + msg);
                #endif
            };
        }
        
        void checkGlErrors(const char* file, int line)
        {
            GLenum error = glGetError();
            switch (error)
            {
                case GL_NO_ERROR: break;
                
                case GL_INVALID_ENUM:
                    LOGE("OpenGL error: GL_INVALID_ENUM at %s:%d\n", file, line);
                
                #ifdef NDEBUG
                    throw runtime_error(string("OpenGL error: GL_INVALID_ENUM: ") + file + ":" + to_string(line));
                #endif
                
                break;
                
                case GL_INVALID_VALUE:
                    LOGE("OpenGL error: GL_INVALID_VALUE at %s:%d\n", file, line);
                #ifdef NDEBUG
                    throw runtime_error(string("OpenGL error: GL_INVALID_VALUE: ") + file + ":" + to_string(line));
                #endif
                    break;
                case GL_INVALID_OPERATION:
                    LOGE("OpenGL error: GL_INVALID_OPERATION at %s:%d\n", file, line);
                #ifdef NDEBUG
                    throw runtime_error(string("OpenGL error: GL_INVALID_OPERATION: ") + file + ":" + to_string(line));
                #endif
                    break;
                case GL_OUT_OF_MEMORY:
                    LOGE("OpenGL error: GL_OUT_OF_MEMORY: at %s:%d\n", file, line);
                #ifdef NDEBUG
                    throw runtime_error(string("OpenGL error: GL_OUT_OF_MEMORY: ") + file + ":" + to_string(line));
                #endif
                    break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    LOGE("OpenGL error: GL_INVALID_FRAMEBUFFER_OPERATION:  at %s:%d\n", file, line);
                #ifdef NDEBUG
                    throw runtime_error(string("OpenGL error: GL_INVALID_FRAMEBUFFER_OPERATION: ") + file + ":" + to_string(line));
                #endif
                    break;
                default:
                    LOGE("OpenGL error %u: at %s:%d\n", error, file, line);
                #ifdef NDEBUG
                    throw runtime_error(string("OpenGL error: ") + to_string(error) + string(" ") + file + ":" + to_string(line));
                #endif
            };
        }

        GLint getCurrentVertexArrayBinding() {
            GLint binding;
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &binding);
            return binding;
        }

        GLint getCurrentBufferBinding(GLenum target) {
            GLint binding;
            glGetIntegerv(target, &binding);
            return binding;
        }

        GLint getCurrentProgram() {
            GLint program;
            glGetIntegerv(GL_CURRENT_PROGRAM, &program);
            return program;
        }

        void printGlBindings() {
            LOGI("Bindings:\n  Vertex Array: %u\n  Array buffer: %u\n  Element Array buffer: %u\nProgram: %u\n",
                   getCurrentVertexArrayBinding(),
                   getCurrentBufferBinding(GL_ARRAY_BUFFER_BINDING),
                   getCurrentBufferBinding(GL_ELEMENT_ARRAY_BUFFER_BINDING),
                   getCurrentProgram());
        }

        GLbitfield convertBuffersToClear(int buffersToClear)
        {
            GLbitfield mask = 0;

            if (buffersToClear & int(BufferBit::color))
                mask |= GL_COLOR_BUFFER_BIT;

            if (buffersToClear & int(BufferBit::depth))
                mask |= GL_DEPTH_BUFFER_BIT;

#ifdef GL_ACCUM_BUFFER_BIT
            if (buffersToClear & int(BufferBit::accum))
                mask |= GL_ACCUM_BUFFER_BIT;
#endif

            if (buffersToClear & int(BufferBit::stencil))
                mask |= GL_STENCIL_BUFFER_BIT;

            return mask;
        }

        GLenum convertBlendFunc(Settings::Blend::Func func)
        {
            switch (func) {
                case Settings::Blend::Func::zero:
                    return GL_ZERO;

                case Settings::Blend::Func::one:
                    return GL_ONE;

                case Settings::Blend::Func::srcColor:
                    return GL_SRC_COLOR;

                case Settings::Blend::Func::dstColor:
                    return GL_DST_COLOR;

                case Settings::Blend::Func::srcAlpha:
                    return GL_SRC_ALPHA;

                case Settings::Blend::Func::dstAlpha:
                    return GL_DST_ALPHA;

                case Settings::Blend::Func::oneMinusSrcColor:
                    return GL_ONE_MINUS_SRC_COLOR;

                case Settings::Blend::Func::oneMinusDstColor:
                    return GL_ONE_MINUS_SRC_ALPHA;

                case Settings::Blend::Func::oneMinusSrcAlpha:
                    return GL_ONE_MINUS_SRC_ALPHA;

                case Settings::Blend::Func::oneMinusDstAlpha:
                    return GL_ONE_MINUS_DST_ALPHA;

                case Settings::Blend::Func::srcAlphaSaturate:
                    return GL_SRC_ALPHA_SATURATE;

                case Settings::Blend::Func::constColor:
                    return GL_CONSTANT_COLOR;

                case Settings::Blend::Func::oneMinusConstColor:
                    return GL_ONE_MINUS_CONSTANT_COLOR;

                case Settings::Blend::Func::constAlpha:
                    return GL_CONSTANT_ALPHA;

                case Settings::Blend::Func::oneMinusConstAlpha:
                    return GL_ONE_MINUS_CONSTANT_ALPHA;

                default:
                    throw "unknown blend func type";
            }
        }

        Settings::Blend::Func convertBlendFunc(GLenum func)
        {
            switch (func) {
                case GL_ZERO:
                    return Settings::Blend::Func::zero;

                case GL_ONE:
                    return Settings::Blend::Func::one;

                case GL_SRC_COLOR:
                    return Settings::Blend::Func::srcColor;

                case GL_DST_COLOR:
                    return Settings::Blend::Func::dstColor;

                case GL_SRC_ALPHA:
                    return Settings::Blend::Func::srcAlpha;

                case GL_DST_ALPHA:
                    return Settings::Blend::Func::dstAlpha;

                case GL_ONE_MINUS_SRC_COLOR:
                    return Settings::Blend::Func::oneMinusSrcColor;

                case GL_ONE_MINUS_DST_COLOR:
                    return Settings::Blend::Func::oneMinusDstColor;

                case GL_ONE_MINUS_SRC_ALPHA:
                    return Settings::Blend::Func::oneMinusSrcAlpha;

                case GL_ONE_MINUS_DST_ALPHA:
                    return Settings::Blend::Func::oneMinusDstAlpha;

                case GL_SRC_ALPHA_SATURATE:
                    return Settings::Blend::Func::srcAlphaSaturate;

                case GL_CONSTANT_COLOR:
                    return Settings::Blend::Func::constColor;

                case GL_ONE_MINUS_CONSTANT_COLOR:
                    return Settings::Blend::Func::oneMinusConstColor;

                case GL_CONSTANT_ALPHA:
                    return Settings::Blend::Func::constAlpha;

                case GL_ONE_MINUS_CONSTANT_ALPHA:
                    return Settings::Blend::Func::oneMinusConstAlpha;

                default:
                    throw "unknown blend func type";
            }
        }

        GLenum convertBlendEquation(Settings::Blend::Equation equation)
        {
            switch (equation) {
                case Settings::Blend::Equation::add:
                    return GL_FUNC_ADD;

                case Settings::Blend::Equation::subtract:
                    return GL_FUNC_SUBTRACT;

                case Settings::Blend::Equation::reverseSubtract:
                    return GL_FUNC_REVERSE_SUBTRACT;

                case Settings::Blend::Equation::min:
                    return GL_MIN;

                case Settings::Blend::Equation::max:
                    return GL_MAX;

                default:
                    throw "unknown blend equation type";
            }
        }

        Settings::Blend::Equation convertBlendEquation(GLenum equation)
        {
            switch (equation) {
                case GL_FUNC_ADD:
                    return Settings::Blend::Equation::add;

                case GL_FUNC_SUBTRACT:
                    return Settings::Blend::Equation::subtract;

                case GL_FUNC_REVERSE_SUBTRACT:
                    return Settings::Blend::Equation::reverseSubtract;

                case GL_MIN:
                    return Settings::Blend::Equation::min;

                case GL_MAX:
                    return Settings::Blend::Equation::max;

                default:
                    throw "unknown blend equation type";
            }
        }

        GLenum convertCullFaceMode(Settings::CullFace::Mode mode)
        {
            switch (mode) {
                case Settings::CullFace::Mode::back:
                    return GL_BACK;

                case Settings::CullFace::Mode::front:
                    return GL_FRONT;

                case Settings::CullFace::Mode::frontAndBack:
                    return GL_FRONT_AND_BACK;

                default:
                    throw "invalid cull face mode";
            }
        }

        Settings::CullFace::Mode convertCullFaceMode(GLenum mode)
        {
            switch (mode) {
                case GL_BACK:
                    return Settings::CullFace::Mode::back;

                case GL_FRONT:
                    return Settings::CullFace::Mode::front;

                case GL_FRONT_AND_BACK:
                    return Settings::CullFace::Mode::frontAndBack;

                default:
                    throw "invalid cull face mode";
            }
        }
    }
}
