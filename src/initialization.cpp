#include <fstream>
#include <random>
#include "initialization.h"
#include "util.h"

// Handle errors
static void error_callback(int error, const char *description)
{
    std::cerr << "Error " << error << ": " << description << std::endl;
}

// Handle keyboard events
static void key_callback(GLFWwindow *window, int key, int /*scancode*/,
                         int action, int /*mods*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

// Handle mouse events
static void cursor_position_callback(GLFWwindow * /*window*/, double /*xpos*/,
                                     double /*ypos*/)
{
    // TODO: highlight squares on mouseover
}

// TODO remove this?
void mouse_button_callback(GLFWwindow * /*window*/, int button, int action,
                           int /*mods*/)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        // TODO: mark square
    }
}

GLFWwindow *initialize_glfw(size_t window_width, size_t window_height)
{
    // Initialize GLFW.
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    // Use OpenGL 3.1 core profile.  When using an OpenGL version lower than
    // 3.2, ANY rather than CORE must be used.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow *window = glfwCreateWindow(static_cast<int>(window_width),
                                          static_cast<int>(window_height),
                                          "Float",
                                          nullptr,
                                          nullptr);
    if (!window) {
        printf("Failed to open a window.\n");
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    return window;
}

void display_opengl_version(GLFWwindow *window)
{
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    std::cout << "GLFW - " << major << "." << minor << "." << rev << std::endl;
    major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
    std::cout << "OpenGL - " << major << "." << minor << "." << rev
              << std::endl;
}

void initialize_glew()
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}

// Create a program from two shaders
GLuint create_shader_program(const char *path_vert_shader,
                             const char *path_frag_shader)
{
    // Load and compile the vertex and fragment shaders
    GLuint vertex_shader =
        load_and_compile_shader(path_vert_shader, GL_VERTEX_SHADER);
    GLuint fragment_shader =
        load_and_compile_shader(path_frag_shader, GL_FRAGMENT_SHADER);

    // Attach the above shaders to a program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    // Flag the shaders for deletion
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Link and use the program
    glLinkProgram(shader_program);
    glUseProgram(shader_program);

    return shader_program;
}

// Compile a shader
GLuint load_and_compile_shader(const char *fname, GLenum shaderType)
{
    // Load a shader from an external file
    std::vector<char> buffer;
    read_shader_src(fname, buffer);
    const char *src = buffer.data();

    // Compile the shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Check the result of the compilation
    GLint test;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
    if (!test) {
        std::cerr << "Shader compilation failed with this message:"
                  << std::endl;
        std::vector<char> compilation_log(512);
        glGetShaderInfoLog(shader,
                           static_cast<GLsizei>(compilation_log.size()),
                           nullptr,
                           compilation_log.data());
        std::cerr << compilation_log.data() << std::endl;
        glfwTerminate();
        exit(-1);
    }

    return shader;
}

// Read a shader source from a file
// store the shader source in a std::vector<char>
void read_shader_src(const char *fname, std::vector<char> &buffer)
{
    std::ifstream in;
    in.open(fname, std::ios::binary);

    if (in.is_open()) {
        // Get the number of bytes stored in this file
        in.seekg(0, std::ios::end);
        std::streampos length = in.tellg();

        // Go to start of the file
        in.seekg(0, std::ios::beg);

        // Read the content of the file in a buffer
        buffer.resize(
            static_cast<size_t>(length + static_cast<std::streampos>(1)));
        buffer[static_cast<size_t>(length)] = '\0';
        in.read(buffer.data(), length);
        in.close();
    } else {
        std::cerr << "Unable to open " << fname << std::endl;
        exit(-1);
    }
}
