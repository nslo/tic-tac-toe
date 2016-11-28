#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "coord.h"
#include "game.h"

void display(GLFWwindow* window, const Game& game, const GLuint& shader);
template <typename T>
void draw(const T& object, const GLuint& shader, GLenum draw_mode,
          GLenum usage);
template <typename T>
void bind_shader(const T& object, const GLuint& shader);
