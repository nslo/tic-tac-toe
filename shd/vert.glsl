#version 450

in vec4 position;
in vec4 color;
out vec4 color_from_vshader;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
        gl_Position = Projection * View * Model * position;
        color_from_vshader = color;
}
