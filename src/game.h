#pragma once

#include <cstddef>
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "coord.h"

// Screen coordinates start at (0, 0) at top left of screen.
// OpenGL coordinates range from (-1.0, -1.0) to (1.0, 1.0).
using ScreenCoord = Coord<size_t>; // A pixel position on screen.
using GLCoord = Coord<GLfloat>;

class Object
{
public:
    virtual const std::vector<GLfloat>& VertexPositions() const = 0;
    virtual const std::vector<GLfloat>& VertexColors() const = 0;
    virtual const GLuint& VBO() const = 0;
};

void populate_colors(std::vector<GLfloat>& vertex_colors,
                     const std::vector<GLfloat>& colors, size_t num_vertices);

class Grid final : public Object
{
public:
    Grid(size_t width, size_t height, size_t cell_width, size_t cell_height,
         const std::vector<GLfloat>& color);
    virtual const std::vector<GLfloat>& VertexPositions() const override;
    virtual const std::vector<GLfloat>& VertexColors() const override;
    virtual const GLuint& VBO() const override;

private:
    std::vector<GLfloat> vertex_positions_;
    std::vector<GLfloat> vertex_colors_;
    GLuint vbo_;
    size_t window_width_;
    size_t window_height_;
    size_t cell_width_;
    size_t cell_height_;
};

// We'll just stuff all the X and O vertices into a container in the game and
// add to it as we go.  Feels a little lazy but...
class Game final : public Object
{
public:
    static GLCoord ScreenToGL(const ScreenCoord& screen, size_t window_width,
                              size_t window_height);

    // returns 0-8, numbered from top left in reading order
    static size_t GetSquareFromCursor(size_t window_width, size_t window_height,
                                      double x, double y);

    enum class Winner { none, tie, player1, player2 };
    enum class BoardContent { empty, X, O };

    Game(size_t window_width, size_t window_height);
    bool ValidSquare(size_t square);
    void AddO(size_t square);
    void AddX(size_t square);
    Winner CheckWinner();
    void NextTurn();

    const std::vector<BoardContent>& Board() const
    {
        return board_;
    };
    const Grid& Grid() const
    {
        return grid_;
    };

    void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    void MouseButtonCallback(GLFWwindow* window, int button, int action,
                             int mods);

    virtual const std::vector<GLfloat>& VertexPositions() const override;
    virtual const std::vector<GLfloat>& VertexColors() const override;
    virtual const GLuint& VBO() const override;

private:
    void GetSquareCenter(size_t square, size_t& x, size_t& y);

    const size_t window_width_;
    const size_t window_height_;
    class Grid grid_;
    int player_2_turn_;
    std::vector<BoardContent> board_;

    std::vector<GLfloat> vertex_positions_;
    std::vector<GLfloat> vertex_colors_;

    GLuint vbo_;
};
