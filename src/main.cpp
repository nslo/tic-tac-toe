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
    GLuint shader = create_shader_program("./shd/vert.glsl", "./shd/frag.glsl");

    // Use a Vertex Array Object.
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Point glfw to the main game object for callbacks
    Game game(window_width, window_height);
    glfwSetWindowUserPointer(window, &game);

    // Main loop.
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
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

        game.NextTurn();
    }

    // TODO wait for a mouse or keyboard event here?

    glfwTerminate();
    return 0;
}
