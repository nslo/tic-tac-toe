#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "coord.h"
#include "game.h"

void display(GLFWwindow* window, const Game& game, const GLuint& shader);
void draw(const Object& object, const GLuint& shader, GLenum draw_mode,
          GLenum usage);
void bind_shader(const Object& object, const GLuint& shader);
