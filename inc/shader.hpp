
#pragma once

#include "api.hpp"
#include "error.hpp"
#include "print.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

// Path to shader directory
// const std::string shader_folder_path = "~/Nextcloud/Projects/Art/quadric-engine/shaders/";
const std::string shader_folder_path = "../shaders/";

// Common shader file
const std::string vert_common_filename = "common.vert";
const std::string frag_common_filename = "common.frag";

class Shader
{
public:
    unsigned int ID;

    Shader(std::string vert_filename,
           std::string frag_filename)
    {
        // Open files
        std::string vert_code = load_shader_file(vert_filename);
        std::string frag_code = load_shader_file(frag_filename);

        // Caution! Include guards are not used!
        // This could end in an endless loop if two files include each other
        // Max includes set to 10 to prevent this
        // TODO increase robustness
        for (int i = 0; i < 10; i++)
        {
            find_and_replace_include_statement(vert_code);
            find_and_replace_include_statement(frag_code);
        }

        // Create and compile shaders
        unsigned int vertex_shader, fragment_shader;
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

        const char *vertex_code_c = vert_code.c_str();
        glShaderSource(vertex_shader, 1, &vertex_code_c, NULL);
        glCompileShader(vertex_shader);
        check_compilation_error(vertex_shader);

        const char *fragment_code_c = frag_code.c_str();
        glShaderSource(fragment_shader, 1, &fragment_code_c, NULL);
        glCompileShader(fragment_shader);
        check_compilation_error(fragment_shader);

        // Shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex_shader);
        glAttachShader(ID, fragment_shader);
        glLinkProgram(ID);
        check_compilation_error(ID);

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    // Activate the shader
    void use() const
    {
        glUseProgram(ID);
    }

private:
    // Shader Parsing ----------------------------------------------------------

    bool find_and_replace_include_statement(std::string &shader_code)
    {
        // Look for "#include"
        std::regex re_pattern_for_include(R"(#include)");
        std::smatch re_match_for_include;

        if (!std::regex_search(shader_code, re_match_for_include, re_pattern_for_include))
        {
            return false;
        }

        // Get name of included file
        std::regex re_pattern_for_file(R"(#include \"([a-zA-Z0-9\.\-_]+)\")");
        std::smatch re_match_for_file;

        if (!std::regex_search(shader_code, re_match_for_file, re_pattern_for_file))
        {
            throw quad::fatal_error("Failed to parse name of included file in shader",
                                    "find_and_replace_include_statement()");
        }

        // Load included file
        std::string included_file = load_shader_file(re_match_for_file[1]);

        shader_code =
            shader_code.substr(0, re_match_for_file.position(0)) +
            included_file +
            shader_code.substr(re_match_for_file.position(0) + re_match_for_file.length(0));

        return true;
    }

    /**
     * Loads file located inside shader directory
     * @param  filename  Name of file
     * @return String with file contents
     */
    std::string load_shader_file(std::string filename)
    {
        // Open files
        std::ifstream file_stream;

        file_stream.open(shader_folder_path + filename);

        if (!file_stream.is_open())
        {
            throw quad::fatal_error("Could not open or read shader file",
                                    "process_shader_files()",
                                    "Filename: " + shader_folder_path + filename);
        }

        // Reading to string
        std::stringstream file_stringstream;
        file_stringstream << file_stream.rdbuf();

        file_stream.close();

        return file_stringstream.str();
    }

    // Unused, needed for OpenGL ES and WebGL compatibility
    void set_mobile()
    {
        // GLSL version handling
        //         size_t start_pos, end_pos = 0;
        // #ifdef OPENGL_ES
        //         start_pos = vertex_code.find("#version", 0);
        //         end_pos = vertex_code.find("\n", start_pos);
        //         vertex_code.insert(end_pos, "#define OPENGL_ES\n");
        //         vertex_code.erase(start_pos, end_pos - start_pos);
        //         vertex_code.insert(start_pos, "#version 300 es\n");

        //         start_pos = fragment_code.find("#version", 0);
        //         end_pos = fragment_code.find("\n", start_pos);
        //         fragment_code.insert(end_pos, "#define OPENGL_ES\n");
        //         fragment_code.erase(start_pos, end_pos - start_pos);
        //         fragment_code.insert(start_pos, "#version 300 es\n");
        // #else
        //         // finding and eliminating OPENGL_ES definition
        //         start_pos = vertex_code.find("#define OPENGL_ES", 0);
        //         if (start_pos != std::string::npos)
        //         {
        //             end_pos = vertex_code.find("\n", start_pos);
        //             vertex_code.erase(start_pos, end_pos - start_pos);
        //         }
        //         // insertig correct version string
        //         start_pos = vertex_code.find("#version", 0);
        //         end_pos = vertex_code.find("\n", start_pos);
        //         vertex_code.erase(start_pos, end_pos - start_pos);
        //         vertex_code.insert(start_pos, "#version 330 core\n");

        //         // finding and eliminating OPENGL_ES definition
        //         start_pos = fragment_code.find("#define OPENGL_ES", 0);
        //         if (start_pos != std::string::npos)
        //         {
        //             end_pos = fragment_code.find("\n", start_pos);
        //             fragment_code.erase(start_pos, end_pos - start_pos);
        //         }
        //         // insertig correct version string
        //         start_pos = fragment_code.find("#version", 0);
        //         end_pos = fragment_code.find("\n", start_pos);
        //         fragment_code.erase(start_pos, end_pos - start_pos);
        //         fragment_code.insert(start_pos, "#version 330 core\n");
        // #endif
    }

    void check_compilation_error(unsigned int shader_id)
    {
        int status;

        if (glIsShader(shader_id))
        {
            glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
            if (status == GL_FALSE)
            {
                int shader_type;
                int info_log_length;
                std::string info_log;

                glGetShaderiv(shader_id, GL_SHADER_TYPE, &shader_type);
                glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

                info_log.resize(info_log_length);
                glGetShaderInfoLog(shader_id, info_log_length, NULL, info_log.data());

                std::string error_msg = "Failed to compile ";
                if (shader_type == GL_VERTEX_SHADER)
                    error_msg += "vertex shader";
                else if (shader_type == GL_FRAGMENT_SHADER)
                    error_msg += "fragment shader";
                else
                    error_msg += "shader of unknown type";

                throw quad::fatal_error(error_msg,
                                        "check_compilation_error()",
                                        info_log);
            }
        }
        else if (glIsProgram(shader_id))
        {
            glGetProgramiv(shader_id, GL_LINK_STATUS, &status);
            if (status == GL_FALSE)
            {
                int info_log_length;
                std::string info_log;

                glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

                info_log.resize(info_log_length);
                glGetProgramInfoLog(shader_id, info_log_length, NULL, info_log.data());

                throw quad::fatal_error("Failed to link program",
                                        "check_compilation_error",
                                        info_log);
            }
        }
        else
        {
            throw quad::fatal_error("Specified shader_id is not a shader nor a program",
                                    "check_compilation_error");
        }
    }

public:
    // Set Uniform Functions ---------------------------------------------------

    void set_bool(const std::string &name, bool value) const
    {
        this->use();
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void set_int(const std::string &name, int value) const
    {
        this->use();
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void set_float(const std::string &name, float value) const
    {
        this->use();
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void set_vec2(const std::string &name, glm::vec2 vec) const
    {
        this->use();
        glUniform2f(glGetUniformLocation(ID, name.c_str()), vec[0], vec[1]);
    }

    void set_vec3(const std::string &name, glm::vec3 vec) const
    {
        this->use();
        glUniform3f(glGetUniformLocation(ID, name.c_str()), vec[0], vec[1], vec[2]);
    }

    void set_vec4(const std::string &name, glm::vec4 vec) const
    {
        this->use();
        glUniform4f(glGetUniformLocation(ID, name.c_str()), vec[0], vec[1], vec[2], vec[3]);
    }

    void set_mat4(const std::string &name, glm::mat4 mat) const
    {
        this->use();
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void set_time(double time) const
    {
        this->use();
        set_float("time", (float)time);
    }
};
