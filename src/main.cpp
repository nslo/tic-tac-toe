#include <iostream>
#include "game.h"
#include "display.h"
#include "initialization.h"
#include "util.h"

int main()
{
    // Create OpenGL context and initialize GLEW.
    size_t window_width = 1000, window_height = 1000;
    GLFWwindow *window = initialize_glfw(window_width, window_height);
    display_opengl_version(window);
    initialize_glew();

    // read, load, compile the shaders
    GLuint shader =
        create_shader_program("../shd/vert.glsl", "../shd/frag.glsl");

    // Use a Vertex Array Object.
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    Game game(window_width, window_height);

    int player_2_turn = 0;

    // Main loop.
    while (!glfwWindowShouldClose(window)) {
        glfwWaitEvents();
        // glfwPollEvents();

        // Check if the left button was just released
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) ==
            GLFW_RELEASE) {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            // not sure if I can trust glfw to not report clicks outside window
            if (x >= window_width || x < 0 || y >= window_height || y < 0)
                continue;
            size_t square = Game::get_square_from_cursor_pos(
                window_width, window_height, x, y);
            if (!game.ValidSquare(square)) continue;

            if (player_2_turn) {
                game.AddO(square);
            } else {
                game.AddX(square);
            }

        } else {
            // All these continues are gross
            continue;
        }

        display(window, game, shader);

        Game::Winner winner = game.CheckWinner();

        if (winner == Game::Winner::player1) {
            std::cout << "Player 1 wins!" << std::endl;
            break;
        } else if (winner == Game::Winner::player2) {
            std::cout << "Player 2 wins!" << std::endl;
            break;
        } else if (winner == Game::Winner::tie) {
            std::cout << "It's a tie" << std::endl;
            break;
        }
        // else game not over yet
        player_2_turn = (player_2_turn + 1) % 2;
    }

    // TODO wait for a mouse or keyboard event here?

    glfwTerminate();
    return 0;
}
