#include <fstream>
#include <vector>

#include "OpenGLShaderUtilities.h"

OpenGLShaderSupportCheck::OpenGLShaderSupportCheck() :
    major_version(0), minor_version(0),
    geometry_shader_supported(false),
    tessellation_shader_supported(false),
    compute_shader_supported(false)
{
    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    glGetIntegerv(GL_MINOR_VERSION, &minor_version);

    // Geometry shader needs version > 3.2
    if (major_version > 3 && minor_version > 2)
        geometry_shader_supported = true;
    // Tessellation shader needs version > 4.0
    if (major_version > 4 && minor_version > 0)
        tessellation_shader_supported = true;
    // Compute shader needs version > 4.3
    if (major_version > 4 && minor_version > 3)
        compute_shader_supported = true;
}

// ===================== OpenGL Shader Clas =====================
const char* OpenGLShader::type_names[] =
{
    "Fragment",
    "Vertex",
    "Geometry",
    "Tessellation Control",
    "Tessellation Evaluation",
    "Compute"
};

const GLenum OpenGLShader::type_gl_macros[] =
{
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER,
    GL_GEOMETRY_SHADER,
    GL_TESS_CONTROL_SHADER,
    GL_TESS_EVALUATION_SHADER,
    GL_COMPUTE_SHADER
};

const size_t OpenGLShader::type_num
    = sizeof(OpenGLShader::type_names) / sizeof(OpenGLShader::type_names[0]);

OpenGLShader::OpenGLShader() :
    type(Invalid), shader_id(0),
    compiled_already(false), log("")
{

}

OpenGLShader::~OpenGLShader()
{
    if (shader_id)
    {
        glDeleteShader(shader_id);
        shader_id = 0;
    }
}

void OpenGLShader::get_code(std::string& code) const
{
    if (!shader_id)
    {
        code = "";
        return;
    }

    GLint code_len = 0;
    glGetShaderiv(shader_id, GL_SHADER_SOURCE_LENGTH, &code_len);
    if (code_len > 0)
    {
        std::vector<char> code_buffer(code_len+1);
        glGetShaderSource(shader_id, code_len, nullptr, &code_buffer[0]);
        code_buffer[code_len] = '\0';
        return;
    }

    code = "";
}


bool OpenGLShader::compile(const char* code)
{
    if (!code)
    {
        std::cout << "OpenGLShader: code is empty.\n";
        return false;
    }

    glShaderSource(shader_id, 1, &code, nullptr);
    glCompileShader(shader_id);
    // get compilation status
    GLint comp_res = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &comp_res);
    compiled_already = (comp_res != 0);
    // compilation error, output msg
    if (!compiled_already)
    {
        // print compilation info log
        GLint info_log_len = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_len);
        if (info_log_len > 0)
        {
            std::vector<char> log_buffer(info_log_len+1);
            glGetShaderInfoLog(shader_id, info_log_len, nullptr, &log_buffer[0]);
            log_buffer[info_log_len] = '\0';
            log = &log_buffer[0];
            return false;
        }
    }
    return true;
}

bool OpenGLShader::compile_code(const char* code, ShaderType type)
{
    if (!create(type))
        return false;

    compile(code);
    if (!compiled_already)
    {
        std::cout << "OpenGLShader: "
            << get_type_name() << " compilation error.\n"
            << log
            << "\nProblematic shader code:\n"
            << code << "\n";
    }
    std::cout << "OpenGLShader: code is empty.\n";
    return false;
}

bool OpenGLShader::compile_file(const char* filename, ShaderType type)
{
    if (!create(type))
        return false;
    
    if (!filename || strlen(filename) == 0)
    {
        std::cout << "OpenGLShader: Filename is empty.\n";
        return false;
    }

    std::fstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "OpenGLShader: Can't open file " << filename << ".\n";
        return false;
    }
    // get file size
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    // read in code
    std::vector<char> code_buffer(file_size+1);
    file.read(&code_buffer[0], file_size);
    file.close();
    code_buffer[file_size] = '\0';
    compile(&code_buffer[0]);
    if (!compiled_already)
    {
        std::cout << "OpenGLShader: "
            << get_type_name() << " shader compilation error:\n"
            << log
            << "\nShader file path:\n"
            << filename
            << "\nProblematic shader code:\n"
            << &code_buffer[0] << "\n";
        return false;
    }

    return true;
}


// ======================== OpenGL Shader Program ========================
OpenGLShaderProgram::OpenGLShaderProgram():
    program_id(0), linked_already(false), log("")
{
    shader_vert.init();
    shader_frag.init();
    shader_geo.init();
    shader_tc.init();
    shader_eva.init();
    shader_comp.init();
}

OpenGLShaderProgram::~OpenGLShaderProgram()
{
    if (program_id)
    {
        glDeleteProgram(program_id);
        program_id = 0;
    }
    del_all_shaders();
}

bool OpenGLShaderProgram::create(
    const char* shd_vert_filename,
    const char* shd_frag_filename
    )
{
    bool res_create = create();
    bool res_shader1 = add_shader_from_file(ShaderType::Vertex, shd_vert_filename);
    bool res_shader2 = add_shader_from_file(ShaderType::Fragment, shd_frag_filename);
    bool res_link = link();
    return res_create && res_shader1 && res_shader2 && res_link;
}

bool OpenGLShaderProgram::add_shader(OpenGLShader &shader)
{
    ShaderType stype = shader.get_type();
    if (stype < 0 || stype >= OpenGLShader::type_num)
    {
        std::cout << "OpenGLShaderprogram: Unsupported shader type.\n";
        return false;
    }

    ShaderPointer &sptr = shader_ptr[stype];
    sptr.clear();
    sptr.shader = &shader;
    sptr.is_internal = false;
    return true;
}

bool OpenGLShaderProgram::add_shader_from_code(ShaderType type, const char* code)
{
    OpenGLShader* shader = new OpenGLShader;
    if (!shader->compile_code(code, type))
    {
        log = shader->get_log();
        delete shader;
        return false;
    }
    
    ShaderPointer& sptr = shader_ptr[type];
    sptr.clear();
    sptr.shader = shader;
    sptr.is_internal = true;
    return true;
}

bool OpenGLShaderProgram::add_shader_from_file(
    ShaderType type,
    const char* filename
    )
{
    OpenGLShader* shader = new OpenGLShader;
    if (!shader->compile_file(filename, type))
    {
        log = shader->get_log();
        delete shader;
        return false;
    }

    ShaderPointer& sptr = shader_ptr[type];
    sptr.clear();
    sptr.shader = shader;
    sptr.is_internal = true;
    return true;
}

bool OpenGLShaderProgram::link()
{
    if (!create())
        return false;

    if (shader_vert.is_valid())
        glAttachShader(program_id, shader_vert.get_id());
    if (shader_frag.is_valid())
        glAttachShader(program_id, shader_frag.get_id());
    if (shader_geo.is_valid())
        glAttachShader(program_id, shader_geo.get_id());
    if (shader_tc.is_valid())
        glAttachShader(program_id, shader_tc.get_id());
    if (shader_eva.is_valid())
        glAttachShader(program_id, shader_eva.get_id());
    if (shader_comp.is_valid())
        glAttachShader(program_id, shader_comp.get_id());

    glLinkProgram(program_id);
    GLint link_res;
    glGetProgramiv(program_id, GL_LINK_STATUS, &link_res);
    linked_already = (link_res != 0);
    if (!linked_already)
    {
        GLint info_log_len;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_len);
        if (info_log_len > 0)
        {
            std::vector<char> log_buf(info_log_len+1);
            glGetProgramInfoLog(program_id, info_log_len, 0, &log_buf[0]);
            log_buf[info_log_len] = '\0';
            log = &log_buf[0];
        }
        else
            log = "";
        std::cout << "OpenGLShaderProgram: linked error.\n" << log << "\n";
        return false;
    }

    del_all_shaders(); // clear shader if success
    return true;
}
