#include <stdio.h>

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
#include "graphics/renderer.h"

#include "world/chunk.h"
#include "world/world.h"

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
        renderer_get_state()->wireframe = !renderer_get_state()->wireframe;
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
    logger_set_level(LOG_LEVEL_INFO);

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

    shader_program_t *shader_program = shader_program_create();
    shader_program_attach_shader(shader_program, vertex_shader);
    shader_program_attach_shader(shader_program, fragment_shader);
    shader_program_link(shader_program);

    shader_destroy(&vertex_shader);
    shader_destroy(&fragment_shader);

    tilemap_t tilemap = {0};
    tilemap_load("assets/tilemaps/default.tilemap", &tilemap);

    image_t *tilemap_image = image_load(tilemap.path);
    uint32_t tilemap_texture = texture_create();
    texture_set_image(tilemap_texture, tilemap_image);
    texture_set_wrapping(tilemap_texture, TEXTURE_WRAPPING_REPEAT, TEXTURE_WRAPPING_REPEAT);
    texture_set_filtering(tilemap_texture, TEXTURE_FILTERING_NEAREST_MIPMAP_NEAREST, TEXTURE_FILTERING_NEAREST);
    texture_generate_mipmaps(tilemap_texture);
    texture_set_anisotropy(tilemap_texture, renderer_get_state()->max_anisotropy);
    image_free(tilemap_image);

    is_running = 1;

    camera_settings_t camera_settings = {0};
    camera_settings.speed = 5.0f;
    camera_settings.sensitivity = 0.002f;
    camera_settings.fov = 45.0f;

    renderer_settings_t renderer_settings = {0};
    renderer_settings.clear_color = (vec3s){{ 0.2f, 0.3f, 0.3f }};
    renderer_settings.camera_settings = camera_settings;
    renderer_init(renderer_settings);

    camera = renderer_get_camera();
    camera_set_position(camera, (vec3s){{ 10.0f, 68.0f, 10.0f }});

    input_set_cursor_enabled(0);
    input_add_key_pressed_callback(key_callback);
    input_add_mouse_position_callback(mouse_callback);

    world_t *world = world_create(&tilemap, tilemap_texture, shader_program);
    int draw_distance = 6;

    while (is_running) {
        window_poll_events();

        update();

        renderer_begin_frame();
           
        for (size_t i = 0; i < WORLD_SIZE * WORLD_SIZE; ++i) {
            chunk_t *chunk = world->chunks[i];
            vec3s position = (vec3s){{ chunk->x * CHUNK_SIZE, 0.0f, chunk->y * CHUNK_SIZE }};
            float distance = glms_vec3_distance(camera_get_position(camera), position);
            if (distance < draw_distance * CHUNK_SIZE) {
                renderer_draw_mesh(chunk->mesh, position, GLMS_VEC3_ZERO, GLMS_VEC3_ONE);
            }
        }

        renderer_end_frame();
        window_update_delta_time();

        is_running &= !window_should_close();
    }

    world_destroy(world);
    shader_program_destroy(shader_program);
    texture_destroy(&tilemap_texture);
    tilemap_free(&tilemap);

    renderer_deinit();
    window_deinit();

    fclose(log_fp);
    return 0;
}
