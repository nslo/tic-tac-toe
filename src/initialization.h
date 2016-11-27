#pragma once

#include "game.h"
#include <iostream>

GLFWwindow *initialize_glfw(size_t window_width, size_t window_height);
void display_opengl_version(GLFWwindow *window);
void initialize_glew();

GLuint create_shader_program(const char *path_vert_shader,
                             const char *path_frag_shader);
GLuint load_and_compile_shader(const char *fname, GLenum shaderType);
void read_shader_src(const char *fname, std::vector<char> &buffer);
