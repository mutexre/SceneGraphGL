//
//  Created by mutexre on 11/01/16.
//  Copyright © 2016 mutexre. All rights reserved.
//

#include <SceneGraphGL/SceneGraphGL.hpp>

using namespace SG::GL;

Program::Program() : program(0)
{}

Program::Program(const Source& src) {
    sources = src;
    load(src);
}

Program::Program(const Program& p)
{
#if 0
    program = glCreateProgram();
	if (!program) {
        throw runtime_error("failed to create shader program");
    }
    
    glProgramBinary(program, p.binary.format, p.binary.data.data(), p.binary.data.size());
#else
    load(p.sources);
#endif

    sources = p.sources;
}

Program::~Program() {
    if (program)
        glDeleteProgram(program);
}

GLuint Program::compile(GLenum type, const string& source)
{
    GLint isCompiled;
    
    const GLchar* sources[] = {
        source.c_str()
    };

    GLuint ID = glCreateShader(type);
    if (!ID) {
        throw runtime_error("failed to create shader");
    }

    glShaderSource(ID, 1, sources, 0);
    glCompileShader(ID);
    glGetShaderiv(ID, GL_COMPILE_STATUS, &isCompiled);

    if (!isCompiled) {
        GLsizei logLength;

        glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLsizei actualCompilationLogLength;

            auto log = new GLchar[logLength + 1];
            memset(log, 0, logLength + 1);

            glGetShaderInfoLog(ID, logLength, &actualCompilationLogLength, log);

            throw runtime_error(log);
        }

        throw runtime_error("failed to compile shader");
    }

    return ID;
}

void Program::link(GLuint id)
{
    GLint isLinked;

    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &isLinked);
    if (!isLinked) {
        GLsizei logLength;

        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLsizei actualCompilationLogLength;

            auto log = new GLchar[logLength + 1];
            memset(log, 0, logLength + 1);

            glGetProgramInfoLog(id, logLength, &actualCompilationLogLength, log);
            LOGW("link program failed because of %s", log);
            throw runtime_error(log);
        }

        throw runtime_error("link program failed");
    }
}

void Program::queryActiveAttributes()
{
    attribs.clear();

    int total = -1;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &total);
    for (int i = 0; i < total; i++) {
        int nameLen = -1;
        int size = -1;
        GLenum type = GL_ZERO;
        char name[100];

        glGetActiveAttrib(program, GLuint(i), sizeof(name) - 1, &nameLen, &size, &type, name);
        name[nameLen] = 0;

        GLuint location = glGetAttribLocation(program, name);

        string attribName(name);
        attribs[attribName] = Attrib{ attribName, location, type, size };
    }
}

void Program::queryActiveUniforms()
{
    uniforms.clear();

    int total = -1;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &total);
    for (int i = 0; i < total; i++) {
        int nameLen = -1;
        int size = -1;
        GLenum type = GL_ZERO;
        char name[100];

        glGetActiveUniform(program, GLuint(i), sizeof(name) - 1, &nameLen, &size, &type, name);
        name[nameLen] = 0;

        GLuint location = glGetUniformLocation(program, name);

        string uniformName(name);
        uniforms[uniformName] = Uniform{ uniformName, location, type, size };
    }
}

const char* Program::convertGlslShaderType(GLuint type) {
    switch (type) {
        case GL_VERTEX_SHADER: return "vertex";
#ifdef GL_GEOMETRY_SHADER
        case GL_GEOMETRY_SHADER: return "geometry";
#endif
        case GL_FRAGMENT_SHADER: return "fragment";
        default: return "";
    }
}

void Program::loadBinary()
{
    /*GLint numProgramBinaryFormats = 123;
    glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &numProgramBinaryFormats);
    
    if (numProgramBinaryFormats > 0) {
        vector<GLint> formats(numProgramBinaryFormats);
        glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, formats.data());
    }

    GLint numShaderBinaryFormats = 123;
    glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &numShaderBinaryFormats);
    
    if (numShaderBinaryFormats > 0) {
        vector<GLint> formats(numShaderBinaryFormats);
        glGetIntegerv(GL_SHADER_BINARY_FORMATS, formats.data());
    }
    
    if (numProgramBinaryFormats > 0)
    {
        GLint size;
        glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &size);
        
        binary->data.resize(size);
        
        GLint length = size;
        glGetProgramBinary(program, size, &length, &binary.format, binary.data.data());
    }*/
}

void Program::load(const Source& src)
{
    struct Shaders {
        map<GLuint, GLuint> ids;

        ~Shaders() {
            for (auto id : ids)
                if (id.second) glDeleteShader(id.second);
        }

        void compile(GLuint type, const string& src) {
            try {
                ids[type] = Program::compile(type, src);
            }
            catch (const runtime_error& err) {
                throw runtime_error(string(convertGlslShaderType(type))
                                         + " shader compilation failed: "
                                         + err.what());
            }
        }

        void attach(GLuint program) {
            for (auto id : ids)
                if (id.second) glAttachShader(program, id.second);
        }
    }
    shaders;

    shaders.compile(GL_VERTEX_SHADER, src.vertex);
    shaders.compile(GL_FRAGMENT_SHADER, src.fragment);

	auto id = glCreateProgram();
	if (!id) {
        throw runtime_error("failed to create shader program");
    }

    //glProgramParameteri(id, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);

    shaders.attach(id);

    try {
        link(id);
    }
    catch (const runtime_error& err) {
        LOGW("vertex shader: %s\n", src.vertex.c_str());
        LOGW("fragment shader: %s\n", src.fragment.c_str());
        throw runtime_error(string("failed to link program: ") + err.what());
    }
    LOGD("program linked\n");

    program = id;

    queryActiveAttributes();
    queryActiveUniforms();
}

void Program::bind() {
    glUseProgram(program);
}

bool Program::isVertexAttributeActive(const string& name) {
    return attribs.find(name) != attribs.end();
}

bool Program::isUniformActive(const string& name) {
    return uniforms.find(name) != uniforms.end();
}

unsigned Program::getVertexAttributeLocation(const string& name) {
    return attribs[name].location;
}

unsigned Program::getUniformLocation(const string& name) {
    return uniforms[name].location;
}

void Program::set(const string& var, float value) {
    glUniform1f(getUniformLocation(var), value);
}

void Program::set(const string& var, float v0, float v1) {
    glUniform2f(getUniformLocation(var), v0, v1);
}

void Program::set(const string& var, float v0, float v1, float v2) {
    glUniform3f(getUniformLocation(var), v0, v1, v2);
}

void Program::set(const string& var, float v0, float v1, float v2, float v3) {
    glUniform4f(getUniformLocation(var), v0, v1, v2, v3);
}

void Program::set(const string& var, unsigned size, unsigned count, const float* data)
{
    switch (size) {
        case 1:
            glUniform1fv(getUniformLocation(var), count, data);
        break;

        case 2:
            glUniform2fv(getUniformLocation(var), count, data);
        break;

        case 3:
            glUniform3fv(getUniformLocation(var), count, data);
        break;

        case 4:
            glUniform4fv(getUniformLocation(var), count, data);
        break;

        default: break;
    }
}

void Program::set(const string& var, int value) {
    glUniform1i(getUniformLocation(var), value);
}

void Program::set(const string& var, int v0, int v1) {
    glUniform2i(getUniformLocation(var), v0, v1);
}

void Program::set(const string& var, int v0, int v1, int v2) {
    glUniform3i(getUniformLocation(var), v0, v1, v2);
}

void Program::set(const string& var, int v0, int v1, int v2, int v3) {
    glUniform4i(getUniformLocation(var), v0, v1, v2, v3);
}

void Program::set(const string& var, unsigned size, unsigned count, const int* data)
{
    switch (size) {
        case 1:
            glUniform1iv(getUniformLocation(var), count, data);
        break;

        case 2:
            glUniform2iv(getUniformLocation(var), count, data);
        break;

        case 3:
            glUniform3iv(getUniformLocation(var), count, data);
        break;

        case 4:
            glUniform4iv(getUniformLocation(var), count, data);
        break;

        default: break;
    }
}

void Program::set(const string& var, unsigned value) {
    glUniform1i(getUniformLocation(var), value);
}

void Program::set(const string& var, unsigned v0, unsigned v1) {
    glUniform2i(getUniformLocation(var), v0, v1);
}

void Program::set(const string& var, unsigned v0, unsigned v1, unsigned v2) {
    glUniform3i(getUniformLocation(var), v0, v1, v2);
}

void Program::set(const string& var, unsigned v0, unsigned v1, unsigned v2, unsigned v3) {
    glUniform4i(getUniformLocation(var), v0, v1, v2, v3);
}

void Program::set(const string& var, unsigned size, unsigned count, const unsigned* data)
{
    switch (size) {
        case 1:
            glUniform1uiv(getUniformLocation(var), count, data);
        break;

        case 2:
            glUniform2uiv(getUniformLocation(var), count, data);
        break;

        case 3:
            glUniform3uiv(getUniformLocation(var), count, data);
        break;

        case 4:
            glUniform4uiv(getUniformLocation(var), count, data);
        break;

        default: break;
    }
}

void Program::set(const string& var, const mat2& m, bool transpose) {
    glUniformMatrix2fv(getUniformLocation(var), 1, transpose, value_ptr(m));
}

void Program::set(const string& var, const mat3& m, bool transpose) {
    glUniformMatrix3fv(getUniformLocation(var), 1, transpose, value_ptr(m));
}

void Program::set(const string& var, const mat4& m, bool transpose) {
    glUniformMatrix4fv(getUniformLocation(var), 1, transpose, value_ptr(m));
}

void Program::set(const string& var, const mat2x3& m, bool transpose) {
    glUniformMatrix2x3fv(getUniformLocation(var), 1, transpose, value_ptr(m));
}

void Program::set(const string& var, const mat3x2& m, bool transpose) {
    glUniformMatrix3x2fv(getUniformLocation(var), 1, transpose, value_ptr(m));
}

void Program::set(const string& var, const mat2x4& m, bool transpose) {
    glUniformMatrix2x4fv(getUniformLocation(var), 1, transpose, value_ptr(m));
}

void Program::set(const string& var, const mat4x2& m, bool transpose) {
    glUniformMatrix4x2fv(getUniformLocation(var), 1, transpose, value_ptr(m));
}

void Program::set(const string& var, const mat3x4& m, bool transpose) {
    glUniformMatrix3x4fv(getUniformLocation(var), 1, transpose, value_ptr(m));
}

void Program::set(const string& var, const mat4x3& m, bool transpose) {
    glUniformMatrix4x3fv(getUniformLocation(var), 1, transpose, value_ptr(m));
}

void Program::set(const string& var, unsigned count, const mat2* m, bool transpose) {
//    glUniformMatrix2fv(getUniformLocation(var), count, transpose, value_ptr(m));
}

void Program::set(const string& var, unsigned count, const mat3* m, bool transpose) {
//    glUniformMatrix3fv(getUniformLocation(var), count, transpose, value_ptr(m));
}

void Program::set(const string& var, unsigned count, const mat4* m, bool transpose) {
//    glUniformMatrix4fv(getUniformLocation(var), count, transpose, value_ptr(m));
}

void Program::set(const string& var, unsigned count, const mat2x3* m, bool transpose) {
//    glUniformMatrix2x3fv(getUniformLocation(var), count, transpose, value_ptr(m));
}

void Program::set(const string& var, unsigned count, const mat3x2* m, bool transpose) {
//    glUniformMatrix3x2fv(getUniformLocation(var), count, transpose, value_ptr(m));
}

void Program::set(const string& var, unsigned count, const mat2x4* m, bool transpose) {
//    glUniformMatrix2x4fv(getUniformLocation(var), count, transpose, value_ptr(m));
}

void Program::set(const string& var, unsigned count, const mat4x2* m, bool transpose) {
//    glUniformMatrix4x2fv(getUniformLocation(var), count, transpose, value_ptr(m));
}

void Program::set(const string& var, unsigned count, const mat3x4* m, bool transpose) {
//    glUniformMatrix3x4fv(getUniformLocation(var), count, transpose, value_ptr(m));
}

void Program::set(const string& var, unsigned count, const mat4x3* m, bool transpose) {
//    glUniformMatrix4x3fv(getUniformLocation(var), count, transpose, value_ptr(m));
}
