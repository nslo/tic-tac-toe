#include <iostream>
#include <random>
#include <unordered_map>
#include "color.h"
#include "game.h"

GLCoord Game::screen_to_gl(const ScreenCoord& screen, size_t window_width,
                           size_t window_height)
{
    // 2 comes from OpenGL coordinate range going from -1.0 to 1.0
    return GLCoord{static_cast<GLfloat>(screen.x) /
                           static_cast<GLfloat>(window_width / 2) -
                       1,
                   static_cast<GLfloat>(screen.y) /
                           static_cast<GLfloat>(window_height / 2) -
                       1};
}

// returns 0-8, numbered from top left in reading order
size_t Game::get_square_from_cursor_pos(size_t window_width,
                                        size_t window_height, double x,
                                        double y)
{
    // TODO
}

Game::Game(size_t window_width, size_t window_height)
    : window_width_(window_width)
    , window_height_(window_height)
    , grid_(window_width_, window_height_, window_width / 3, window_height / 3,
            color::LightGrey)
{
    board_.reserve(8);
    for (auto& square : board_) square = Game::BoardContent::empty;

    glGenBuffers(1, &vbo_);
}

bool Game::ValidSquare(size_t square)
{
    if (board_[square] == Game::BoardContent::empty)
        return true;
    else
        return false;
}

void Game::AddO(size_t square)
{
    board_[square] = Game::BoardContent::O;
    // TODO: calculate the positions and add them to vertex_positions_
    populate_colors(vertex_colors_, color::White, vertex_positions_.size() / 2);
}

void Game::AddX(size_t square)
{
    board_[square] = Game::BoardContent::X;
    // TODO: calculate the positions and add them to vertex_positions_
    populate_colors(vertex_colors_, color::White, vertex_positions_.size() / 2);
}

Game::Winner Game::CheckWinner()
{
    // TODO
}

void populate_colors(std::vector<GLfloat>& vertex_colors,
                     const std::vector<GLfloat>& colors, size_t num_vertices)
{
    // For every vertex (two elements of position vector) push all three color
    // elements. i.e. one RGB color per vertex.
    for (size_t i = 0; i < num_vertices; ++i) {
        vertex_colors.push_back(colors[0]);
        vertex_colors.push_back(colors[1]);
        vertex_colors.push_back(colors[2]);
    }
}

const std::vector<GLfloat>& Game::VertexPositions() const
{
    return vertex_positions_;
}
const std::vector<GLfloat>& Game::VertexColors() const
{
    return vertex_colors_;
}
const GLuint& Game::VBO() const
{
    return vbo_;
}

Grid::Grid(size_t width, size_t height, size_t cell_width, size_t cell_height,
           const std::vector<GLfloat>& color)
    : window_width_(width)
    , window_height_(height)
    , cell_width_(cell_width)
    , cell_height_(cell_height)
{
    auto get_grid_edges = [this](const ScreenCoord& screen) -> void {
        GLCoord gl = Game::screen_to_gl(screen, window_width_, window_height_);
        vertex_positions_.push_back(gl.x);
        vertex_positions_.push_back(gl.y);
    };

    // Just need to store the extremities of the grid to draw it.
    for (size_t x = 0; x < window_width_; x += cell_width_) {
        get_grid_edges(ScreenCoord(x, 0));
        get_grid_edges(ScreenCoord(x, window_height_));
    }

    for (size_t y = 0; y < window_height_; y += cell_height_) {
        get_grid_edges(ScreenCoord(0, y));
        get_grid_edges(ScreenCoord(window_width_, y));
    }

    populate_colors(vertex_colors_, color, vertex_positions_.size() / 2);

    glGenBuffers(1, &vbo_);
}

const std::vector<GLfloat>& Grid::VertexPositions() const
{
    return vertex_positions_;
}
const std::vector<GLfloat>& Grid::VertexColors() const
{
    return vertex_colors_;
}
const GLuint& Grid::VBO() const
{
    return vbo_;
}
