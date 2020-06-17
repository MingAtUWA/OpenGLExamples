#ifndef __OpenGL_Shader_Utilities_h__
#define __OpenGL_Shader_Utilities_h__

#include <string>
#include <iostream>

#include <GLAD/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class OpenGLShaderProgram;

class OpenGLShaderSupportCheck
{
protected:
    GLint major_version;
    GLint minor_version;

    bool geometry_shader_supported;
    bool tessellation_shader_supported;
    bool compute_shader_supported;

public:
    OpenGLShaderSupportCheck();
    ~OpenGLShaderSupportCheck() {}

    void get_version(GLint& ma_ver, GLint& mi_ver) const
    { ma_ver = major_version; mi_ver = minor_version; }
    inline bool support_geometry_shader() const { return geometry_shader_supported; }
    inline bool support_tessellation_shader() const { return tessellation_shader_supported; }
    inline bool support_compute_shader() const { return compute_shader_supported; }
};

class OpenGLShader
{
    friend class OpenGLShaderProgram;
public:
    enum ShaderType
    {
        Invalid = -1,
        Vertex = 0,
        Fragment = 1,
        Geometry = 2,
        TessellationControl = 3,
        TessellationEvaluation = 4,
        Compute = 5
    };

protected:
    static const size_t type_num;
    static const char* type_names[];
    static const GLenum type_gl_macros[];

    ShaderType type;
    GLuint shader_id;
    bool compiled_already;
    std::string log;

public:
    explicit OpenGLShader();
    ~OpenGLShader();

    ShaderType get_type() const { return type; }
    inline GLuint get_id() const { return shader_id; }
    inline const char* get_type_name() const
    {
        return type < type_num ? type_names[type] : "";
    }
    inline bool is_compiled() const { return compiled_already; }
    inline const std::string get_log() const { return log; }
    void get_code(std::string& code) const;
    
    inline bool create(ShaderType _type)
    {
        if (shader_id)
            return true;
        type = _type;
        if (type < 0 || type >= type_num)
        {
            std::cout << "OpenGLShader: Unsupported shader type.\n";
            return false;
        }
        shader_id = glCreateShader(type_gl_macros[type]);
        if (!shader_id)
        {
            std::cout << "OpenGLShader: Can't create shader.\n";
            return false;
        }
        return true;
    }

    bool compile_code(const char* code, ShaderType type);
    inline bool OpenGLShader::compile_code(const std::string& code, ShaderType type)
    { return compile_code(code.c_str(), type); }

    bool compile_file(const char *filename, ShaderType type);
    inline bool compile_file(const std::string& filename, ShaderType type)
    { return compile_file(filename.c_str(), type); }

protected: // compile and set log (if fails)
    bool compile(const char* code);

private: // no copy
    OpenGLShader(const OpenGLShader& other) = delete;
    OpenGLShader &operator=(const OpenGLShader& other) = delete;
};


class OpenGLShaderProgram
{
protected:
    GLuint program_id;
    bool linked_already;
    std::string log;

    // shaders
    struct ShaderPointer
    {
        OpenGLShader *shader;
        // whether the shader is allocated on heap
        bool is_internal;
        inline void init() { shader = nullptr; is_internal = false; }
        inline void clear()
        {
            if (shader)
            {
                if (is_internal)
                    delete shader;
                shader = nullptr;
                is_internal = false;
            }
        }
        inline bool is_valid()
        {
            return shader && shader->get_id() &&
                   shader->is_compiled();
        }
        inline GLuint get_id() { return shader->get_id(); }
    };

    union
    {
        struct
        {
            ShaderPointer shader_vert;
            ShaderPointer shader_frag;
            ShaderPointer shader_geo;
            ShaderPointer shader_tc; // Tessellation control
            ShaderPointer shader_eva; // Tessellation evaluation
            ShaderPointer shader_comp; // Compute
        };
        ShaderPointer shader_ptr[6];
    };

public:
    typedef OpenGLShader::ShaderType ShaderType;

    explicit OpenGLShaderProgram();
    ~OpenGLShaderProgram();

    inline bool create()
    {
        if (program_id)
            return true;

        program_id = glCreateProgram();
        if (!program_id)
        {
            std::cout << "OpenGLShaderProgram: Cannot create shader program.\n";
            return false;
        }

        return true;
    }
    // Initialize vertex and fragment shaders from files
    bool create(const char* shd_vert_filename,
                const char* shd_frag_filename);

    inline GLuint get_id() const { return program_id; }
    inline bool is_linked() const { return linked_already; };
    inline const std::string &get_log() const { return log; }

    bool add_shader(OpenGLShader &shader);
    bool add_shader_from_code(ShaderType type, const char* code);
    inline bool add_shader_from_code(ShaderType type, const std::string &code)
    { return add_shader_from_code(type, code.c_str()); }
    bool add_shader_from_file(ShaderType type, const char *filename);
    inline bool add_shader_from_file(ShaderType type, const std::string &filename)
    { return add_shader_from_file(type, filename.c_str()); }
    
    inline void del_shader(ShaderType type)
    {
        if (type > 0 && type < OpenGLShader::type_num)
            shader_ptr[type].clear();
    }
    inline void del_all_shaders()
    {
        shader_vert.clear();
        shader_frag.clear();
        shader_geo.clear();
        shader_tc.clear();
        shader_eva.clear();
        shader_comp.clear();
    }
    
    bool link();

    inline bool use()
    {
        if (program_id && linked_already)
        {
            glUseProgram(program_id);
            return true;
        }
        return false;
    }
    inline void unuse() { glUseProgram(0); }

    // Attribute array pointer
    inline void bind_attr_loc(const char* name, GLint location)
    {
        if (program_id && linked_already)
            glBindAttribLocation(program_id, location, name);
    }
    inline void bind_attr_loc(const std::string &name, GLint location)
    {
        bind_attr_loc(name.c_str(), location);
    }

    inline int attr_loc(const char* name) const
    {
        if (program_id && linked_already)
            return glGetAttribLocation(program_id, name);
        return -1;
    }
    inline int attr_loc(const std::string& name) const
    {
        return attr_loc(name.c_str());
    }

    inline void set_attr(GLint location, GLfloat value)
    {
        glVertexAttrib1fv(location, &value);
    }
    inline void set_attr(GLint location, GLfloat x, GLfloat y)
    {
        GLfloat values[2] = { x, y };
        glVertexAttrib2fv(location, values);
    }
    inline void set_attr(GLint location, GLfloat x, GLfloat y, GLfloat z)
    {
        GLfloat values[3] = { x, y, z };
        glVertexAttrib3fv(location, values);
    }
    inline void set_attr(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        GLfloat values[4] = { x, y, z, w };
        glVertexAttrib4fv(location, values);
    }
    inline void set_attr(GLint location, const glm::vec2& value)
    {
        glVertexAttrib2fv(location, glm::value_ptr(value));
    }
    inline void set_attr(GLint location, const glm::vec3& value)
    {
        glVertexAttrib3fv(location, glm::value_ptr(value));
    }
    inline void set_attr(GLint location, const glm::vec4& value)
    {
        glVertexAttrib4fv(location, glm::value_ptr(value));
    }

    inline void set_attr(const char* name, GLfloat value)
    {
        GLint loc = attr_loc(name);
        if (loc != -1)
            set_attr(loc, value);
    }
    inline void set_attr(const char* name, GLfloat x, GLfloat y)
    {
        GLint loc = attr_loc(name);
        if (loc != -1)
            set_attr(loc, x, y);
    }
    inline void set_attr(const char* name, GLfloat x, GLfloat y, GLfloat z)
    {
        GLint loc = attr_loc(name);
        if (loc != -1)
            set_attr(loc, x, y, z);
    }
    inline void set_attr(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        GLint loc = attr_loc(name);
        if (loc != -1)
            set_attr(loc, x, y, z, w);
    }
    inline void set_attr(const char* name, const glm::vec2& value)
    {
        GLint loc = attr_loc(name);
        if (loc != -1)
            set_attr(loc, value);
    }
    inline void set_attr(const char* name, const glm::vec3& value)
    {
        GLint loc = attr_loc(name);
        if (loc != -1)
            set_attr(loc, value);
    }
    inline void set_attr(const char* name, const glm::vec4& value)
    {
        GLint loc = attr_loc(name);
        if (loc != -1)
            set_attr(loc, value);
    }

    inline void set_attrs(GLint location, const GLfloat* values, GLsizei vec_dim, GLsizei stride)
    {
        glVertexAttribPointer(location, vec_dim, GL_FLOAT, GL_FALSE, stride, values);
    }
    inline void set_attrs(GLint location, const glm::vec2* values, GLsizei stride)
    {
        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, stride, values);
    }
    inline void set_attrs(GLint location, const glm::vec3* values, GLsizei stride)
    {
        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, stride, values);
    }
    inline void set_attrs(GLint location, const glm::vec4* values, GLsizei stride)
    {
        glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, stride, values);
    }
    inline void set_attrs(GLint location, GLenum type, const void* values, GLsizei vec_dim, GLsizei stride)
    {
        glVertexAttribPointer(location, vec_dim, type, GL_TRUE, stride, values);
    }

    inline void set_attrs(const char* name, const GLfloat* values, GLsizei vec_dim, GLsizei stride)
    {
        GLint loc = attr_loc(name);
        if (loc != -1)
            set_attrs(loc, values, vec_dim, stride);
    }
    inline void set_attrs(const char* name, const glm::vec2* values, GLsizei stride)
    {
        GLint loc = attr_loc(name);
        if (loc != -1)
            set_attrs(loc, values, stride);
    }
    inline void set_attrs(const char* name, const glm::vec3* values, GLsizei stride)
    {
        GLint loc = attr_loc(name);
        if (loc != -1)
            set_attrs(loc, values, stride);
    }
    inline void set_attrs(const char* name, const glm::vec4* values, GLsizei stride)
    {
        GLint loc = attr_loc(name);
        if (loc != -1)
            set_attrs(loc, values, stride);
    }
    inline void set_attrs(const char* name, GLenum type, const void* values, GLsizei vec_dim, GLsizei stride)
    {
        GLint loc = attr_loc(name);
        if (loc != -1)
            set_attrs(loc, type, values, vec_dim, stride);
    }
    inline void set_obj_buf_attrs(GLint location, GLenum type, size_t offset, GLint data_size, GLsizei stride)
    {
        glVertexAttribPointer(location, data_size, type, GL_TRUE, stride, (GLvoid *)offset);
    }
    inline void set_attr_buf(const char* name, GLenum type, size_t offset, GLint data_size, GLsizei stride)
    {
        GLint loc = attr_loc(name);
        if (loc != -1)
            set_obj_buf_attrs(loc, type, offset, data_size, stride);
    }

    inline void enable_attrs(GLint location) { glEnableVertexAttribArray(location); }
    inline void disable_attrs(GLint location) { glDisableVertexAttribArray(location); }

    inline void enable_attrs(const char* name)
    {
        GLint loc = attr_loc(name);
        if (loc != -1)
            enable_attrs(loc);
    }
    inline void disable_attrs(const char* name)
    {
        GLint loc = attr_loc(name);
        if (loc != -1)
            disable_attrs(loc);
    }
    
    // Uniform variables
    inline int uniform_loc(const char* name) const
    {
        if (program_id && linked_already)
            return glGetUniformLocation(program_id, name);
        return -1;
    }
    inline int uniform_loc(const std::string &name) const
    {
        uniform_loc(name.c_str());
    }

    inline void set_uniform(GLint location, GLfloat value)
    {
        glUniform1fv(location, 1, &value);
    }
    inline void set_uniform(GLint location, GLint value)
    {
        glUniform1i(location, value);
    }
    inline void set_uniform(GLint location, GLuint value)
    {
        glUniform1ui(location, value);
    }
    inline void set_uniform(GLint location, GLfloat x, GLfloat y)
    {
        GLfloat values[2] = { x, y };
        glUniform2fv(location, 1, values);
    }
    inline void set_uniform(GLint location, GLfloat x, GLfloat y, GLfloat z)
    {
        GLfloat values[3] = { x, y, z };
        glUniform3fv(location, 1, values);
    }
    inline void set_uniform(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        GLfloat values[4] = { x, y, z, w };
        glUniform4fv(location, 1, values);
    }
    inline void set_uniform(GLint location, const glm::vec2& value)
    {
        glUniform2fv(location, 1, glm::value_ptr(value));
    }
    inline void set_uniform(GLint location, const glm::vec3 &value)
    {
        glUniform3fv(location, 1, glm::value_ptr(value));
    }
    inline void set_uniform(GLint location, const glm::vec4& value)
    {
        glUniform4fv(location, 1, glm::value_ptr(value));
    }
    inline void set_uniform(GLint location, const glm::mat2x2 &value)
    {
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
    // Treat vec3[2] as mat2x3
    inline void set_uniform(GLint location, const glm::mat2x3& value)
    {
        glUniform3fv(location, 2, glm::value_ptr(value));
    }
    // Treat vec4[2] as mat2x4
    inline void set_uniform(GLint location, const glm::mat2x4& value)
    {
        glUniform4fv(location, 2, glm::value_ptr(value));
    }
    // Treat vec2[3] as mat3x2
    inline void set_uniform(GLint location, const glm::mat3x2& value)
    {
        glUniform2fv(location, 3, glm::value_ptr(value));
    }
    inline void set_uniform(GLint location, const glm::mat3x3& value)
    {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
    // Treat vec3[4] as mat3x3
    inline void set_uniform(GLint location, const glm::mat3x4& value)
    {
        glUniform4fv(location, 3, glm::value_ptr(value));
    }
    // Treat vec2[4] as mat4x2
    inline void set_uniform(GLint location, const glm::mat4x2& value)
    {
        glUniform2fv(location, 4, glm::value_ptr(value));
    }
    // Treat vec3[4] as mat4x3
    inline void set_uniform(GLint location, const glm::mat4x3& value)
    {
        glUniform3fv(location, 4, glm::value_ptr(value));
    }
    inline void set_uniform(GLint location, const glm::mat4x4& value)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
    inline void set_uniform(GLint location, const GLfloat value[2][2])
    {
        glUniformMatrix2fv(location, 1, GL_FALSE, value[0]);
    }
    inline void set_uniform(GLint location, const GLfloat value[3][3])
    {
        glUniformMatrix3fv(location, 1, GL_FALSE, value[0]);
    }
    inline void set_uniform(GLint location, const GLfloat value[4][4])
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, value[0]);
    }

    inline void set_uniform(const char* name, GLfloat value)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, GLint value)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, GLuint value)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, GLfloat x, GLfloat y)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, x, y);
    }
    inline void set_uniform(const char* name, GLfloat x, GLfloat y, GLfloat z)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, x, y, z);
    }
    inline void set_uniform(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, x, y, z, w);
    }
    inline void set_uniform(const char* name, const glm::vec2& value)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, const glm::vec3& value)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, const glm::vec4& value)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, const glm::mat2x2& value)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, const glm::mat2x3& value)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, const glm::mat2x4& value)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, const glm::mat3x2& value)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, const glm::mat3x3& value)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, const glm::mat3x4& value)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, const glm::mat4x2& value)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, const glm::mat4x3& value)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, const glm::mat4x4& value)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, const GLfloat value[2][2])
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, const GLfloat value[3][3])
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniform(const char* name, const GLfloat value[4][4])
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniform(loc, value);
    }
    inline void set_uniforms(GLint location, const GLfloat* values, GLsizei count, GLsizei vec_dim)
    {
        switch (vec_dim)
        {
        case 1:
            glUniform1fv(location, count, values);
            break;
        case 2:
            glUniform2fv(location, count, values);
            break;
        case 3:
            glUniform3fv(location, count, values);
            break;
        case 4:
            glUniform4fv(location, count, values);
            break;
        default:
            std::cout << "OpenGLShaderProgram: vec"<< vec_dim << " not supported\n";
            break;
        }
    }
    inline void set_uniforms(GLint location, const GLint* values, GLsizei count)
    {
        glUniform1iv(location, count, values);
    }
    inline void set_uniforms(GLint location, const GLuint* values, GLsizei count)
    {
        glUniform1uiv(location, count, values);
    }
    inline void set_uniforms(GLint location, const glm::vec2* values, GLsizei count)
    {
        glUniform2fv(location, count, glm::value_ptr(values[0]));
    }
    inline void set_uniforms(GLint location, const glm::vec3* values, GLsizei count)
    {
        glUniform3fv(location, count, glm::value_ptr(values[0]));
    }
    inline void set_uniforms(GLint location, const glm::vec4* values, GLsizei count)
    {
        glUniform4fv(location, count, glm::value_ptr(values[0]));
    }
    inline void set_uniforms(GLint location, const glm::mat2x2* values, GLsizei count)
    {
        glUniformMatrix2fv(location, count, GL_FALSE, glm::value_ptr(values[0]));
    }
    inline void set_uniforms(GLint location, const glm::mat2x3* values, GLsizei count)
    {
        glUniform3fv(location, count*2, glm::value_ptr(values[0]));
    }
    inline void set_uniforms(GLint location, const glm::mat2x4* values, GLsizei count)
    {
        glUniform4fv(location, count*2, glm::value_ptr(values[0]));
    }
    inline void set_uniforms(GLint location, const glm::mat3x2* values, GLsizei count)
    {
        glUniform2fv(location, count*3, glm::value_ptr(values[0]));
    }
    inline void set_uniforms(GLint location, const glm::mat3x3* values, GLsizei count)
    {
        glUniformMatrix3fv(location, count, GL_FALSE, glm::value_ptr(values[0]));
    }
    inline void set_uniforms(GLint location, const glm::mat3x4* values, GLsizei count)
    {
        glUniform4fv(location, count*3, glm::value_ptr(values[0]));
    }
    inline void set_uniforms(GLint location, const glm::mat4x2* values, GLsizei count)
    {
        glUniform2fv(location, count*4, glm::value_ptr(values[0]));
    }
    inline void set_uniforms(GLint location, const glm::mat4x3* values, GLsizei count)
    {
        glUniform3fv(location, count*4, glm::value_ptr(values[0]));
    }
    inline void set_uniforms(GLint location, const glm::mat4x4* values, GLsizei count)
    {
        glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(values[0]));
    }

    inline void set_uniforms(const char* name, const GLfloat* values, GLsizei count, GLsizei vec_dim)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniforms(loc, values, count, vec_dim);
    }
    inline void set_uniforms(const char* name, const GLint* values, GLsizei count)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniforms(loc, values, count);
    }
    inline void set_uniforms(const char* name, const GLuint* values, GLsizei count)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniforms(loc, values, count);
    }
    inline void set_uniforms(const char* name, const glm::vec2* values, GLsizei count)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniforms(loc, values, count);
    }
    inline void set_uniforms(const char* name, const glm::vec3* values, GLsizei count)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniforms(loc, values, count);
    }
    inline void set_uniforms(const char* name, const glm::vec4* values, GLsizei count)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniforms(loc, values, count);
    }
    inline void set_uniforms(const char* name, const glm::mat2x2* values, GLsizei count)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniforms(loc, values, count);
    }
    inline void set_uniforms(const char* name, const glm::mat2x3* values, GLsizei count)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniforms(loc, values, count);
    }
    inline void set_uniforms(const char* name, const glm::mat2x4* values, GLsizei count)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniforms(loc, values, count);
    }
    inline void set_uniforms(const char* name, const glm::mat3x2* values, GLsizei count)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniforms(loc, values, count);
    }
    inline void set_uniforms(const char* name, const glm::mat3x3* values, GLsizei count)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniforms(loc, values, count);
    }
    inline void set_uniforms(const char* name, const glm::mat3x4* values, GLsizei count)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniforms(loc, values, count);
    }
    inline void set_uniforms(const char* name, const glm::mat4x2* values, GLsizei count)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniforms(loc, values, count);
    }
    inline void set_uniforms(const char* name, const glm::mat4x3* values, GLsizei count)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniforms(loc, values, count);
    }
    inline void set_uniforms(const char* name, const glm::mat4x4* values, GLsizei count)
    {
        GLint loc = uniform_loc(name);
        if (loc != -1)
            set_uniforms(loc, values, count);
    }

public:
    // Geometry shader properties
    inline GLint get_max_geometry_output_vertices() const
    {
        GLint num;
        glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES, &num);
        return num;
    }

    inline void set_patch_vertex_count(GLint count)
    {
        glPatchParameteri(GL_PATCH_VERTICES, count);
    }
    inline int get_patch_vertex_count() const
    {
        GLint num;
        glGetIntegerv(GL_PATCH_VERTICES, &num);
        return num;
    }

    // Tesselleation properties
    void set_default_outer_tessellation_levels(const GLfloat levels[4])
    {
        glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, levels);
    }
    void get_default_outer_tessellation_levels(GLfloat levels[4]) const
    {
        glGetFloatv(GL_PATCH_DEFAULT_OUTER_LEVEL, levels);
    }
    inline void set_default_inner_tessellation_levels(const GLfloat levels[2])
    {
        glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, levels);
    }
    inline void get_default_inner_tessellation_levels(GLfloat levels[2]) const
    {
        glGetFloatv(GL_PATCH_DEFAULT_INNER_LEVEL, levels);
    }

private: // no copy
    OpenGLShaderProgram(const OpenGLShaderProgram& other) = delete;
    OpenGLShaderProgram& operator=(const OpenGLShaderProgram& other) = delete;
};

#endif