#include "graphics/camera.h"

#include "core/input.h"
#include "core/log.h"
#include "core/math.h"
#include "graphics/window.h"
#include "graphics/buffer.h"

#define NEAR 0.1f
#define FAR 100.0f

struct camera {
    vec3 position;

    vec3 up;
    vec3 front;
    vec3 right;

    float yaw;
    float pitch;

    camera_settings_t settings;

    uint32_t uniform_buffer;
};

static struct camera camera;
static vec3 world_up = (vec3){0.0f, 1.0f, 0.0f};

static void update_view() {
    mat4 view;
    vec3 center;
    glm_vec3_add(camera.position, camera.front, center);
    glm_lookat(camera.position, center, camera.up, view);
    buffer_sub_data(camera.uniform_buffer, sizeof(mat4), sizeof(mat4), &view);
}

void camera_mouse_callback(double x, double y) {
    static double last_x;
    static double last_y;
    static int first_mouse = 1;

    // Initialize last x and y
    if (first_mouse) {
        last_x = x;
        last_y = y;
        first_mouse = 0;
    }

    double x_offset = x - last_x;
    double y_offset = last_y - y;

    last_x = x;
    last_y = y;

    x_offset *= camera.settings.sensitivity;
    y_offset *= camera.settings.sensitivity;

    camera.pitch = CLAMP(camera.pitch + y_offset, -PI_2 + 0.01f, PI_2 - 0.01f);

    camera.yaw += x_offset;
    camera.yaw = (camera.yaw < 0 ? TAU : 0.0f) + fmod(camera.yaw, TAU);

    // Update front, right, and up vectors
    camera.front[0] = cos(camera.yaw) * cos(camera.pitch);
    camera.front[1] = sin(camera.pitch);
    camera.front[2] = sin(camera.yaw) * cos(camera.pitch);
    glm_vec3_normalize(camera.front);

    glm_vec3_cross(camera.front, world_up, camera.right);
    glm_vec3_normalize(camera.right);

    glm_vec3_cross(camera.right, camera.front, camera.up);
    glm_vec3_normalize(camera.up);

    update_view();
}

void camera_update_position() {
    float velocity = camera.settings.speed * get_delta_time();
    vec3 translation = {0.0f, 0.0f, 0.0f};

    if (input_key_pressed(KEY_W)) {
        vec3 temp;
        glm_vec3_scale(camera.front, velocity, temp);
        glm_vec3_add(translation, temp, translation);
    }
    if (input_key_pressed(KEY_S)) {
        vec3 temp;
        glm_vec3_scale(camera.front, velocity, temp);
        glm_vec3_sub(translation, temp, translation);
    }
    if (input_key_pressed(KEY_A)) {
        vec3 temp;
        glm_vec3_scale(camera.right, velocity, temp);
        glm_vec3_sub(translation, temp, translation);
    }
    if (input_key_pressed(KEY_D)) {
        vec3 temp;
        glm_vec3_scale(camera.right, velocity, temp);
        glm_vec3_add(translation, temp, translation);
    }

    glm_vec3_add(camera.position, translation, camera.position);

    // Update view if there was any translation
    if (!glm_vec3_eqv(translation, GLM_VEC3_ZERO)) {
        update_view();
    }
}

void camera_init(vec3 position, camera_settings_t settings, uint32_t uniform_buffer) {
    glm_vec3_copy(position, camera.position);

    camera.settings = settings;
    camera.yaw = -PI_2;
    camera.pitch = 0.0f;

    camera.uniform_buffer = uniform_buffer;

    // Initialize front, up, and right vectors
    glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera.front);
    glm_vec3_copy(world_up, camera.up);
    glm_vec3_cross(camera.front, camera.up, camera.right);
    glm_vec3_normalize(camera.right);

    // Disable cursor
    input_set_cursor_enabled(0);

    input_add_mouse_position_callback(camera_mouse_callback);

    update_view();
}

void camera_set_position(vec3 position) {
    glm_vec3_copy(position, camera.position);
    update_view();
}

void camera_get_position(vec3 position) {
    glm_vec3_copy(camera.position, position);
}

camera_settings_t camera_get_settings() {
    return camera.settings;
}

void camera_set_settings(camera_settings_t settings) {
    camera.settings = settings;
}

void camera_get_perpective(mat4 projection) {
    int width, height;
    get_framebuffer_size(&width, &height);
    glm_perspective(RAD(camera.settings.fov), (float)width / (float)height, NEAR, FAR, projection);
}
