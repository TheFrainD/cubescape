#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <cglm/cglm.h>

#include <cubegl/shader.h>
#include <cubegl/shader_program.h>
#include <cubelog/cubelog.h>

#include "core/file.h"
#include "core/input.h"
#include "core/profiling.h"

#include "collections/llist.h"

#include "graphics/camera.h"
#include "graphics/renderer.h"
#include "graphics/tilemap.h"
#include "graphics/window.h"

#include "world/ray.h"
#include "world/world.h"
#include "world/world_renderer.h"

#define VERTEX_SHADER_PATH   "assets/shaders/main.vs"
#define FRAGMENT_SHADER_PATH "assets/shaders/main.fs"

#define CUBELOG_FILE EXECUTABLE_NAME ".log"

static int is_running               = 0;
static camera_t *camera             = NULL;
static world_t *world               = NULL;
static const float horizontal_speed = 7.0f;
static const float vertical_speed   = 5.0f;
static const int draw_distance      = 6;

#define MAX_THREADS 100

struct world_gen_task {
    pthread_t thread;
    chunk_t *chunk;
    bool busy : 1;
};

static struct world_gen_task world_gen_task_pool[MAX_THREADS];

void *world_gen_thread(void *arg) {
    struct world_gen_task *task = arg;
    chunk_t *chunk              = task->chunk;

    CUBELOG_INFO("Generating chunk at position (%d, %d)", chunk->position.x, chunk->position.y);

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int z = 0; z < CHUNK_SIZE; ++z) {
            for (int y = 0; y < CHUNK_HEIGHT; ++y) {
                if (y < 50) {
                    chunk_set_block(chunk, (ivec3s) {{x, y, z}}, BLOCK_ID_STONE);
                } else {
                    chunk_set_block(chunk, (ivec3s) {{x, y, z}}, BLOCK_ID_AIR);
                }
            }
        }
    }

    chunk->flags.generated  = true;
    chunk->flags.generating = false;
    task->busy              = false;
    return NULL;
}

void world_gen_execute(chunk_t *chunk) {
    for (size_t i = 0; i < MAX_THREADS; ++i) {
        struct world_gen_task *task = &world_gen_task_pool[i];
        if (task->busy) {
            continue;
        }
        task->chunk             = chunk;
        task->busy              = true;
        chunk->flags.generating = true;
        pthread_create(&task->thread, NULL, world_gen_thread, task);
        return;
    }
}

struct mesh_gen_task {
    pthread_t thread;
    chunk_t *chunk;
    shader_program_t *shader_program;
    tilemap_t *tilemap;
    bool busy : 1;
};

void *mesh_gen_thread(void *arg) {
    struct mesh_gen_task *task = arg;
    chunk_t *chunk             = task->chunk;

    CUBELOG_INFO("Generating mesh for chunk at position (%d, %d)", chunk->position.x, chunk->position.y);

    chunk_generate_mesh(chunk, task->shader_program, task->tilemap);
    chunk->flags.mesh_generating = false;

    task->busy = false;
    return NULL;
}

static struct mesh_gen_task mesh_gen_task_pool[MAX_THREADS];

void chunk_generate_mesh_async(chunk_t *chunk, shader_program_t *shader_program, tilemap_t *tilemap) {
    for (size_t i = 0; i < MAX_THREADS; ++i) {
        struct mesh_gen_task *task = &mesh_gen_task_pool[i];
        if (task->busy) {
            continue;
        }
        task->chunk                  = chunk;
        task->shader_program         = shader_program;
        task->tilemap                = tilemap;
        task->busy                   = true;
        chunk->flags.mesh_generating = true;
        pthread_create(&task->thread, NULL, mesh_gen_thread, task);
        return;
    }
}

void key_callback(key_code_t key) {
    if (key == KEY_ESCAPE) {
        is_running = 0;
    }

    if (key == KEY_F1) {
        renderer_get_state()->wireframe = !renderer_get_state()->wireframe;
    }
}

void mouse_button_callback(mouse_button_code_t button) {
    ivec3s hit_block = {0};
    ivec3s hit_face  = {0};
    ray_t ray        = {camera_get_position(camera), camera_get_front(camera)};

    if (!ray_cast(world, ray, 30.0f, &hit_block, &hit_face)) {
        return;
    }

    if (input_mouse_button_pressed(MOUSE_BUTTON_LEFT)) {
        world_set_block(world, hit_block, BLOCK_ID_AIR);
    } else if (input_mouse_button_pressed(MOUSE_BUTTON_RIGHT)) {
        world_set_block(world, glms_ivec3_add(hit_block, hit_face), BLOCK_ID_COBBLESTONE);
    }
}

void mouse_callback(double x, double y) { camera_update_view(camera, (vec2s) {{x, y}}); }

void update() {
    float horizontal_velocity = horizontal_speed * window_get_delta_time();

    if (input_key_pressed(KEY_LEFT_CONTROL)) {
        horizontal_velocity *= 1.5f;
    }

    vec3s direction = GLMS_VEC3_ZERO_INIT;
    vec3s front     = camera_get_front(camera);
    vec3s right     = camera_get_right(camera);

    // Zero out the y component to restrict movement to the x and z axes
    front.y = 0.0f;
    right.y = 0.0f;

    if (input_key_pressed(KEY_W)) {
        direction = glms_vec3_add(direction, glms_vec3_scale(front, horizontal_velocity));
    } else if (input_key_pressed(KEY_S)) {
        direction = glms_vec3_sub(direction, glms_vec3_scale(front, horizontal_velocity));
    }

    if (input_key_pressed(KEY_A)) {
        direction = glms_vec3_sub(direction, glms_vec3_scale(right, horizontal_velocity));
    } else if (input_key_pressed(KEY_D)) {
        direction = glms_vec3_add(direction, glms_vec3_scale(right, horizontal_velocity));
    }

    camera_translate(camera, glms_vec3_scale(glms_normalize(direction), horizontal_velocity));

    float vertical_movement = vertical_speed * window_get_delta_time();
    if (input_key_pressed(KEY_SPACE)) {
        camera_translate(camera, (vec3s) {{0.0f, vertical_movement, 0.0f}});
    } else if (input_key_pressed(KEY_LEFT_SHIFT)) {
        camera_translate(camera, (vec3s) {{0.0f, -vertical_movement, 0.0f}});
    }
}

int main(int argc, char **argv) {
    srand(time(NULL));

    cubelog_set_level(CUBELOG_LEVEL_DEBUG);

    memset(world_gen_task_pool, 0, sizeof(world_gen_task_pool));

    // Log to file
    FILE *log_fp = fopen(CUBELOG_FILE, "w");
    if (log_fp) {
        cubelog_set_fp(log_fp, CUBELOG_LEVEL_TRACE);
    } else {
        CUBELOG_WARN("Failed to open log file: %s", CUBELOG_FILE);
    }

    CUBELOG_INFO("%s starting up...", EXECUTABLE_NAME);

    int result = profiling_init();
    if (result) {
        CUBELOG_FATAL("Failed to initialize profiling");
        return 1;
    }

    window_settings_t window_settings = {0};
    window_settings.width             = 800;
    window_settings.height            = 600;
    window_settings.title             = EXECUTABLE_NAME;
    window_settings.multisample       = 4;
    result                            = window_init(window_settings);
    if (result) {
        CUBELOG_FATAL("Failed to initialize window");
        return 1;
    }

    input_init();

    FILE *fp = fopen(VERTEX_SHADER_PATH, "r");
    if (!fp) {
        CUBELOG_FATAL("Failed to open file: %s", VERTEX_SHADER_PATH);
        return 1;
    }

    size_t vertex_shader_size  = get_file_size(fp);
    char *vertex_shader_source = malloc(vertex_shader_size);
    read_file_content(fp, vertex_shader_source, vertex_shader_size);
    fclose(fp);

    fp = fopen(FRAGMENT_SHADER_PATH, "r");
    if (!fp) {
        CUBELOG_FATAL("Failed to open file: %s", FRAGMENT_SHADER_PATH);
        return 1;
    }

    size_t fragment_shader_size  = get_file_size(fp);
    char *fragment_shader_source = malloc(fragment_shader_size);
    read_file_content(fp, fragment_shader_source, fragment_shader_size);
    fclose(fp);

    uint32_t vertex_shader = shader_create(SHADER_TYPE_VERTEX, vertex_shader_source);
    free(vertex_shader_source);

    uint32_t fragment_shader = shader_create(SHADER_TYPE_FRAGMENT, fragment_shader_source);
    free(fragment_shader_source);

    shader_program_t *shader_program = shader_program_create();
    if (!shader_program) {
        CUBELOG_FATAL("Failed to create shader program");
        return 1;
    }

    shader_program_attach_shader(shader_program, vertex_shader);
    shader_program_attach_shader(shader_program, fragment_shader);
    shader_program_link(shader_program);

    shader_destroy(&vertex_shader);
    shader_destroy(&fragment_shader);

    tilemap_t *tilemap = tilemap_load("assets/tilemaps/default.tilemap");
    if (!tilemap) {
        CUBELOG_FATAL("Failed to load tilemap");
        return 1;
    }

    camera_settings_t camera_settings = {0};
    camera_settings.sensitivity       = 0.002f;
    camera_settings.fov               = 45.0f;

    renderer_settings_t renderer_settings = {0};
    renderer_settings.clear_color         = (vec3s) {{0.2f, 0.3f, 0.3f}};
    renderer_settings.camera_settings     = camera_settings;
    renderer_settings.near_clip           = 0.1f;
    renderer_settings.far_clip            = 1000.0f;
    result                                = renderer_init(renderer_settings);
    if (result) {
        CUBELOG_FATAL("Failed to initialize renderer");
        return 1;
    }

    camera = renderer_get_camera();
    camera_set_position(camera, (vec3s) {{10.0f, 68.0f, 10.0f}});

    input_set_cursor_enabled(0);
    input_add_key_pressed_callback(key_callback);
    input_add_mouse_position_callback(mouse_callback);
    input_add_mouse_button_pressed_callback(mouse_button_callback);

    // world_renderer_settings_t world_renderer_settings = {0};
    // world_renderer_settings.tilemap                   = tilemap;
    // world_renderer_settings.block_shader              = shader_program;
    // world_renderer_settings.draw_distance             = 6;
    // world_renderer_t *world_renderer                  = world_renderer_create(world_renderer_settings);
    // if (!world_renderer) {
    //     CUBELOG_FATAL("Failed to create world renderer");
    //     return 1;
    // }

    world_settings_t world_settings = {0};
    world_settings.size             = 1;
    world                           = world_create(world_settings);
    if (!world) {
        CUBELOG_FATAL("Failed to create world");
        return 1;
    }

    // world_generator_parameters_t generator_parameters = {0};
    // generator_parameters.height = 64;
    // generator_parameters.water_level = 32;
    // world_generator_t *generator = world_generator_create(generator_parameters);

    // world_generator_generate(generator, world);

    is_running = true;

    while (is_running) {
        is_running &= !window_should_close();

        window_poll_events();
        window_update_delta_time();
        update();

        // world_renderer_prepare(world_renderer, world);

        renderer_begin_frame();

        vec3s camera_position = camera_get_position(camera);
        ivec2s index          = (ivec2s) {
            {camera_position.x >= 0 ? (camera_position.x / CHUNK_SIZE) : (camera_position.x / CHUNK_SIZE - 1),
             camera_position.z >= 0 ? (camera_position.z / CHUNK_SIZE) : (camera_position.z / CHUNK_SIZE - 1)}};

        for (int x = index.x - draw_distance; x <= index.x + draw_distance; ++x) {
            for (int z = index.y - draw_distance; z <= index.y + draw_distance; ++z) {
                ivec2s chunk_index = (ivec2s) {{x, z}};
                chunk_t *chunk     = world_get_chunk(world, chunk_index);
                if (!chunk) {
                    chunk = world_add_chunk(world, chunk_index);
                }

                if (!chunk->flags.generated) {
                    world_gen_execute(chunk);
                }

                if (chunk->flags.dirty && chunk->flags.generated && !chunk->flags.mesh_generating) {
                    chunk_generate_mesh_async(chunk, shader_program, tilemap);
                }

                if (chunk->mesh) {
                    if (chunk->mesh->flags.ready_to_upload) {
                        mesh_upload(chunk->mesh);
                    }

                    if (chunk->mesh->flags.uploaded) {
                        vec3s position =
                            (vec3s) {{chunk->position.x * CHUNK_SIZE, 0.0f, chunk->position.y * CHUNK_SIZE}};
                        renderer_draw_mesh(chunk->mesh, position, (vec3s) {{0.0f, 0.0f, 0.0f}},
                                           (vec3s) {{1.0f, 1.0f, 1.0f}});
                    }
                }
            }
        }

        // world_renderer_render(world_renderer, world, camera_get_position(camera));

        renderer_end_frame();
    }

    for (size_t i = 0; i < MAX_THREADS; ++i) {
        pthread_join(world_gen_task_pool[i].thread, NULL);
        pthread_join(mesh_gen_task_pool[i].thread, NULL);
    }

    // world_generator_destroy(generator);
    // world_renderer_destroy(world_renderer);
    world_destroy(world);
    // world_renderer_destroy(world_renderer);
    shader_program_destroy(shader_program);
    tilemap_free(tilemap);

    renderer_deinit();
    window_deinit();
    profiling_deinit();

    fclose(log_fp);
    return 0;
}
