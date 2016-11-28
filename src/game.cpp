#include <cmath>
#include <iostream>
#include <random>
#include <unordered_map>
#include "color.h"
#include "game.h"

Game::Game(size_t window_width, size_t window_height)
    : window_width_(window_width)
    , window_height_(window_height)
    , grid_(window_width_, window_height_, window_width / 3, window_height / 3,
            color::LightGrey)
    , turn_(Winner::X)
    , winner_(Winner::none)
    , board_(8, BoardContent::empty)
{
    glGenBuffers(1, &vbo_);
}

GLCoord Game::ScreenToGL(const ScreenCoord& screen, size_t window_width,
                         size_t window_height)
{
    // 2 comes from OpenGL coordinate range going from -1.0 to 1.0
    return GLCoord{static_cast<GLfloat>(screen.x) /
                           static_cast<GLfloat>(window_width / 2) -
                       1,
                   -1 * (static_cast<GLfloat>(screen.y) /
                             static_cast<GLfloat>(window_height / 2) -
                         1)};
}

// returns 0-8, numbered from top left in reading order
size_t Game::GetSquareFromCursor(size_t window_width, size_t window_height,
                                 double x, double y)
{
    // Tried to get a neat formula for this but was bothered by edge cases.
    size_t index = 0;
    size_t w = window_width / 3;
    size_t h = window_height / 3;
    if (x >= 0 && x < w) index += 0; // for completeness
    if (x >= w && x < 2 * w) index += 1;
    if (x >= 2 * w && x < window_width) index += 2;
    if (y >= 0 && y < h) index += 0; // for completeness
    if (y >= h && y < 2 * h) index += 3;
    if (y >= 2 * h && y < window_height) index += 6;

    return index;
}

// Handle mouse events
void Game::CursorPositionCallback(GLFWwindow* /*window*/, double /*xpos*/,
                                  double /*ypos*/)
{
    // TODO: highlight squares on mouseover
}

void Game::MouseButtonCallback(GLFWwindow* window, int button, int action,
                               int /*mods*/)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        // not sure if I can trust glfw to not report clicks outside window
        if (x >= window_width_ || x < 0 || y >= window_height_ || y < 0) return;
        size_t square =
            Game::GetSquareFromCursor(window_width_, window_height_, x, y);
        if (!ValidSquare(square)) return;

        if (turn_ == Winner::O) {
            AddO(square);
        } else {
            AddX(square);
        }

        CheckWinner();
        NextTurn();
    }
}

void Game::NextTurn()
{
    if (turn_ == Winner::X)
        turn_ = Winner::O;
    else
        turn_ = Winner::X;
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

    size_t cx, cy;
    GetSquareCenter(square, cx, cy);

    // Number of segments is arbitrary, but make sure it's even.
    // Radius is 35% of the smaller dimension.
    size_t segments = 1000;
    double r = 0.35 * (window_width_ > window_height_ ? window_height_ / 3
                                                      : window_width_ / 3);
    for (size_t i = 0; i < segments; i++) {
        double theta = 2.0f * 3.14159 * i / segments; // current angle
        ScreenCoord coord(static_cast<size_t>(cx + r * std::cos(theta)),
                          static_cast<size_t>(cy + r * std::sin(theta)));
        GLCoord gl = Game::ScreenToGL(coord, window_width_, window_height_);
        vertex_positions_.push_back(gl.x);
        vertex_positions_.push_back(gl.y);
    }

    populate_colors(vertex_colors_, color::White, segments / 2);
}

void Game::AddX(size_t square)
{
    board_[square] = Game::BoardContent::X;

    size_t cx, cy;
    GetSquareCenter(square, cx, cy);

    double r = 0.35 * (window_width_ > window_height_ ? window_height_ / 3
                                                      : window_width_ / 3);

    auto add_coord = [this](size_t x, size_t y) -> void {
        ScreenCoord coord(x, y);
        GLCoord gl = Game::ScreenToGL(coord, window_width_, window_height_);
        vertex_positions_.push_back(gl.x);
        vertex_positions_.push_back(gl.y);
    };
    add_coord(cx + static_cast<size_t>(r), cy + static_cast<size_t>(r));
    add_coord(cx - static_cast<size_t>(r), cy - static_cast<size_t>(r));
    add_coord(cx + static_cast<size_t>(r), cy - static_cast<size_t>(r));
    add_coord(cx - static_cast<size_t>(r), cy + static_cast<size_t>(r));

    populate_colors(vertex_colors_, color::White, 2);
}

void Game::GetSquareCenter(size_t square, size_t& x, size_t& y)
{
    x = ((square % 3) * 2 + 1) * (window_width_ / 6);
    y = ((square / 3) * 2 + 1) * (window_height_ / 6);
}

Game::Winner Game::GetWinner()
{
    return winner_;
}

void Game::CheckWinner()
{
    // Can't think of an elegant way to do this.  Feels bad man.
    // When men were men you could use a goto here.
    bool over = false;
    if ((board_[0] == board_[1] && board_[1] == board_[2] &&
         board_[0] != BoardContent::empty) ||
        (board_[3] == board_[4] && board_[4] == board_[5] &&
         board_[3] != BoardContent::empty) ||
        (board_[6] == board_[7] && board_[7] == board_[8] &&
         board_[6] != BoardContent::empty) ||
        (board_[0] == board_[3] && board_[3] == board_[6] &&
         board_[0] != BoardContent::empty) ||
        (board_[1] == board_[4] && board_[4] == board_[7] &&
         board_[1] != BoardContent::empty) ||
        (board_[2] == board_[5] && board_[5] == board_[8] &&
         board_[2] != BoardContent::empty) ||
        (board_[0] == board_[4] && board_[4] == board_[8] &&
         board_[0] != BoardContent::empty) ||
        (board_[2] == board_[4] && board_[4] == board_[6] &&
         board_[2] != BoardContent::empty)) {
        over = true;
    }

    if (over) {
        // Whoever's turn it just was is the winning player.
        winner_ = turn_;
        return;
    }

    // Check if there are still moves to make.
    for (size_t i = 0; i <= 8; ++i) {
        if (board_[i] == BoardContent::empty) return;
    }

    winner_ = Winner::tie;
}

void Game::populate_colors(std::vector<GLfloat>& vertex_colors,
                           const std::vector<GLfloat>& colors,
                           size_t num_vertices)
{
    // For every vertex (two elements of position vector) push all three color
    // elements. i.e. one RGB color per vertex.
    for (size_t i = 0; i < num_vertices; ++i) {
        vertex_colors.push_back(colors[0]);
        vertex_colors.push_back(colors[1]);
        vertex_colors.push_back(colors[2]);
    }
}

Grid::Grid(size_t width, size_t height, size_t cell_width, size_t cell_height,
           const std::vector<GLfloat>& color)
    : window_width_(width)
    , window_height_(height)
    , cell_width_(cell_width)
    , cell_height_(cell_height)
{
    auto get_grid_edges = [this](const ScreenCoord& screen) -> void {
        GLCoord gl = Game::ScreenToGL(screen, window_width_, window_height_);
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

    Game::populate_colors(vertex_colors_, color, vertex_positions_.size() / 2);

    glGenBuffers(1, &vbo_);
}
