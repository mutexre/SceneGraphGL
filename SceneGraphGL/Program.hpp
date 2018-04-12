//
//  Created by mutexre on 11/01/16.
//  Copyright © 2016 mutexre. All rights reserved.
//

namespace SG::GL
{
    class Program : public SG::Program
//                    public Object
    {
    private:
        struct Uniform {
            std::string name;
            GLuint location;
            GLenum type;
            GLint size;
        };

        struct Attrib {
            std::string name;
            GLuint location;
            GLenum type;
            GLint size;
        };

    private:
        GLuint program;
        std::map<std::string, Uniform> uniforms;
        std::map<std::string, Attrib> attribs;

    private:
        static const char* convertGlslShaderType(GLuint);
        static GLuint compile(GLenum type, const std::string& source);
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

        virtual bool isVertexAttributeActive(const std::string&) override;
        virtual bool isUniformActive(const std::string&) override;

        virtual unsigned getVertexAttributeLocation(const std::string& name) override;
        virtual unsigned getUniformLocation(const std::string& name) override;

        virtual void set(const std::string &var, float) override;
        virtual void set(const std::string &var, float, float) override;
        virtual void set(const std::string &var, float, float, float) override;
        virtual void set(const std::string &var, float, float, float, float) override;
        virtual void set(const std::string &var, unsigned size, unsigned count, const float *) override;

        virtual void set(const std::string &var, int) override;
        virtual void set(const std::string &var, int, int) override;
        virtual void set(const std::string &var, int, int, int) override;
        virtual void set(const std::string &var, int, int, int, int) override;
        virtual void set(const std::string &var, unsigned size, unsigned count, const int *) override;

        virtual void set(const std::string &var, unsigned) override;
        virtual void set(const std::string &var, unsigned, unsigned) override;
        virtual void set(const std::string &var, unsigned, unsigned, unsigned) override;
        virtual void set(const std::string &var, unsigned, unsigned, unsigned, unsigned) override;
        virtual void set(const std::string &var, unsigned size, unsigned count, const unsigned *) override;

        virtual void set(const std::string &var, const glm::mat2 &, bool transpose = false) override;
        virtual void set(const std::string &var, const glm::mat3 &, bool transpose = false) override;
        virtual void set(const std::string &var, const glm::mat4 &, bool transpose = false) override;
        virtual void set(const std::string &var, const glm::mat2x3 &, bool transpose = false) override;
        virtual void set(const std::string &var, const glm::mat3x2 &, bool transpose = false) override;
        virtual void set(const std::string &var, const glm::mat2x4 &, bool transpose = false) override;
        virtual void set(const std::string &var, const glm::mat4x2 &, bool transpose = false) override;
        virtual void set(const std::string &var, const glm::mat3x4 &, bool transpose = false) override;
        virtual void set(const std::string &var, const glm::mat4x3 &, bool transpose = false) override;

        virtual void set(const std::string &var, unsigned count, const glm::mat2 *, bool transpose = false) override;
        virtual void set(const std::string &var, unsigned count, const glm::mat3 *, bool transpose = false) override;
        virtual void set(const std::string &var, unsigned count, const glm::mat4 *, bool transpose = false) override;
        virtual void set(const std::string &var, unsigned count, const glm::mat2x3 *, bool transpose = false) override;
        virtual void set(const std::string &var, unsigned count, const glm::mat3x2 *, bool transpose = false) override;
        virtual void set(const std::string &var, unsigned count, const glm::mat2x4 *, bool transpose = false) override;
        virtual void set(const std::string &var, unsigned count, const glm::mat4x2 *, bool transpose = false) override;
        virtual void set(const std::string &var, unsigned count, const glm::mat3x4 *, bool transpose = false) override;
        virtual void set(const std::string &var, unsigned count, const glm::mat4x3 *, bool transpose = false) override;
    };
}
