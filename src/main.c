#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "core/log.h"
#include "core/input.h"
#include "core/file.h"

#include "graphics/window.h"
#include "graphics/buffer.h"
#include "graphics/vertex_array.h"
#include "graphics/shader.h"
#include "graphics/shader_program.h"
#include "graphics/camera.h"

#define EXECUTABLE_NAME "Cubescape"

#define VERTEX_SHADER_PATH "assets/shaders/main.vs"
#define FRAGMENT_SHADER_PATH "assets/shaders/main.fs"

int main(int argc, char **argv) {
    set_log_level(LOG_TRACE);

    log_info("%s starting up...", EXECUTABLE_NAME);

    create_window(800, 600, EXECUTABLE_NAME);

    init_input_manager();

    // Vertex data for a cube
    float vertices[] = {
        // positions
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,   // front face
        4, 5, 6, 6, 7, 4,   // back face
        0, 1, 5, 5, 4, 0,   // bottom face
        2, 3, 7, 7, 6, 2,   // top face
        0, 3, 7, 7, 4, 0,   // left face
        1, 2, 6, 6, 5, 1    // right face
    };

    uint32_t vertex_array = create_vertex_array();

    uint32_t vertex_buffer = create_buffer(vertices, sizeof(vertices), BUFFER_USAGE_STATIC_DRAW, BUFFER_TARGET_ARRAY_BUFFER);
    uint32_t element_buffer = create_buffer(indices, sizeof(indices), BUFFER_USAGE_STATIC_DRAW, BUFFER_TARGET_ELEMENT_ARRAY_BUFFER);

    bind_vertex_array(vertex_array);
    bind_buffer(vertex_buffer, BUFFER_TARGET_ARRAY_BUFFER);
    bind_buffer(element_buffer, BUFFER_TARGET_ELEMENT_ARRAY_BUFFER);

    vertex_array_attrib(0, 3, VERTEX_ARRAY_DATA_TYPE_FLOAT, 3 * sizeof(float), (void*)0);

    FILE *fp = fopen(VERTEX_SHADER_PATH, "r");
    if (!fp) {
        log_fatal("Failed to open file: %s", VERTEX_SHADER_PATH);
        return 1;
    }

    size_t vertex_shader_size = get_file_size(fp);
    char *vertex_shader_source = malloc(vertex_shader_size);
    read_file_content(fp, vertex_shader_source, vertex_shader_size);
    fclose(fp);

    fp = fopen(FRAGMENT_SHADER_PATH, "r");
    if (!fp) {
        log_fatal("Failed to open file: %s", FRAGMENT_SHADER_PATH);
        return 1;
    }

    size_t fragment_shader_size = get_file_size(fp);
    char *fragment_shader_source = malloc(fragment_shader_size);
    read_file_content(fp, fragment_shader_source, fragment_shader_size);
    fclose(fp);

    uint32_t vertex_shader = create_shader(SHADER_TYPE_VERTEX, vertex_shader_source);
    free(vertex_shader_source);

    uint32_t fragment_shader = create_shader(SHADER_TYPE_FRAGMENT, fragment_shader_source);
    free(fragment_shader_source);

    ShaderProgram shader_program = create_shader_program();
    attach_shader(&shader_program, vertex_shader);
    attach_shader(&shader_program, fragment_shader);
    link_shader_program(&shader_program);

    destroy_shader(&vertex_shader);
    destroy_shader(&fragment_shader);

    uint32_t uniform_buffer = create_buffer(NULL, sizeof(mat4) * 3, BUFFER_USAGE_STATIC_DRAW, BUFFER_TARGET_UNIFORM_BUFFER);
    bind_buffer_base(uniform_buffer, BUFFER_TARGET_UNIFORM_BUFFER, 0);
    bind_uniform_block(&shader_program, "Matrices", 0);

    CameraSettings camera_settings = {0};
    camera_settings.speed = 2.5f;
    camera_settings.sensitivity = 0.002f;
    camera_settings.fov = 45.0f;
    camera_init((vec3){ 0.0f, 0.0f, 3.0f }, camera_settings, uniform_buffer);

    mat4 projection, model;
    camera_get_perpective(projection);
    glm_mat4_identity(model);

    buffer_sub_data(uniform_buffer, &model, sizeof(mat4), 0, BUFFER_TARGET_UNIFORM_BUFFER);
    buffer_sub_data(uniform_buffer, &projection, sizeof(mat4), sizeof(mat4) * 2, BUFFER_TARGET_UNIFORM_BUFFER);

    while (!window_should_close()) {
        poll_events();

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        use_shader_program(&shader_program);

        camera_update_position();

        bind_vertex_array(vertex_array);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        if (key_pressed(KEY_ESCAPE)) {
            break;
        }

        swap_buffers();
        update_delta_time();
    }

    destroy_vertex_array(&vertex_array);
    destroy_buffer(&vertex_buffer);
    destroy_buffer(&element_buffer);
    destroy_buffer(&uniform_buffer);
    destroy_shader_program(&shader_program);

    destroy_window();
    return 0;
}