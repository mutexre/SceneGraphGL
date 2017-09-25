//
//  Created by mutexre on 11/01/16.
//  Copyright © 2016 mutexre. All rights reserved.
//

namespace SG::GL
{
    class Program : public SG::Program,
                    public Object
    {
    private:
        struct Uniform {
            string name;
            GLuint location;
            GLenum type;
            GLint size;
        };

        struct Attrib {
            string name;
            GLuint location;
            GLenum type;
            GLint size;
        };

    private:
        GLuint program;
        map<string, Uniform> uniforms;
        map<string, Attrib> attribs;

    private:
        static const char* convertGlslShaderType(GLuint);
        static GLuint compile(GLenum type, const string& source);
        static void link(GLuint id);

        void queryActiveAttributes();
        void queryActiveUniforms();
        virtual void loadBinary() override;

    public:
        Program();
        Program(const Source&);
        Program(const Program&);
        virtual ~Program();

        void load(const Source& src);
        Program clone();

        virtual void bind() override;

        virtual bool isVertexAttributeActive(const string&) override;
        virtual bool isUniformActive(const string&) override;

        virtual unsigned getVertexAttributeLocation(const string& name) override;
        virtual unsigned getUniformLocation(const string& name) override;

        virtual void set(const string &var, float) override;
        virtual void set(const string &var, float, float) override;
        virtual void set(const string &var, float, float, float) override;
        virtual void set(const string &var, float, float, float, float) override;
        virtual void set(const string &var, unsigned size, unsigned count, const float *) override;

        virtual void set(const string &var, int) override;
        virtual void set(const string &var, int, int) override;
        virtual void set(const string &var, int, int, int) override;
        virtual void set(const string &var, int, int, int, int) override;
        virtual void set(const string &var, unsigned size, unsigned count, const int *) override;

        virtual void set(const string &var, unsigned) override;
        virtual void set(const string &var, unsigned, unsigned) override;
        virtual void set(const string &var, unsigned, unsigned, unsigned) override;
        virtual void set(const string &var, unsigned, unsigned, unsigned, unsigned) override;
        virtual void set(const string &var, unsigned size, unsigned count, const unsigned *) override;

        virtual void set(const string &var, const mat2 &, bool transpose = false) override;
        virtual void set(const string &var, const mat3 &, bool transpose = false) override;
        virtual void set(const string &var, const mat4 &, bool transpose = false) override;
        virtual void set(const string &var, const mat2x3 &, bool transpose = false) override;
        virtual void set(const string &var, const mat3x2 &, bool transpose = false) override;
        virtual void set(const string &var, const mat2x4 &, bool transpose = false) override;
        virtual void set(const string &var, const mat4x2 &, bool transpose = false) override;
        virtual void set(const string &var, const mat3x4 &, bool transpose = false) override;
        virtual void set(const string &var, const mat4x3 &, bool transpose = false) override;

        virtual void set(const string &var, unsigned count, const mat2 *, bool transpose = false) override;
        virtual void set(const string &var, unsigned count, const mat3 *, bool transpose = false) override;
        virtual void set(const string &var, unsigned count, const mat4 *, bool transpose = false) override;
        virtual void set(const string &var, unsigned count, const mat2x3 *, bool transpose = false) override;
        virtual void set(const string &var, unsigned count, const mat3x2 *, bool transpose = false) override;
        virtual void set(const string &var, unsigned count, const mat2x4 *, bool transpose = false) override;
        virtual void set(const string &var, unsigned count, const mat4x2 *, bool transpose = false) override;
        virtual void set(const string &var, unsigned count, const mat3x4 *, bool transpose = false) override;
        virtual void set(const string &var, unsigned count, const mat4x3 *, bool transpose = false) override;
    };
}
