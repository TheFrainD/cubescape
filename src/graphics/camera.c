#include "graphics/camera.h"

#include "core/input.h"
#include "core/log.h"
#include "core/math.h"
#include "graphics/window.h"
#include "graphics/buffer.h"

struct Camera {
    vec3 position;

    vec3 up;
    vec3 front;
    vec3 right;

    float yaw;
    float pitch;

    CameraSettings settings;

    uint32_t uniform_buffer;
};

#define NEAR 0.1f
#define FAR 100.0f

static struct Camera g_camera;
static vec3 world_up = (vec3){0.0f, 1.0f, 0.0f};

static void update_view() {
    mat4 view;
    vec3 center;
    glm_vec3_add(g_camera.position, g_camera.front, center);
    glm_lookat(g_camera.position, center, g_camera.up, view);
    buffer_sub_data(g_camera.uniform_buffer, &view, sizeof(mat4), sizeof(mat4), BUFFER_TARGET_UNIFORM_BUFFER);
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

    x_offset *= g_camera.settings.sensitivity;
    y_offset *= g_camera.settings.sensitivity;

    g_camera.pitch = CLAMP(g_camera.pitch + y_offset, -PI_2 + 0.01f, PI_2 - 0.01f);

    g_camera.yaw += x_offset;
    g_camera.yaw = (g_camera.yaw < 0 ? TAU : 0.0f) + fmod(g_camera.yaw, TAU);

    // Update front, right, and up vectors
    g_camera.front[0] = cos(g_camera.yaw) * cos(g_camera.pitch);
    g_camera.front[1] = sin(g_camera.pitch);
    g_camera.front[2] = sin(g_camera.yaw) * cos(g_camera.pitch);
    glm_vec3_normalize(g_camera.front);

    glm_vec3_cross(g_camera.front, world_up, g_camera.right);
    glm_vec3_normalize(g_camera.right);

    glm_vec3_cross(g_camera.right, g_camera.front, g_camera.up);
    glm_vec3_normalize(g_camera.up);

    update_view();
}

void camera_update_position() {
    float velocity = g_camera.settings.speed * get_delta_time();
    vec3 translation = {0.0f, 0.0f, 0.0f};

    if (key_pressed(KEY_W)) {
        vec3 temp;
        glm_vec3_scale(g_camera.front, velocity, temp);
        glm_vec3_add(translation, temp, translation);
    }
    if (key_pressed(KEY_S)) {
        vec3 temp;
        glm_vec3_scale(g_camera.front, velocity, temp);
        glm_vec3_sub(translation, temp, translation);
    }
    if (key_pressed(KEY_A)) {
        vec3 temp;
        glm_vec3_scale(g_camera.right, velocity, temp);
        glm_vec3_sub(translation, temp, translation);
    }
    if (key_pressed(KEY_D)) {
        vec3 temp;
        glm_vec3_scale(g_camera.right, velocity, temp);
        glm_vec3_add(translation, temp, translation);
    }

    glm_vec3_add(g_camera.position, translation, g_camera.position);

    // Update view if there was any translation
    if (!glm_vec3_eqv(translation, GLM_VEC3_ZERO)) {
        update_view();
    }
}

void camera_init(vec3 position, CameraSettings settings, uint32_t uniform_buffer) {
    glm_vec3_copy(position, g_camera.position);

    g_camera.settings = settings;
    g_camera.yaw = -PI_2;
    g_camera.pitch = 0.0f;

    g_camera.uniform_buffer = uniform_buffer;

    // Initialize front, up, and right vectors
    glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, g_camera.front);
    glm_vec3_copy(world_up, g_camera.up);
    glm_vec3_cross(g_camera.front, g_camera.up, g_camera.right);
    glm_vec3_normalize(g_camera.right);

    // Disable cursor
    set_cursor_enabled(0);

    add_mouse_position_callback(camera_mouse_callback);

    update_view();
}

void camera_set_position(vec3 position) {
    glm_vec3_copy(position, g_camera.position);
    update_view();
}

void camera_get_position(vec3 position) {
    glm_vec3_copy(g_camera.position, position);
}

void camera_set_settings(CameraSettings settings) {
    g_camera.settings = settings;
}

void camera_get_perpective(mat4 projection) {
    int width, height;
    get_framebuffer_size(&width, &height);
    glm_perspective(RAD(g_camera.settings.fov), (float)width / (float)height, NEAR, FAR, projection);
}
