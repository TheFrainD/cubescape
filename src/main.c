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
#include "graphics/image.h"
#include "graphics/texture.h"
#include "graphics/tilemap.h"

#include "world/chunk.h"

#define EXECUTABLE_NAME "Cubescape"

#define VERTEX_SHADER_PATH "assets/shaders/main.vs"
#define FRAGMENT_SHADER_PATH "assets/shaders/main.fs"

#define LOG_FILE EXECUTABLE_NAME ".log"

static int is_running = 0;

void key_callback(key_code_t key) {
    if (key == KEY_ESCAPE) {
        is_running = 0;
    }

    if (key == KEY_F1) {
        static int wireframe = 0;
        if (wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        wireframe = !wireframe;
    }
}

int main(int argc, char **argv) {
    logger_set_level(LOG_LEVEL_TRACE);

    // Log to file
    FILE *log_fp = fopen(LOG_FILE, "w");
    if (log_fp) {
        logger_set_fp(log_fp, LOG_LEVEL_TRACE);
    } else {
        LOG_WARN("Failed to open log file: %s", LOG_FILE);
    }

    LOG_INFO("%s starting up...", EXECUTABLE_NAME);

    WindowSettings window_settings = {0};
    window_settings.width = 800;
    window_settings.height = 600;
    window_settings.title = EXECUTABLE_NAME;
    window_init(window_settings);

    input_init();

    FILE *fp = fopen(VERTEX_SHADER_PATH, "r");
    if (!fp) {
        LOG_FATAL("Failed to open file: %s", VERTEX_SHADER_PATH);
        return 1;
    }

    size_t vertex_shader_size = get_file_size(fp);
    char *vertex_shader_source = malloc(vertex_shader_size);
    read_file_content(fp, vertex_shader_source, vertex_shader_size);
    fclose(fp);

    fp = fopen(FRAGMENT_SHADER_PATH, "r");
    if (!fp) {
        LOG_FATAL("Failed to open file: %s", FRAGMENT_SHADER_PATH);
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
    attach_shader_to_shader_program(&shader_program, vertex_shader);
    attach_shader_to_shader_program(&shader_program, fragment_shader);
    link_shader_program(&shader_program);

    destroy_shader(&vertex_shader);
    destroy_shader(&fragment_shader);

    uint32_t uniform_buffer = create_buffer(sizeof(mat4) * 3, NULL, BUFFER_USAGE_STATIC_DRAW, BUFFER_TARGET_UNIFORM_BUFFER);
    bind_buffer_base(uniform_buffer, BUFFER_TARGET_UNIFORM_BUFFER, 0);
    shader_program_bind_uniform_block(&shader_program, "Matrices", 0);

    float max_anisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);
    LOG_INFO("Max anisotropy: %.2f", max_anisotropy);

    TileMap tilemap = {0};
    load_tilemap("assets/tilemaps/default.tilemap", &tilemap);

    Image tilemap_image = image_load(tilemap.path);
    uint32_t tilemap_texture = create_texture();
    texture_set_image(tilemap_texture, tilemap_image);
    texture_set_wrapping(tilemap_texture, TEXTURE_WRAPPING_REPEAT, TEXTURE_WRAPPING_REPEAT);
    texture_set_filtering(tilemap_texture, TEXTURE_FILTERING_NEAREST_MIPMAP_NEAREST, TEXTURE_FILTERING_NEAREST);
    texture_generate_mipmaps(tilemap_texture);
    texture_set_anisotropy(tilemap_texture, max_anisotropy);
    image_free(tilemap_image);

    CameraSettings camera_settings = {0};
    camera_settings.speed = 5.0f;
    camera_settings.sensitivity = 0.002f;
    camera_settings.fov = 45.0f;
    camera_init((vec3){ 0.0f, 68.0f, 3.0f }, camera_settings, uniform_buffer);

    mat4 projection, model;
    camera_get_perpective(projection);
    glm_mat4_identity(model);

    buffer_sub_data(uniform_buffer, 0, sizeof(mat4), &model);
    buffer_sub_data(uniform_buffer, sizeof(mat4) * 2, sizeof(mat4), &projection);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    Chunk *chunk = create_chunk(0, 0, &tilemap);
    chunk_generate_mesh(chunk);

    is_running = 1;

    input_add_key_pressed_callback(key_callback);

    while (is_running) {
        poll_events();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        camera_update_position();

        bind_texture(tilemap_texture, 0);
        chunk_render(chunk, &shader_program);

        swap_buffers();
        update_delta_time();

        is_running &= !window_should_close();
    }

    destroy_buffer(&uniform_buffer);
    destroy_shader_program(&shader_program);
    destroy_texture(&tilemap_texture);
    destroy_chunk(chunk);
    destroy_tilemap(&tilemap);

    window_destroy();

    fclose(log_fp);
    return 0;
}