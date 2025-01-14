#include "graphics/renderer.h"

#include <glad/glad.h>

#include "core/log.h"
#include "core/math.h"
#include "graphics/buffer.h"
#include "graphics/vertex.h"
#include "graphics/vertex_array.h"
#include "graphics/window.h"

typedef struct {
    uint32_t vertex_array;
    uint32_t uniform_buffer;

    renderer_state_t state;
} renderer_t;

static renderer_t renderer;

static void set_perspective(ivec2s size) {
    mat4s projection =
        glms_perspective(RAD(camera_get_settings(renderer.state.camera).fov), (float)size.x / (float)size.y,
                         renderer.state.near_clip, renderer.state.far_clip);
    buffer_sub_data(renderer.uniform_buffer, BUFFER_TARGET_UNIFORM_BUFFER, sizeof(mat4) * 2, sizeof(mat4), &projection);
}

void renderer_init(renderer_settings_t settings) {
    renderer.state.clear_color = settings.clear_color;
    renderer.state.near_clip   = settings.near_clip;
    renderer.state.far_clip    = settings.far_clip;

    renderer.vertex_array = vertex_array_create();
    renderer.uniform_buffer =
        buffer_create(sizeof(mat4) * 3, NULL, BUFFER_USAGE_STATIC_DRAW, BUFFER_TARGET_UNIFORM_BUFFER);
    renderer.state.camera = camera_create(settings.camera_settings);

    set_perspective(window_get_framebuffer_size());

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);

    glEnable(GL_MULTISAMPLE);

    // Default to solid rendering
    renderer.state.wireframe = 0;

    // Get the maximum anisotropy level
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &renderer.state.max_anisotropy);

    LOG_INFO("Renderer initialized");
}

void renderer_deinit() {
    vertex_array_destroy(&renderer.vertex_array);
    buffer_destroy(&renderer.uniform_buffer);
    camera_destroy(renderer.state.camera);

    LOG_INFO("Renderer deinitialized");
}

void renderer_begin_frame() {
    if (renderer.state.wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(renderer.state.clear_color.x, renderer.state.clear_color.y, renderer.state.clear_color.z, 1.0f);

    if (camera_view_changed(renderer.state.camera)) {
        mat4s view = camera_get_view(renderer.state.camera);
        buffer_sub_data(renderer.uniform_buffer, BUFFER_TARGET_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), &view);
        camera_view_reset(renderer.state.camera);
    }

    vertex_array_bind(renderer.vertex_array);
}

void renderer_end_frame() {
    vertex_array_unbind();
    window_swap_buffers();
}

void renderer_draw_mesh(mesh_t *mesh, vec3s position, vec3s rotation, vec3s scale) {
    if (!mesh) {
        LOG_ERROR("'renderer_draw_mesh' called with NULL mesh");
        return;
    }

    buffer_bind_base(renderer.uniform_buffer, BUFFER_TARGET_UNIFORM_BUFFER, 0);
    shader_program_bind_uniform_block(mesh->shader_program, "Matrices", 0);

    mesh_bind(mesh);

    mat4s model = glms_mat4_identity();
    model       = glms_translate(model, position);
    model       = glms_rotate(model, rotation.x, (vec3s) {{1.0f, 0.0f, 0.0f}});
    model       = glms_rotate(model, rotation.y, (vec3s) {{0.0f, 1.0f, 0.0f}});
    model       = glms_rotate(model, rotation.z, (vec3s) {{0.0f, 0.0f, 1.0f}});
    model       = glms_scale(model, scale);

    buffer_sub_data(renderer.uniform_buffer, BUFFER_TARGET_UNIFORM_BUFFER, 0, sizeof(mat4), &model);

    glDrawElements(GL_TRIANGLES, mesh_get_index_count(mesh), GL_UNSIGNED_INT, 0);

    mesh_unbind();
}

camera_t *renderer_get_camera() { return renderer.state.camera; }

uint32_t renderer_get_uniform_buffer() { return renderer.uniform_buffer; }

uint32_t renderer_get_vertex_array() { return renderer.vertex_array; }

void renderer_bind_vertex_buffer(uint32_t buffer) {
    vertex_array_bind(renderer.vertex_array);
    buffer_bind(buffer, BUFFER_TARGET_ARRAY_BUFFER);
    vertex_array_attrib(0, 3, VERTEX_ARRAY_DATA_TYPE_FLOAT, sizeof(vertex_t), (void *)offsetof(vertex_t, position));
    vertex_array_attrib(1, 2, VERTEX_ARRAY_DATA_TYPE_FLOAT, sizeof(vertex_t), (void *)offsetof(vertex_t, uv));
    buffer_unbind(BUFFER_TARGET_ARRAY_BUFFER);
    vertex_array_unbind();
}

renderer_state_t *renderer_get_state() { return &renderer.state; }
