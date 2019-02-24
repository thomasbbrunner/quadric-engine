#ifndef SHADER_H
#define SHADER_H

#include <common.h>

class Shader
{
  public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char *vertex_path = "./shaders/std.vert", const char *fragment_path = "./shaders/std.frag")
    {
        // Retrieve code from files
        std::string vertex_code;
        std::string fragment_code;
        std::string geometry_code;

        std::ifstream vertex_file;
        std::ifstream fragment_file;

        std::ifstream vertex_std;
        std::ifstream fragment_std;
        std::ifstream geometry_std;

        // ensure ifstream objects can throw exceptions
        vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        vertex_std.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragment_std.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        geometry_std.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            std::stringstream vertex_stream, fragment_stream;

            vertex_file.open(vertex_path);
            vertex_std.open("./shaders/common.vert");
            vertex_stream << vertex_std.rdbuf() << vertex_file.rdbuf();
            vertex_file.close();
            vertex_std.close();
            vertex_code = vertex_stream.str();

            fragment_file.open(fragment_path);
            fragment_std.open("./shaders/common.frag");
            fragment_stream << fragment_std.rdbuf() << fragment_file.rdbuf();
            fragment_file.close();
            fragment_std.close();
            fragment_code = fragment_stream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        // GLSL version handling
        size_t start_pos, end_pos = 0;
#ifdef OPENGL_ES
        start_pos = vertex_code.find("#version", 0);
        end_pos = vertex_code.find("\n", start_pos);
        vertex_code.insert(end_pos, "#define OPENGL_ES\n");
        vertex_code.erase(start_pos, end_pos - start_pos);
        vertex_code.insert(start_pos, "#version 300 es\n");

        start_pos = fragment_code.find("#version", 0);
        end_pos = fragment_code.find("\n", start_pos);
        fragment_code.insert(end_pos, "#define OPENGL_ES\n");
        fragment_code.erase(start_pos, end_pos - start_pos);
        fragment_code.insert(start_pos, "#version 300 es\n");
#else
        // finding and eliminating OPENGL_ES definition
        start_pos = vertex_code.find("#define OPENGL_ES", 0);
        if (start_pos != std::string::npos)
        {
            end_pos = vertex_code.find("\n", start_pos);
            vertex_code.erase(start_pos, end_pos-start_pos);
        }
        // insertig correct version string
        start_pos = vertex_code.find("#version", 0);
        end_pos = vertex_code.find("\n", start_pos);
        vertex_code.erase(start_pos, end_pos - start_pos);
        vertex_code.insert(start_pos, "#version 330 core\n");

        // finding and eliminating OPENGL_ES definition
        start_pos = fragment_code.find("#define OPENGL_ES", 0);
        if (start_pos != std::string::npos)
        {
            end_pos = fragment_code.find("\n", start_pos);
            fragment_code.erase(start_pos, end_pos-start_pos);
        }
        // insertig correct version string
        start_pos = fragment_code.find("#version", 0);
        end_pos = fragment_code.find("\n", start_pos);
        fragment_code.erase(start_pos, end_pos - start_pos);
        fragment_code.insert(start_pos, "#version 330 core\n");
#endif

        // DEBUG - print shader
        // printf("%s", vertex_code.c_str());

        // Compile shaders
        unsigned int vertex_shader, fragment_shader;

        // Vertex shader
        const char *vertex_code_c = vertex_code.c_str();
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_code_c, NULL);
        glCompileShader(vertex_shader);
        check_error(vertex_shader, "VERTEX");

        // Fragment Shader
        const char *fragment_code_c = fragment_code.c_str();
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_code_c, NULL);
        glCompileShader(fragment_shader);
        check_error(fragment_shader, "FRAGMENT");

        // Shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex_shader);
        glAttachShader(ID, fragment_shader);

        glLinkProgram(ID);
        check_error(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use() const
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void set_bool(const std::string &name, bool value) const
    {
        use();
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void set_int(const std::string &name, int value) const
    {
        use();
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void set_float(const std::string &name, float value) const
    {
        use();
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void set_vec2(const std::string &name, glm::vec2 vec) const
    {
        use();
        glUniform2f(glGetUniformLocation(ID, name.c_str()), vec[0], vec[1]);
    }
    // ------------------------------------------------------------------------
    void set_vec3(const std::string &name, glm::vec3 vec) const
    {
        use();
        glUniform3f(glGetUniformLocation(ID, name.c_str()), vec[0], vec[1], vec[2]);
    }
    // ------------------------------------------------------------------------
    void set_vec4(const std::string &name, glm::vec4 vec) const
    {
        use();
        glUniform4f(glGetUniformLocation(ID, name.c_str()), vec[0], vec[1], vec[2], vec[3]);
    }
    // ------------------------------------------------------------------------
    void set_mat4(const std::string &name, glm::mat4 mat) const
    {
        use();
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
    // ------------------------------------------------------------------------
    void update_time(double time) const
    {
        use();
        glUniform1f(glGetUniformLocation(ID, "time"), (float)time);
    }

  private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void check_error(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                exit(0);
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                exit(0);
            }
        }
    }
};

#endif