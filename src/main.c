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
static camera_t *camera = NULL;

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

void mouse_callback(double x, double y) {
    camera_update_view(camera, (vec2s){{ x, y }});
}

void update() {
    float velocity = camera_get_settings(camera).speed * window_get_delta_time();
    vec3s direction = GLMS_VEC3_ZERO_INIT;
    vec3s front = camera_get_front(camera);
    vec3s right = camera_get_right(camera);

    if (input_key_pressed(KEY_W)) {
        direction = glms_vec3_add(direction, glms_vec3_scale(front, velocity));
    } else if (input_key_pressed(KEY_S)) {
        direction = glms_vec3_sub(direction, glms_vec3_scale(front, velocity));
    }

    if (input_key_pressed(KEY_A)) {
        direction = glms_vec3_sub(direction, glms_vec3_scale(right, velocity));
    } else if (input_key_pressed(KEY_D)) {
        direction = glms_vec3_add(direction, glms_vec3_scale(right, velocity));
    }

    camera_translate(camera, glms_vec3_scale(glms_normalize(direction), velocity));
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

    window_settings_t window_settings = {0};
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

    uint32_t vertex_shader = shader_create(SHADER_TYPE_VERTEX, vertex_shader_source);
    free(vertex_shader_source);

    uint32_t fragment_shader = shader_create(SHADER_TYPE_FRAGMENT, fragment_shader_source);
    free(fragment_shader_source);

    shader_program_t shader_program = shader_program_create();
    shader_program_attach_shader(&shader_program, vertex_shader);
    shader_program_attach_shader(&shader_program, fragment_shader);
    shader_program_link(&shader_program);

    shader_destroy(&vertex_shader);
    shader_destroy(&fragment_shader);

    uint32_t uniform_buffer = buffer_create(sizeof(mat4) * 3, NULL, BUFFER_USAGE_STATIC_DRAW, BUFFER_TARGET_UNIFORM_BUFFER);
    buffer_bind_base(uniform_buffer, BUFFER_TARGET_UNIFORM_BUFFER, 0);
    shader_program_bind_uniform_block(&shader_program, "Matrices", 0);

    float max_anisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy);
    LOG_INFO("Max anisotropy: %.2f", max_anisotropy);

    tilemap_t tilemap = {0};
    tilemap_load("assets/tilemaps/default.tilemap", &tilemap);

    image_t tilemap_image = image_load(tilemap.path);
    uint32_t tilemap_texture = texture_create();
    texture_set_image(tilemap_texture, tilemap_image);
    texture_set_wrapping(tilemap_texture, TEXTURE_WRAPPING_REPEAT, TEXTURE_WRAPPING_REPEAT);
    texture_set_filtering(tilemap_texture, TEXTURE_FILTERING_NEAREST_MIPMAP_NEAREST, TEXTURE_FILTERING_NEAREST);
    texture_generate_mipmaps(tilemap_texture);
    texture_set_anisotropy(tilemap_texture, max_anisotropy);
    image_free(tilemap_image);

    mat4s model = GLMS_MAT4_IDENTITY_INIT;
    buffer_sub_data(uniform_buffer, 0, sizeof(mat4), &model);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    chunk_t *chunk = chunk_create(0, 0, &tilemap);
    chunk_generate_mesh(chunk);

    is_running = 1;

    input_set_cursor_enabled(0);
    input_add_key_pressed_callback(key_callback);
    input_add_mouse_position_callback(mouse_callback);

    camera_settings_t camera_settings = {0};
    camera_settings.speed = 5.0f;
    camera_settings.sensitivity = 0.002f;
    camera_settings.fov = 45.0f;
    camera = camera_create(camera_settings);

    camera_set_position(camera, (vec3s){{ 0.0f, 68.0f, 3.0f }});

    mat4s projection = camera_get_perspective(camera);
    buffer_sub_data(uniform_buffer, sizeof(mat4) * 2, sizeof(mat4), &projection);

    while (is_running) {
        window_poll_events();

        update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        if (camera_view_changed(camera)) {
            mat4s view = camera_get_view(camera);
            buffer_sub_data(uniform_buffer, sizeof(mat4), sizeof(mat4), &view);
            camera_view_reset(camera);
        }

        texture_bind(tilemap_texture, 0);
        chunk_render(chunk, &shader_program);

        window_swap_buffers();
        window_update_delta_time();

        is_running &= !window_should_close();
    }

    camera_destroy(camera);
    buffer_destroy(&uniform_buffer);
    shader_program_destroy(&shader_program);
    texture_destroy(&tilemap_texture);
    chunk_destroy(chunk);
    tilemap_free(&tilemap);

    window_deinit();

    fclose(log_fp);
    return 0;
}
