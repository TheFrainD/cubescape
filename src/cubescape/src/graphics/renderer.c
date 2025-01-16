#include "graphics/renderer.h"

#include <cubegl/buffer.h>
#include <cubegl/vertex_array.h>
#include <cubelog/cubelog.h>
#include <glad/glad.h>

#include "core/math.h"
#include "graphics/vertex.h"
#include "graphics/window.h"

typedef struct {
    buffer_t *uniform_buffer;

    renderer_state_t state;
} renderer_t;

static renderer_t renderer;

static void set_perspective(ivec2s size) {
    mat4s projection = glms_perspective(RAD(camera_get_fov(renderer.state.camera)), (float)size.x / (float)size.y,
                                        renderer.state.near_clip, renderer.state.far_clip);
    buffer_sub_data(renderer.uniform_buffer, sizeof(mat4) * 2, sizeof(mat4), &projection);
}

int renderer_init(renderer_settings_t settings) {
    renderer.state.clear_color = settings.clear_color;
    renderer.state.near_clip   = settings.near_clip;
    renderer.state.far_clip    = settings.far_clip;

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

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version  = glGetString(GL_VERSION);
    CUBELOG_INFO("Renderer: %s", renderer);
    CUBELOG_INFO("OpenGL version supported %s", version);

    CUBELOG_INFO("Renderer initialized");

    return 0;
}

void renderer_deinit() {
    buffer_destroy(renderer.uniform_buffer);
    camera_destroy(renderer.state.camera);

    CUBELOG_INFO("Renderer deinitialized");
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
        buffer_sub_data(renderer.uniform_buffer, sizeof(mat4), sizeof(mat4), &view);
        camera_view_reset(renderer.state.camera);
    }
}

void renderer_end_frame() {
    vertex_array_unbind();
    window_swap_buffers();
}

void renderer_draw_mesh(const mesh_t *const mesh, vec3s position, vec3s rotation, vec3s scale) {
    if (!mesh) {
        CUBELOG_ERROR("'renderer_draw_mesh' called with NULL mesh");
        return;
    }

    buffer_bind_base(renderer.uniform_buffer, 0);
    shader_program_bind_uniform_block(mesh->shader_program, "Matrices", 0);

    mesh_bind(mesh);

    mat4s model = glms_mat4_identity();
    model       = glms_translate(model, position);
    model       = glms_rotate(model, rotation.x, (vec3s) {{1.0f, 0.0f, 0.0f}});
    model       = glms_rotate(model, rotation.y, (vec3s) {{0.0f, 1.0f, 0.0f}});
    model       = glms_rotate(model, rotation.z, (vec3s) {{0.0f, 0.0f, 1.0f}});
    model       = glms_scale(model, scale);

    buffer_sub_data(renderer.uniform_buffer, 0, sizeof(mat4), &model);

    glDrawElements(GL_TRIANGLES, mesh_get_index_count(mesh), GL_UNSIGNED_INT, 0);

    mesh_unbind(mesh);
}

camera_t *renderer_get_camera() { return renderer.state.camera; }

renderer_state_t *renderer_get_state() { return &renderer.state; }
