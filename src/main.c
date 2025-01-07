#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/log.h"
#include "graphics/window.h"
#include "graphics/buffer.h"
#include "graphics/vertex_array.h"
#include "graphics/shader.h"
#include "graphics/shader_program.h"

#define EXECUTABLE_NAME "Cubescape"

const char* vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

const char* fragment_shader_source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
    "}\n\0";

int main(int argc, char **argv) {
    log_info("%s starting up...", EXECUTABLE_NAME);

    create_window(800, 600, EXECUTABLE_NAME);

    // Vertex data for a quad
    float vertices[] = {
        // positions
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,   // first triangle
        2, 3, 0    // second triangle
    };

    uint32_t vertex_array = create_vertex_array();

    uint32_t vertex_buffer = create_buffer(vertices, sizeof(vertices), BUFFER_USAGE_STATIC_DRAW, BUFFER_TARGET_ARRAY_BUFFER);
    uint32_t element_buffer = create_buffer(indices, sizeof(indices), BUFFER_USAGE_STATIC_DRAW, BUFFER_TARGET_ELEMENT_ARRAY_BUFFER);

    bind_vertex_array(vertex_array);
    bind_buffer(vertex_buffer, BUFFER_TARGET_ARRAY_BUFFER);
    bind_buffer(element_buffer, BUFFER_TARGET_ELEMENT_ARRAY_BUFFER);

    vertex_array_attrib(0, 3, VERTEX_ARRAY_DATA_TYPE_FLOAT, 3 * sizeof(float), (void*)0);

    uint32_t vertex_shader = create_shader(SHADER_TYPE_VERTEX, vertex_shader_source);
    uint32_t fragment_shader = create_shader(SHADER_TYPE_FRAGMENT, fragment_shader_source);

    ShaderProgram shader_program = create_shader_program();
    attach_shader(&shader_program, vertex_shader);
    attach_shader(&shader_program, fragment_shader);
    link_shader_program(&shader_program);

    destroy_shader(&vertex_shader);
    destroy_shader(&fragment_shader);

    while (!window_should_close()) {
        poll_events();

        // Render
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the quad
        use_shader_program(&shader_program);
        bind_vertex_array(vertex_array);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        swap_buffers();
    }

    // Cleanup
    destroy_vertex_array(&vertex_array);
    destroy_buffer(&vertex_buffer);
    destroy_buffer(&element_buffer);
    destroy_shader_program(&shader_program);

    destroy_window();
    return 0;
}