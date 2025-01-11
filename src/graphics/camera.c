#include "graphics/camera.h"

#include "core/input.h"
#include "core/log.h"
#include "core/math.h"
#include "graphics/window.h"
#include "graphics/buffer.h"

#define NEAR 0.1f
#define FAR 100.0f

struct camera {
    vec3s position;

    vec3s up;
    vec3s front;
    vec3s right;

    float yaw;
    float pitch;

    camera_settings_t settings;

    mat4s view;
    int view_dirty;
};

static vec3s world_up = (vec3s){{0.0f, 1.0f, 0.0f}};

static void update_view(camera_t *camera) {
    if (camera == NULL) {
        LOG_ERROR("'update_view' called with NULL camera");
        return;
    }

    vec3s center = glms_vec3_add(camera->position, camera->front);
    camera->view = glms_lookat(camera->position, center, camera->up);
    camera->view_dirty = 1;
}

void camera_update_view(camera_t *camera, vec2s mouse_position) {
    static vec2s last_mouse_position = GLMS_VEC2_ZERO_INIT;
    static int first_mouse = 1;

    if (first_mouse) {
        last_mouse_position = mouse_position;
        first_mouse = 0;
    }

    vec2s offset = glms_vec2_scale(glms_vec2_sub(mouse_position, last_mouse_position), camera->settings.sensitivity);
    last_mouse_position = mouse_position;

    camera->pitch = CLAMP(camera->pitch - offset.y, -PI_2 + 0.01f, PI_2 - 0.01f);

    camera->yaw += offset.x;
    camera->yaw = (camera->yaw < 0 ? TAU : 0.0f) + fmod(camera->yaw, TAU);

    // Update front, right, and up vectors
    camera->front = glms_normalize((vec3s){{
        cos(camera->yaw) * cos(camera->pitch), 
        sin(camera->pitch), 
        sin(camera->yaw) * cos(camera->pitch)}});

    camera->right = glms_normalize(glms_vec3_cross(camera->front, world_up));

    camera->up = glms_normalize(glms_vec3_cross(camera->right, camera->front));

    update_view(camera);
}

void camera_destroy(camera_t *camera) {
    free(camera);
}

camera_t *camera_create(camera_settings_t settings) {
    camera_t *camera = malloc(sizeof(camera_t));
    camera->position = GLMS_VEC3_ZERO;
    camera->settings = settings;
    camera->yaw = -PI_2;
    camera->pitch = 0.0f;

    // Initialize front, up, and right vectors
    camera->front = (vec3s){{0.0f, 0.0f, -1.0f}};
    camera->up = world_up;
    camera->right = glms_normalize(glms_vec3_cross(camera->front, camera->up));

    update_view(camera);
    return camera;
}

void camera_set_position(camera_t *camera, vec3s position) {
    if (camera == NULL) {
        LOG_ERROR("'camera_set_position' called with NULL camera");
        return;
    }

    if (glms_vec3_eqv(camera->position, position)) {
        return;
    }

    camera->position = position;
    update_view(camera);
}

vec3s camera_get_position(camera_t *camera) {
    if (camera == NULL) {
        LOG_ERROR("'camera_get_position' called with NULL camera");
        return GLMS_VEC3_ZERO;
    }

    return camera->position;
}

camera_settings_t camera_get_settings(camera_t *camera) {
    if (camera == NULL) {
        LOG_ERROR("'camera_get_settings' called with NULL camera");
        return (camera_settings_t){0};
    }
    return camera->settings;
}

void camera_set_settings(camera_t *camera, camera_settings_t settings) {
    if (camera == NULL) {
        LOG_ERROR("'camera_set_settings' called with NULL camera");
        return;
    }
    camera->settings = settings;
}

mat4s camera_get_perspective(camera_t *camera) {
    if (camera == NULL) {
        LOG_ERROR("'camera_get_perspective' called with NULL camera");
        return GLMS_MAT4_ZERO;
    }

    int width, height;
    window_get_framebuffer_size(&width, &height);
    return glms_perspective(RAD(camera->settings.fov), (float)width / (float)height, NEAR, FAR);
}

mat4s camera_get_view(camera_t *camera) {
    if (camera == NULL) {
        LOG_ERROR("'camera_get_view' called with NULL camera");
        return GLMS_MAT4_ZERO;
    }
    return glms_lookat(camera->position, glms_vec3_add(camera->position, camera->front), camera->up);
}

int camera_view_changed(camera_t *camera) {
    if (camera == NULL) {
        LOG_ERROR("'camera_view_changed' called with NULL camera");
        return 0;
    }
    return camera->view_dirty;
}

void camera_view_reset(camera_t *camera) {
    if (camera == NULL) {
        LOG_ERROR("'camera_view_reset' called with NULL camera");
        return;
    }
    camera->view_dirty = 0;
}

vec3s camera_get_front(camera_t *camera) {
    if (camera == NULL) {
        LOG_ERROR("'camera_get_front' called with NULL camera");
        return GLMS_VEC3_ZERO;
    }
    return camera->front;
}

vec3s camera_get_right(camera_t *camera) {
    if (camera == NULL) {
        LOG_ERROR("'camera_get_right' called with NULL camera");
        return GLMS_VEC3_ZERO;
    }
    return camera->right;
}

vec3s camera_get_up(camera_t *camera) {
    if (camera == NULL) {
        LOG_ERROR("'camera_get_up' called with NULL camera");
        return GLMS_VEC3_ZERO;
    }
    return camera->up;
}

void camera_translate(camera_t *camera, vec3s translation) {
    if (camera == NULL) {
        LOG_ERROR("'camera_translate' called with NULL camera");
        return;
    }

    camera->position = glms_vec3_add(camera->position, translation);
    if (!glms_vec3_eqv(translation, GLMS_VEC3_ZERO)) {
        update_view(camera);
    }
}
