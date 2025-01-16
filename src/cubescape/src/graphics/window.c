#include "graphics/window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#include "core/log.h"

#define CALLBACK_ARRAY_SIZE 64

GLFWwindow* g_window = NULL;

static float last_frame = 0.0f;
static float delta_time = 0.0f;

static window_framebuffersize_callback_t framebuffersize_callbacks[CALLBACK_ARRAY_SIZE];
static size_t framebuffersize_callback_count = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    for (size_t i = 0; i < framebuffersize_callback_count; i++) {
        framebuffersize_callbacks[i]((ivec2s) {{width, height}});
    }
}

void glfw_error_callback(int error, const char* description) { LOG_ERROR("GLFW error: %s", description); }

void window_set_settings(window_settings_t settings) {
    if (!g_window) {
        LOG_ERROR("'window_set_settings' failed: no window created");
        return;
    }
    glfwSetWindowSize(g_window, settings.width, settings.height);
    glfwSetWindowTitle(g_window, settings.title);
}

window_settings_t window_get_settings() {
    window_settings_t settings = {0};
    if (!g_window) {
        LOG_ERROR("'window_get_settings' failed: no window created");
        return settings;
    }
    glfwGetWindowSize(g_window, &settings.width, &settings.height);
    settings.title = glfwGetWindowTitle(g_window);
    return settings;
}

void window_set_swap_interval(int interval) {
    if (!g_window) {
        LOG_ERROR("'window_set_swap_interval' failed: no window created");
        return;
    }
    glfwSwapInterval(interval);
}

int window_init(window_settings_t settings) {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        LOG_FATAL("Failed to initialize GLFW");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_SAMPLES, settings.multisample);

    g_window = glfwCreateWindow(settings.width, settings.height, settings.title, NULL, NULL);
    if (!g_window) {
        LOG_FATAL("Failed to create GLFW window");
        glfwTerminate();
        return 1;
    }

    glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);

    glfwMakeContextCurrent(g_window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_FATAL("Failed to initialize GLAD");
        glfwTerminate();
        return 1;
    }

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(g_window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    LOG_INFO("Window initialized: %s (%dx%d)", settings.title, settings.width, settings.height);

    return 0;
}

void window_deinit() {
    if (g_window) {
        glfwDestroyWindow(g_window);
        glfwTerminate();
        g_window = NULL;

        LOG_INFO("Window deinitialized");
    }
}

int window_should_close() { return g_window ? glfwWindowShouldClose(g_window) : 1; }

void window_swap_buffers() {
    if (!g_window) {
        LOG_ERROR("'window_swap_buffers' failed: no window created");
        return;
    }
    glfwSwapBuffers(g_window);
}

void window_poll_events() { glfwPollEvents(); }

void window_update_delta_time() {
    float current_frame = glfwGetTime();
    delta_time          = current_frame - last_frame;
    last_frame          = current_frame;
}

float window_get_delta_time() { return delta_time; }

ivec2s window_get_framebuffer_size() {
    if (!g_window) {
        LOG_ERROR("'window_get_framebuffer_size' failed: no window created");
        return (ivec2s) {{0, 0}};
    }

    ivec2s size = {0};
    glfwGetFramebufferSize(g_window, &size.x, &size.y);
    return size;
}

ivec2s window_get_size() {
    if (!g_window) {
        LOG_ERROR("'window_get_size' failed: no window created");
        return (ivec2s) {{0, 0}};
    }

    ivec2s size = {0};
    glfwGetWindowSize(g_window, &size.x, &size.y);
    return size;
}

void window_set_size(ivec2s size) {
    if (!g_window) {
        LOG_ERROR("'window_set_size' failed: no window created");
        return;
    }
    glfwSetWindowSize(g_window, size.x, size.y);
}

void window_add_framebuffersize_callback(window_framebuffersize_callback_t callback) {
    if (framebuffersize_callback_count < CALLBACK_ARRAY_SIZE) {
        framebuffersize_callbacks[framebuffersize_callback_count++] = callback;
    }
}
