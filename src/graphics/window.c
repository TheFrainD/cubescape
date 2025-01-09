#include "graphics/window.h"

#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "core/log.h"

GLFWwindow* g_window = NULL;

static float last_frame = 0.0f;
static float delta_time = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void glfw_error_callback(int error, const char* description) {
    LOG_ERROR("GLFW error: %s", description);
}

void window_set_title(const char* title) {
    if (!g_window) {
        LOG_ERROR("'set_window_title' failed: no window created");
        return;
    }
    glfwSetWindowTitle(g_window, title);
}

void get_window_size(int *width, int* height) {
    if (!g_window) {
        LOG_ERROR("'get_window_size' failed: no window created");
        return;
    }
    glfwGetWindowSize(g_window, width, height);
}

void set_window_size(int width, int height) {
    if (!g_window) {
        LOG_ERROR("'set_window_size' failed: no window created");
        return;
    }
    glfwSetWindowSize(g_window, width, height);
}

void set_swap_interval(int interval) {
    if (!g_window) {
        LOG_ERROR("'set_swap_interval' failed: no window created");
        return;
    }
    glfwSwapInterval(interval);
}

void window_init(WindowSettings settings) {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        LOG_FATAL("Failed to initialize GLFW");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
#endif

    g_window = glfwCreateWindow(settings.width, settings.height, settings.title, NULL, NULL);
    if (!g_window) {
        LOG_FATAL("Failed to create GLFW window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);

    glfwMakeContextCurrent(g_window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_FATAL("Failed to initialize GLAD");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(g_window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    LOG_INFO("Window created: %s (%dx%d)", settings.title, settings.width, settings.height);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    LOG_INFO("Renderer: %s", renderer);
    LOG_INFO("OpenGL version supported %s", version);
}


void window_destroy() {
    if (g_window) {
        glfwDestroyWindow(g_window);
        glfwTerminate();
        g_window = NULL;

        LOG_INFO("Window destroyed");
    }
}

int window_should_close() {
    return g_window ? glfwWindowShouldClose(g_window) : 1;
}

void swap_buffers() {
    if (!g_window) {
        LOG_ERROR("'swap_buffers' failed: no window created");
        return;
    }
    glfwSwapBuffers(g_window);
}

void poll_events() {
    glfwPollEvents();
}

void update_delta_time() {
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
}

float get_delta_time() {
    return delta_time;
}

void get_framebuffer_size(int *width, int* height) {
    if (!g_window) {
        LOG_ERROR("'get_framebuffer_size' failed: no window created");
        return;
    }
    glfwGetFramebufferSize(g_window, width, height);
}