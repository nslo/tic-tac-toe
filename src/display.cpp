#include "display.h"
#include "color.h"

static const GLfloat BackgroundOpacity = 1.0f;

// Render scene.
void display(GLFWwindow* window, const Game& game, const GLuint& shader)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(color::DarkGrey[0],
                 color::DarkGrey[1],
                 color::DarkGrey[2],
                 BackgroundOpacity);
    draw(game.Grid(), shader, GL_LINES, GL_STATIC_DRAW);
    draw(game, shader, GL_LINES, GL_STATIC_DRAW);
    glfwSwapBuffers(window);
}

// TODO can everything up to bind_shader go in constructor?
void draw(const Object& object, const GLuint& shader, GLenum draw_mode,
          GLenum usage)
{
    //// Indices into vbo.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.IBO());
    // glBufferData(
    //    GL_ELEMENT_ARRAY_BUFFER,
    //    static_cast<GLsizeiptr>(sizeof(size_t) * object.Indices().size()),
    //    // static_cast<GLsizeiptr>(sizeof(object.Indices().data())),
    //    object.Indices().data(),
    //    usage);

    // Allocate space for vertex positions and colors
    glBindBuffer(GL_ARRAY_BUFFER, object.VBO());
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(
                     sizeof(GLfloat) * object.VertexPositions().size() +
                     sizeof(GLfloat) * object.VertexColors().size()),
                 nullptr,
                 usage);

    // Transfer the vertex positions and colors.
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    static_cast<GLsizeiptr>(sizeof(GLfloat) *
                                            object.VertexPositions().size()),
                    object.VertexPositions().data());
    glBufferSubData(
        GL_ARRAY_BUFFER,
        static_cast<GLintptr>(sizeof(GLfloat) *
                              object.VertexPositions().size()),
        static_cast<GLsizeiptr>(sizeof(GLfloat) * object.VertexColors().size()),
        object.VertexColors().data());

    bind_shader(object, shader);

    glDrawArrays(draw_mode,
                 0,
                 static_cast<GLsizei>(object.VertexPositions().size() / 2));
    // glDrawElements(draw_mode,
    //                static_cast<GLsizei>(object.Indices().size()),
    //                GL_UNSIGNED_INT,
    //                nullptr);
}

void bind_shader(const Object& object, const GLuint& shader)
{
    // Get locations of position and color attributes in shaders; specify /
    // enable.
    GLuint position_attribute =
        static_cast<GLuint>(glGetAttribLocation(shader, "position"));
    glEnableVertexAttribArray(position_attribute);
    glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint color_attribute =
        static_cast<GLuint>(glGetAttribLocation(shader, "color"));
    glEnableVertexAttribArray(color_attribute);
    glVertexAttribPointer(
        color_attribute,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        reinterpret_cast<GLvoid*>(sizeof(GLfloat) *
                                  object.VertexPositions().size()));

    // Transfer the transformation matrices to the shader program.
    glm::mat4 Model, View, Projection;
    GLint model = glGetUniformLocation(shader, "Model");
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Model));
    GLint view = glGetUniformLocation(shader, "View");
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(View));
    GLint projection = glGetUniformLocation(shader, "Projection");
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(Projection));

    // TODO: compute projection * view * model before sending to vertex shader
}
